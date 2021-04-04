/*
   MIT License

   Copyright (c) [2021] [Davide Dravindran Pistilli] [https://github.com/DavidePistilli173/CJMToolkit]

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#include "BaseSettings.hpp"

namespace cjm::data
{
   using cjm::io::Log;

   BaseSettings::BaseSettings()
   {
      using Log = cjm::io::Log;

      logger_ = Log::logger();

      // At least the root node should always exist.
      root_ = std::unique_ptr<Node>(new Node());
      if (root_ == nullptr)
      {
         logger_->error("Failed to allocate the root of the settings tree.");
      }

      currentNode_ = root_.get();
   }

   BaseSettings::BaseSettings(Node* root) : currentNode_{ root }
   {
      using Log = cjm::io::Log;
      logger_ = Log::logger();
   }

   std::string_view BaseSettings::operator()(std::string_view nodeName, long index) const
   {
      BaseSettings targetNode{ enterNode(nodeName, index) };
      if (targetNode.valid())
      {
         return targetNode.value();
      }
      else
      {
         logger_->warn("Node not found.", Log::pack("node name", nodeName), Log::pack("index", index));
         return default_value;
      }
   }

   void BaseSettings::addNode(std::string_view nodeName, std::string_view value)
   {
      if (valid())
      {
         // Retrieve the current nodes with the desired name.
         auto otherNodes = currentNode_->children.find(nodeName);

         if (otherNodes == currentNode_->children.end())
         {
            auto newNodeIterator =
               currentNode_->children.insert(std::pair<std::string, std::vector<std::unique_ptr<Node>>>(
                  nodeName, std::vector<std::unique_ptr<Node>>()));
            auto& newNode = newNodeIterator.first->second.emplace_back(new Node());
            newNode->value = value;
            newNode->parent = currentNode_;
         }
         else
         {
            auto& newNode = otherNodes->second.emplace_back(new Node());
            newNode->value = value;
            newNode->parent = currentNode_;
         }
      }
      else
      {
         logger_->warn(
            "Trying to add a node to a non-existent node.",
            Log::pack("node name", nodeName),
            Log::pack("value", value));
      }
   }

   std::string_view BaseSettings::attribute(std::string_view attributeName) const
   {
      if (valid())
      {
         auto targetAttribute = currentNode_->attributes.find(attributeName);
         if (targetAttribute != currentNode_->attributes.end())
         {
            return targetAttribute->second;
         }
         else
         {
            return default_value;
         }
      }
      else
      {
         logger_->warn(
            "Trying to retrieve an attribute from a non-existent node.", Log::pack("attribute name", attributeName));
         return default_value;
      }
   }

   BaseSettings BaseSettings::enterNode(std::string_view nodeName, long index) const
   {
      if (valid())
      {
         auto nodeVec = currentNode_->children.find(nodeName);
         if (nodeVec == currentNode_->children.end()) return BaseSettings(nullptr);

         if (index < 0 && index != last_node_idx)
         {
            logger_->warn("Passed invalid index to enterNode.", Log::pack("index", index));
            return BaseSettings(nullptr);
         }

         size_t uIndex;
         if (index == last_node_idx)
         {
            uIndex = nodeVec->second.size() - 1;
         }
         else
         {
            uIndex = static_cast<size_t>(index);
            if (nodeVec->second.size() <= uIndex) return BaseSettings(nullptr);
         }

         return BaseSettings(nodeVec->second[uIndex].get());
      }
      else
      {
         logger_->warn(
            "Trying to enter a child of a non-existent node.",
            Log::pack("node name", nodeName),
            Log::pack("index", index));
         return BaseSettings(nullptr);
      }
   }

   void BaseSettings::setAttribute(std::string_view attributeName, std::string_view value)
   {
      if (valid())
      {
         auto attribute = currentNode_->attributes.find(attributeName);
         if (attribute == currentNode_->attributes.end())
         {
            currentNode_->attributes.insert(std::pair<std::string, std::string>(attributeName, value));
         }
         else
         {
            attribute->second = value;
         }
      }
      else
      {
         logger_->warn(
            "Trying to add an attribute to a non-existent node.",
            Log::pack("attribute name", attributeName),
            Log::pack("value", value));
      }
   }

   void BaseSettings::setValue(std::string_view value)
   {
      if (valid())
      {
         currentNode_->value = value;
      }
      else
      {
         logger_->warn("Trying to set the value of a non-existent node.", Log::pack("value", value));
      }
   }

   bool BaseSettings::valid() const
   {
      return currentNode_ != nullptr;
   }

   std::string_view BaseSettings::value() const
   {
      if (valid())
      {
         return currentNode_->value;
      }
      else
      {
         logger_->warn("Trying to get the value of a non-existent node.");
         return default_value;
      }
   }

   void BaseSettings::internalEnterNode_(std::string_view nodeName, long index)
   {
      if (valid())
      {
         auto nodeVec = currentNode_->children.find(nodeName);
         if (nodeVec == currentNode_->children.end())
         {
            logger_->error("Node does not exist.", Log::pack("node name", nodeName), Log::pack("index", index));
            return;
         }

         if (index < 0 && index != last_node_idx)
         {
            logger_->error("Passed invalid index to enterNode.", Log::pack("index", index));
            return;
         }

         size_t uIndex;
         if (index == last_node_idx)
         {
            uIndex = nodeVec->second.size() - 1;
         }
         else
         {
            uIndex = static_cast<size_t>(index);
            if (nodeVec->second.size() <= uIndex)
            {
               logger_->error(
                  "Not enough nodes: index too high.",
                  Log::pack("index", uIndex),
                  Log::pack("number of nodes", nodeVec->second.size()));
               return;
            }
         }

         currentNode_ = nodeVec->second[uIndex].get();
      }
      else
      {
         logger_->error(
            "Trying to enter a child of a non-existent node.",
            Log::pack("node name", nodeName),
            Log::pack("index", index));
         return;
      }
   }

   void BaseSettings::internalExitNode_()
   {
      if (valid())
      {
         currentNode_ = currentNode_->parent;
      }
      else
      {
         logger_->error("Trying to exit a non-existent node.");
      }
   }

   void BaseSettings::internalReturnToRoot_()
   {
      if (valid())
      {
         currentNode_ = root_.get();
      }
      else
      {
         logger_->error("No root specified.");
      }
   }
} // namespace cjm::data
