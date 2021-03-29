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

#include "Log.hpp"

namespace cjm::io
{
   /********** STATIC VARIABLES DEFINITIONS ***************************************************************************/
   bool                 Log::initialised_{ false };
   std::unique_ptr<Log> Log::logger_;

   /********** METHOD DEFINITIONS *************************************************************************************/

   bool Log::init(std::string_view logFile)
   {
      if (!initialised_)
      {
         // Initialise the logger's instance.
         logger_ = std::unique_ptr<Log>(new Log());
         if (logger_ == nullptr)
         {
            return false;
         }

         // Stop sync with stdio.
         std::ios_base::sync_with_stdio(false);

         // Initialise the logger.
         logger_->outputFile_ = std::ofstream(logFile.data());
         if (logger_->outputFile_.fail())
         {
            return false;
         }
         logger_->startTime_ = std::chrono::steady_clock::now();

         initialised_ = true;
      }

      return true;
   }

   Log::Level Log::level() const
   {
      return logLevel_;
   }

   Log* Log::logger()
   {
      return logger_.get();
   }

   void Log::setLevel(Level level)
   {
      logLevel_ = level;
      trace("Current log level set to:", logLevel_);
   }
} // namespace cjm::io
