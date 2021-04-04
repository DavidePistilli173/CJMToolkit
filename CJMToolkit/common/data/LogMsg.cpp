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

#include "LogMsg.hpp"

namespace cjm::data
{
   LogMsg::LogMsg(Level level, long long timestamp, std::string_view message) :
      level_{ level }, timestamp_{ timestamp }, msg_{ message }
   {
   }

   std::ostream& operator<<(std::ostream& stream, const LogMsg::Level& level)
   {
      stream << LogMsg::level_keys[static_cast<int>(level)];
      return stream;
   }

   std::stringstream& operator<<(std::stringstream& stream, const LogMsg::Level& level)
   {
      stream << LogMsg::level_keys[static_cast<int>(level)];
      return stream;
   }

   void LogMsg::addData(const Datagram& data)
   {
      data_.emplace_back(data);
   }

   void LogMsg::addData(const std::string& description, const std::string& data)
   {
      data_.emplace_back(description, data);
   }

   std::string LogMsg::baseMessage() const
   {
      return std::string(header_begin) + level_keys[static_cast<int>(level_)].data() + separator.data() +
             std::to_string(timestamp_) + time_unit.data() + header_end.data() + separator.data() + msg_;
   }
} // namespace cjm::data
