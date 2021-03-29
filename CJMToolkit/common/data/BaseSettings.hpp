#ifndef BASESETTINGS_H
#define BASESETTINGS_H

#include "common/io/Log.hpp"

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace cjm::data
{
   /**
    * @brief Data structure for storing settings.
    * @details It is designed to be extended to support file I/O with the desired formats.
    */
   class BaseSettings
   {
   public:
      /********** DATA ************************************************************************************************/

      /**
       * @brief Node of the settings tree.
       */
      struct Node
      {
         std::string                                     value;      /**< Value corresponding to the node. */
         std::map<std::string, std::string, std::less<>> attributes; /**< Attributes of the node. */
         Node*                                           parent;     /**< Parent of the current node. */

         /**
          * @brief Children of the current node.
          */
         std::map<std::string, std::vector<std::unique_ptr<Node>>, std::less<>> children;
      };

      static constexpr std::string_view default_value{ "" }; /**< Default value for attribtues and nodes. */
      static constexpr long             last_node_idx{ -1 }; /**< Code used to identfy the last node of a vector. */

      /********** CONSTRUCTORS ****************************************************************************************/

      /**
       * @brief Construct an empty settings object.
       */
      BaseSettings();

      /**
       * @brief Create a settings object with a different root.
       * @param root New root of the tree.
       */
      BaseSettings(Node* root);

      /**
       * @brief Copy constructor.
       */
      BaseSettings(const BaseSettings&) = delete;

      /**
       * @brief Move constructor.
       */
      BaseSettings(BaseSettings&&) = default;

      /**
       * @brief Destructor.
       */
      virtual ~BaseSettings() = default;

      /********** OPERATORS *******************************************************************************************/

      /**
       * @brief Access the value of a node with a specific key.
       * @param nodeName Key of the desired node.
       * @param index Index of the desired node.
       * @return Value associated with the desired node. If no value is present, an empy string is returned. If multiple
       *         nodes with the same key are present, the value corresponding to the first one is returned.
       */
      std::string_view operator()(std::string_view nodeName, long index = 0) const;

      /********** METHODS *********************************************************************************************/

      /**
       * @brief Add a child node to the current node.
       * @param nodeName Name of the new node.
       * @param value Value of the new node.
       */
      void addNode(std::string_view nodeName, std::string_view value = default_value);

      /**
       * @brief Get the value associated with a specific attribute of the current node.
       * @param attributeName Name of the desired attribute.
       * @return Value of the desired attribute. If no value is present, an empy string is returned.
       */
      std::string_view attribute(std::string_view attributeName) const;

      /**
       * @brief Enter a node with the specified name, if it exists.
       * @param nodeName Name of the node.
       * @param index Index of the node, in case there are more nodes with the same name. -1 will enter the last node.
       * @return New BaseSettings object pointing to the desired node on success or on nullptr on failure.
       */
      BaseSettings enterNode(std::string_view nodeName, long index = 0) const;

      /**
       * @brief Perform a post-order visit of the settings tree and perform some action on each node.
       * @tparam Callable Function that will be applied to each node. Should accept a Node* as parameter.
       * @param function Function to apply to each node.
       */
      template<typename Callable>
      void postOrderVisit(Callable function)
      {
         postOrderRecursion_(function, currentNode_);
      }

      /**
       * @brief Perform a pre-order visit of the settings tree and perform some action on each node.
       * @tparam Callable Function that will be applied to each node. Should accept a Node* as parameter.
       * @param function Function to apply to each node.
       */
      template<typename Callable>
      void preOrderVisit(Callable function)
      {
         preOrderRecursion_(function, currentNode_);
      }

      /**
       * @brief Set an attribute of the current node.
       * @param attributeName Name of the attribute.
       * @param value Value of the attribute.
       */
      void setAttribute(std::string_view attributeName, std::string_view value);

      /**
       * @brief Set the value of the currently selected node.
       * @param value Value to set.
       */
      void setValue(std::string_view value);

      /**
       * @brief Check whether the settings object is valid.
       * @return true or false.
       */
      bool valid() const;

      /**
       * @brief Access the value of the current node.
       * @return Value associated with the desired node. If no value is present, an empy string is returned.
       */
      std::string_view value() const;

   protected:
      /********** METHODS *********************************************************************************************/

      /**
       * @brief Internal function that modifies the current node when entering a child.
       * @param nodeName Name of the node.
       * @param index Index of the node, in case there are more nodes with the same name. -1 will enter the last node.
       */
      void internalEnterNode_(std::string_view nodeName, long index = 0);

      /**
       * @brief Internal function that returns the settings tree to the parent of the current node.
       */
      void internalExitNode_();

      /**
       * @brief Internal function that brings back the current node to the root of the settings tree.
       */
      void internalReturnToRoot_();

      /********** VARIABLES *******************************************************************************************/

      cjm::io::Log* logger_{ nullptr }; /**< Message logger. */

      std::unique_ptr<Node> root_;                   /**< Actual root of the tree. */
      Node*                 currentNode_{ nullptr }; /**< Currently selected node. */

   private:
      /**
       * @brief Recursive part of a post-order visit.
       * @tparam Callable Function that will be applied to each node. Should accept a Node* as parameter.
       * @param function Function to apply to each node.
       * @param node Current node.
       */
      template<typename Callable>
      void postOrderRecursion_(Callable& function, Node* node)
      {
         if (node == nullptr) return;

         for (auto& childKey : node->children)
         {
            for (auto& childValue : childKey.second)
            {
               postOrderRecursion_(function, childValue.get());
            }
         }
         function(node);
      }

      /**
       * @brief Recursive part of a pre-order visit.
       * @tparam Callable Function that will be applied to each node. Should accept a Node* as parameter.
       * @param function Function to apply to each node.
       * @param node Current node.
       */
      template<typename Callable>
      void preOrderRecursion_(Callable& function, Node* node)
      {
         if (node == nullptr) return;

         function(node);
         for (auto& childKey : node->children)
         {
            for (auto& childValue : childKey.second)
            {
               preOrderRecursion_(function, childValue.get());
            }
         }
      }
   };
} // namespace cjm::data

#endif // BASESETTINGS_H
