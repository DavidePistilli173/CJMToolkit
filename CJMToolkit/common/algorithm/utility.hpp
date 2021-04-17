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

#ifndef COMMON_ALGORITHM_UTILITY_H
#define COMMON_ALGORITHM_UTILITY_H

#include "common/io/Log.hpp"

#include <tuple>

namespace cjm::alg
{
   /**
    * @brief Construct an object on the heap.
    * @tparam DstType Type of the pointer that will store the memory address of the newly created object.
    * @tparam TrueType True type of the object to create.
    * @tparam ConstructorArgs Arguments to pass to the constructor.
    * @param logger Message logger.
    * @param obj Reference to the pointer that will store the memory address of the newly created object.
    * @param constructorArgs Optional arguments to pass to the constructor.
    * @return true on success, false otherwise.
    */
   template<typename DstType, typename TrueType = DstType, typename... ConstructorArgs>
   bool constructObj(
      cjm::io::Log* logger, DstType*& obj, std::tuple<ConstructorArgs&...>&& constructorArgs = std::tuple<>())
   {
      // Allocate memory.
      auto create = [](auto... args) { return new TrueType(args...); };

      obj = std::apply(create, std::forward<std::tuple<ConstructorArgs&...>>(constructorArgs));

      // Error checking.
      if (obj == nullptr)
      {
         logger->error(
            "Memory allocation failed.",
            cjm::io::Log::pack("object type", typeid(obj).name()),
            cjm::io::Log::pack("object size [B]", sizeof(TrueType)));
         return false;
      }
      logger->trace(
         "Memory allocated.",
         cjm::io::Log::pack("object type", typeid(obj).name()),
         cjm::io::Log::pack("object size [B]", sizeof(TrueType)));

      return true;
   }

   /**
    * @brief Call the "init" method of a given object.
    * @tparam DstType Type of the pointer to the object.
    * @tparam InitArgs Arguments to pass to the init method.
    * @param logger Message logger.
    * @param obj Pointer to the object.
    * @param initArgs Optional arguments to pass to the init method.
    * @return true on success, false otherwise.
    */
   template<typename DstType, typename... InitArgs>
   bool initObj(cjm::io::Log* logger, DstType* obj, std::tuple<InitArgs&...>&& initArgs = std::tuple<>())
   {
      // Initialise the object.
      auto initialise = [obj](auto... args) { return obj->init(args...); };

      if (!std::apply(initialise, std::forward<std::tuple<InitArgs&...>>(initArgs)))
      {
         logger->error("Initialisation failed.", cjm::io::Log::pack("object type", typeid(obj).name()));
         return false;
      }
      logger->trace("Object initialised.", cjm::io::Log::pack("object type", typeid(obj).name()));

      return true;
   }

   /**
    * @brief Construct an object on the heap and call its "init" method.
    * @tparam DstType Type of the pointer that will store the memory address of the newly created object.
    * @tparam TrueType True type of the object to create.
    * @tparam ConstructorArgs Arguments to pass to the constructor.
    * @tparam InitArgs Arguments to pass to the init method.
    * @param logger Message logger.
    * @param obj Reference to the pointer that will store the memory address of the newly created object.
    * @param constructorArgs Optional arguments to pass to the constructor.
    * @param initArgs Optional arguments to pass to the init method.
    * @return true on success, false otherwise.
    */
   template<typename DstType, typename TrueType = DstType, typename... ConstructorArgs, typename... InitArgs>
   bool makeObj(
      cjm::io::Log*                     logger,
      DstType*&                         obj,
      std::tuple<ConstructorArgs&...>&& constructorArgs = std::tuple<>(),
      std::tuple<InitArgs&...>&&        initArgs = std::tuple<>())
   {
      if (!constructObj<DstType, ConstructorArgs..., TrueType>(
             logger, obj, std::forward<std::tuple<ConstructorArgs&...>>(constructorArgs)))
      {
         logger->error("Failed to create object.");
         return false;
      }

      if (!initObj(logger, obj, std::forward<std::tuple<InitArgs&...>>(initArgs)))
      {
         logger->error("Failed to initialise object.");
         delete obj;
         obj = nullptr;
         return false;
      }

      logger->trace("Object created and initialised.", cjm::io::Log::pack("object type", typeid(obj).name()));
      return true;
   }
} // namespace cjm::alg

#endif // COMMON_ALGORITHM_UTILITY_H
