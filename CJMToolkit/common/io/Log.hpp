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

#ifndef LOG_HPP
#define LOG_HPP

#include "common/data/CircularQueue.hpp"
#include "common/data/LogMsg.hpp"

#include <array>
#include <atomic>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <string_view>
#include <type_traits>

namespace cjm::io
{
   /**
    * @brief Class for logging information.
    */
   class Log
   {
   public:
      using LogMsg = cjm::data::LogMsg;

      /**
       * @brief Special codes used in the logging messages.
       */
      struct Codes
      {
         static constexpr std::string_view ansi_reset{ "\033[0m" };    /**< Reset the console font. */
         static constexpr std::string_view ansi_bold{ "\033[1m" };     /**< Set the ANSI console font to bold. */
         static constexpr std::string_view ansi_black{ "\033[30m" };   /**< Set the ANSI console font to black. */
         static constexpr std::string_view ansi_red{ "\033[31m" };     /**< Set the ANSI console font to red. */
         static constexpr std::string_view ansi_green{ "\033[32m" };   /**< Set the ANSI console font to green. */
         static constexpr std::string_view ansi_yellow{ "\033[33m" };  /**< Set the ANSI console font to yellow. */
         static constexpr std::string_view ansi_blue{ "\033[34m" };    /**< Set the ANSI console font to blue. */
         static constexpr std::string_view ansi_magenta{ "\033[35m" }; /**< Set the ANSI console font to magenta. */
         static constexpr std::string_view ansi_cyan{ "\033[36m" };    /**< Set the ANSI console font to cyan. */
         static constexpr std::string_view ansi_white{ "\033[37m" };   /**< Set the ANSI console font to white. */
      };

      /**
       * @brief ANSI colour codes for the logging levels.
       */
      static constexpr std::array<std::string_view, static_cast<size_t>(LogMsg::Level::fatal) + 1> level_ansi_colours{
         Codes::ansi_reset, Codes::ansi_white, Codes::ansi_yellow, Codes::ansi_red, Codes::ansi_magenta
      };

      /**
       * @brief Separator between the level keyword and the message.
       */
      static constexpr std::string_view level_separator{ " - " };

      static constexpr std::string_view header_left_bracket{ "[" };  /**< Left bracket for the header. */
      static constexpr std::string_view header_right_bracket{ "]" }; /**< Right bracket for the header. */
      static constexpr std::string_view time_ms{ "ms" };             /**< Millseconds in text. */
      static constexpr std::string_view tab{ "   " };                /**< Tab size for log contents. */

      static constexpr size_t queue_size{ 1024 }; /**< Number of messages that will be stored in a single queue. */

      /********** METHODS *********************************************************************************************/

      /**
       * @brief Log an error message.
       */
      template<typename... Args>
      void error(std::string_view msg, const Args&... args)
      {
         log(LogMsg::Level::error, msg, args...);
      }

      /**
       * @brief Log a fatal error message.
       */
      template<typename... Args>
      void fatal(std::string_view msg, const Args&... args)
      {
         log(LogMsg::Level::fatal, msg, args...);
      }

      /**
       * @brief Initialise the logger.
       * @param logFile Path of the output file to use for logging.
       * @return true on success, false otherwise.
       */
      static bool init(std::string_view logFile);

      /**
       * @brief Log an information message.
       */
      template<typename... Args>
      void info(std::string_view msg, const Args&... args)
      {
         log(LogMsg::Level::info, msg, args...);
      }

      /**
       * @brief Get the current logging level.
       * @return Current logging level.
       */
      LogMsg::Level level() const;

      /**
       * @brief Log a message and optional additional information.
       * @param level Desired logging level.
       * @param msg Message to print on the first line.
       * @param args Optional variables to print. Should be a pair (description, variable).
       */
      template<typename... Args>
      void log(LogMsg::Level level, std::string_view msg, const Args&... args)
      {
         if (!initialised_) return;

         // Create and store the logging message and data.
         auto timestamp{
            std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime_).count()
         };

         std::scoped_lock lck{ ioMtx_ };
         auto&            newMessage = messages_[static_cast<int>(level)].push(LogMsg(level, timestamp, msg));
         // If there is additional data, add it to the message.
         if constexpr (sizeof...(Args) > 0) (newMessage.addData(args), ...);

         // If the message level is high enough, print the basic message information.
         if (level >= logLevel_)
         {
            // Log on stdio.
            std::cout << level_ansi_colours[static_cast<int>(level)] << newMessage.baseMessage() << '\n';

            // Log on file.
            outputFile_ << newMessage.baseMessage() << '\n';
         }
      }

      /**
       * @brief Get the current instance of the logger.
       * @return Pointer to the logger. Can be nullptr.
       */
      static Log* logger();

      /**
       * @brief Pack all information necessary for a logging datagram.
       * @tparam Data type to store.
       * @param description Description of the data.
       * @param data Data to store.
       * @return Pair containing both the description
       */
      template<typename T>
      static constexpr std::pair<const char*, const T&> pack(const char* description, const T& data)
      {
         return std::pair<const char*, const T&>(description, data);
      }

      /**
       * @brief Pack all information necessary for a logging datagram.
       * @tparam Data type to store.
       * @param description Description of the data.
       * @param data Data to store.
       * @return Pair containing both the description
       */
      template<typename T>
      static constexpr std::pair<const char*, const T&> pack(std::string_view description, const T& data)
      {
         return std::pair<const char*, const T&>(description.data(), data);
      }

      /**
       * @brief Log a trace message.
       */
      template<typename... Args>
      void trace(std::string_view msg, const Args&... args)
      {
         log(LogMsg::Level::trace, msg, args...);
      }

      /**
       * @brief Set the logging level.
       * @param level New logging level.
       */
      void setLevel(LogMsg::Level level);

      /**
       * @brief Log a warning message.
       */
      template<typename... Args>
      void warn(std::string_view msg, const Args&... args)
      {
         log(LogMsg::Level::warn, msg, args...);
      }

   private:
      /**
       * @brief Constructor.
       */
      Log() = default;

      static bool                 initialised_; /**< true if the logger was initialised. */
      static std::unique_ptr<Log> logger_;      /**< Single instance of the logger. */

      std::mutex    ioMtx_;      /**< Mutex for protecting the standard IO stream. */
      std::ofstream outputFile_; /**< Logging file. */

      std::atomic<LogMsg::Level> logLevel_{ LogMsg::Level::trace };  /**< Current logging level. */
      std::chrono::time_point<std::chrono::steady_clock> startTime_; /**< Starting time of the program. */

      /**
       * @brief Message queues.
       */
      std::array<cjm::data::CircularQueue<LogMsg, queue_size>, LogMsg::level_keys.size()> messages_;
   };
} // namespace cjm::io

#endif // LOG_HPP
