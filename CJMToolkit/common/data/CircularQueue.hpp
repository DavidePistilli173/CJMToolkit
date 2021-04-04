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

#ifndef CIRCULARQUEUE_HPP
#define CIRCULARQUEUE_HPP

#include <array>

namespace cjm::data
{
   /**
    * @brief Circular queue with a fixed size.
    * @tparam Type Type contained inside the queue.
    * @tparam Size Size of the queue.
    */
   template<typename Type, size_t Size>
   class CircularQueue
   {
   public:
      /**
       * @brief Default constructor.
       */
      constexpr CircularQueue() = default;

      /**
       * @brief Push an element into the queue.
       * @param newElement New element tu push into the queue.
       * @return Reference to the newly inserted element.
       */
      constexpr Type& push(const Type& newElement)
      {
         // Push a new element into the queue.
         data_[pushIdx_] = newElement;
         Type& returnValue = data_[pushIdx_];

         ++pushIdx_;
         ++currentSize_;

         // Cycle back to the beginning of the array.
         if (pushIdx_ == data_.size()) pushIdx_ = 0U;

         // Pop an element if the maximum size has been reached.
         if (currentSize_ > Size)
         {
            --currentSize_;
            pop();
         }

         return returnValue;
      }

      /**
       * @brief Pop an element from the queue.
       */
      constexpr void pop()
      {
         if (currentSize_ == 0U) return;

         if (popIdx_ == 0U) popIdx_ = data_.size();
         --popIdx_;
         --currentSize_;
      }

   private:
      std::array<Type, Size> data_; /**< Actual data contained inside the queue. */

      size_t pushIdx_{ 0U };     /**< Next index where a new element will be stored. */
      size_t popIdx_{ 0U };      /**< Next index from where an element will be extracted. */
      size_t currentSize_{ 0U }; /**< Actual size of the queue. */
   };
} // namespace cjm::data

#endif // CIRCULARQUEUE_HPP
