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

#ifndef LOGMSG_HPP
#define LOGMSG_HPP

#include <array>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace cjm::data
{
   /**
    * @brief Data from a logging message.
    */
   class LogMsg
   {
   public:
      using Datagram = std::pair<std::string, std::string>;

      /**
       * @brief Logging levels.
       */
      enum class Level
      {
         trace,
         info,
         warn,
         error,
         fatal
      };

      /**
       * @brief Keywords for the logging levels.
       */
      static constexpr std::array<std::string_view, static_cast<size_t>(Level::fatal) + 1> level_keys{
         "TRC", "INF", "WRN", "ERR", "FTL"
      };

      static constexpr std::string_view time_unit{ "ms" };   /**< Time unit of measurement. */
      static constexpr std::string_view header_begin{ "[" }; /**< Beginning of the message header. */
      static constexpr std::string_view header_end{ "]" };   /**< End of the message header. */
      static constexpr std::string_view separator{ " - " };  /**< Separator between header elements. */

      /**
       * @brief Default constructor.
       */
      LogMsg() = default;

      /**
       * @brief Constructor.
       * @param level Error level of the message.
       * @param timestame Timestamp of the message [ms].
       * @param message Text message.
       */
      LogMsg(Level level, long long timestamp, std::string_view message);

      friend std::ostream& operator<<(std::ostream& stream, const LogMsg::Level& level);

      friend std::stringstream& operator<<(std::stringstream& stream, const LogMsg::Level& level);

      /**
       * @brief Add further details to the message.
       * @param data Datagrams containing additional data.
       */
      void addData(const Datagram& data);

      /**
       * @brief Add further details to the message.
       * @param description Description of the data.
       * @param data Data to be stored.
       */
      void addData(const std::string& description, const std::string& data);

      /**
       * @brief Add further details to the message.
       * @tparam Type of the description.
       * @tparam Type of the data.
       * @param data Data to be stored.
       */
      template<typename DescriptionType, typename DataType>
      void addData(const std::pair<DescriptionType, DataType> data)
      {
         std::stringstream descriptionStream;
         descriptionStream << data.first;
         std::stringstream dataStream;
         dataStream << data.second;

         data_.emplace_back(descriptionStream.str(), dataStream.str());
      }

      /**
       * @brief Obtain the base message that can be sent to stdout or to file.
       * @return Log message with the header information.
       */
      std::string baseMessage() const;

   private:
      Level                 level_{ Level::trace }; /**< Level of the message. */
      long long             timestamp_{ 0U };       /**< Timespamt of the message. */
      std::string           msg_;                   /**< Actual text message. */
      std::vector<Datagram> data_;                  /**< Additional data attached to the message. */
   };
} // namespace cjm::data

#endif // LOGMSG_HPP
