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

#include "MainWindow.hpp"
#include "common/io/Log.hpp"
#include "common/qt/Settings.hpp"

#include <QApplication>
#include <QFile>
#include <string_view>

constexpr std::string_view settings_file{ "./config/settings.xml" };
constexpr std::string_view settings_root{ "CJMToolkit" };
constexpr std::string_view settings_main_window{ "MainWindow" };

constexpr std::string_view log_file{ "./log/log.txt" };

int main(int argc, char* argv[])
{
   using cjm::data::BaseSettings;
   using cjm::io::Log;
   using cjm::qt::Settings;

   if (!Log::init(log_file))
   {
      std::cout << "Failed to initialise logger.\n";
      return -1;
   }
   Log* logger{ Log::logger() };

   QApplication a(argc, argv);

   Settings settings{ settings_file.data(), Settings::Format::xml };
   if (settings.status() != Settings::Status::no_error)
   {
      logger->fatal("Failed to initialise the settings file.");
      return -1;
   }
   logger->info("Settings file loaded successfully.", settings_file);

   BaseSettings settingsRoot{ settings.enterNode(settings_root) };
   if (!settingsRoot.valid())
   {
      logger->error("No root settings node.", settings_root);
      return -1;
   }

   BaseSettings mainWindowSettings{ settingsRoot.enterNode(settings_main_window) };
   if (!mainWindowSettings.valid())
   {
      logger->warn("No settings for the main window.", settings_file, settings_main_window);
   }

   MainWindow w{ mainWindowSettings };
   w.show();

   return a.exec();
}
