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

#include <QFile>

using cjm::io::Log;

MainWindow::MainWindow(cjm::data::BaseSettings& settings, QWidget* parent) :
   QMainWindow{ parent }, settings_{ settings }
{
}

MainWindow::~MainWindow() {}

bool MainWindow::init()
{
   // Initialise the logger.
   logger_ = cjm::io::Log::logger();
   if (logger_ == nullptr) return false;

   if (!settings_.valid())
   {
      logger_->error("Invalid settings.");
      return false;
   }

   if (!loadSettings_())
   {
      logger_->error("Failed to load settings");
      return false;
   }

   if (!initUI_())
   {
      logger_->error("Failed to initialise the UI.");
      return false;
   }

   return true;
}

bool MainWindow::initPages_(PagePanel& panel)
{
   using Panel = PagePanel;

   panel.debugPage = new DebugPanel();
   if (panel.debugPage == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }

   if (!panel.debugPage->init())
   {
      logger_->error("Failed to initialise the DEBUG page.");
      return false;
   }

   panel.layout = new QStackedLayout();
   if (panel.layout == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }

   panel.layout->setContentsMargins(
      Panel::margin_horizontal, Panel::margin_vertical, Panel::margin_horizontal, Panel::margin_vertical);
   panel.layout->setSpacing(Panel::spacing);

   panel.layout->insertWidget(static_cast<int>(PageSelectorPanel::Order::debug), panel.debugPage);
   panel.layout->setCurrentIndex(static_cast<int>(PageSelectorPanel::Order::debug));

   return true;
}

bool MainWindow::initPageSelector_(PageSelectorPanel& pageSelectorPanel)
{
   using Panel = PageSelectorPanel;

   pageSelectorPanel.pageSelector_ = new cjm::qt::ButtonSelector(Panel::buttons);
   if (pageSelectorPanel.pageSelector_ == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }

   return true;
}

bool MainWindow::initUI_()
{
   using Panel = MainPanel;

   if (!initWindow_())
   {
      logger_->error("Failed to initialise the main application window.");
      return false;
   }

   if (!initPageSelector_(mainPanel_.pageSelectorPanel))
   {
      logger_->error("Failed to initialise the page selector.");
      return false;
   }

   if (!initPages_(mainPanel_.pagePanel))
   {
      logger_->error("Failed to initialise the application pages.");
      return false;
   }

   mainPanel_.mainLayout = new QVBoxLayout();
   if (mainPanel_.mainLayout == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }
   mainPanel_.mainLayout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
   mainPanel_.mainLayout->setSpacing(Panel::spacing);

   mainPanel_.mainLayout->insertWidget(
      static_cast<int>(Panel::Order::page_selector),
      mainPanel_.pageSelectorPanel.pageSelector_,
      Panel::stretches[static_cast<int>(Panel::Order::page_selector)]);
   mainPanel_.mainLayout->insertLayout(
      static_cast<int>(Panel::Order::page),
      mainPanel_.pagePanel.layout,
      Panel::stretches[static_cast<int>(Panel::Order::page)]);

   mainPanel_.mainWidget = new QWidget();
   if (mainPanel_.mainWidget == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }

   mainPanel_.mainWidget->setContentsMargins(0, 0, 0, 0);
   setCentralWidget(mainPanel_.mainWidget);
   mainPanel_.mainWidget->setLayout(mainPanel_.mainLayout);

   return true;
}

bool MainWindow::initWindow_()
{
   // Set the title bar.
   setWindowTitle(QString(application_name.data()) + " " + appVersion_.getString().data());

   return true;
}

bool MainWindow::loadSettings_()
{
   if (!loadSizes_())
   {
      logger_->error("Failed to load window sizes.");
      return false;
   }

   if (!loadStyleSheets_())
   {
      logger_->error("Failed to load stylesheets.");
      return false;
   }

   return true;
}

bool MainWindow::loadSizes_()
{
   using cjm::data::BaseSettings;

   BaseSettings sizeSettings{ settings_.enterNode(Size::node) };
   if (sizeSettings.valid())
   {
      BaseSettings minimumSizeSettings{ sizeSettings.enterNode(Size::minimum) };
      if (minimumSizeSettings.valid())
      {
         std::string_view value;
         value = minimumSizeSettings(Size::width);
         if (value != BaseSettings::default_value)
         {
            int minimumWidth{ std::atoi(value.data()) };
            setMinimumWidth(minimumWidth);
            logger_->info("Minimum window width set.", Log::pack("minimum width", minimumWidth));
         }
         else
         {
            logger_->warn(
               "No minimum width specified for the main window.",
               Log::pack("previous node", Size::node),
               Log::pack("current node", Size::minimum),
               Log::pack("missing node", Size::width));
         }

         value = minimumSizeSettings(Size::height);
         if (value != BaseSettings::default_value)
         {
            int minimumHeight{ std::atoi(value.data()) };
            setMinimumHeight(minimumHeight);
            logger_->info("Mimimum window height set.", Log::pack("minimum height", minimumHeight));
         }
         else
         {
            logger_->warn(
               "No minimum height specified for the main window.",
               Log::pack("previous node", Size::node),
               Log::pack("current node", Size::minimum),
               Log::pack("missing node", Size::height));
         }
      }
      else
      {
         logger_->warn(
            "No minimum size section specified.",
            Log::pack("current node", Size::node),
            Log::pack("missing node", Size::minimum));
      }
   }
   else
   {
      logger_->warn("No size section specified.", Log::pack("missing node", Size::node));
   }

   return true;
}

bool MainWindow::loadStyleSheets_()
{
   using cjm::data::BaseSettings;

   BaseSettings styleSheetSettings{ settings_.enterNode(StyleSheet::name) };
   if (styleSheetSettings.valid())
   {
      std::string_view fileName;
      long             i{ 0 };
      for (fileName = styleSheetSettings(StyleSheet::file, i); fileName != BaseSettings::default_value;
           fileName = styleSheetSettings(StyleSheet::file, ++i))
      {
         if (QFile::exists(fileName.data()))
         {
            QFile stylesheetFile{ fileName.data() };
            stylesheetFile.open(QFile::OpenModeFlag::ReadOnly);
            setStyleSheet(stylesheetFile.readAll());
            logger_->info("Style-sheet set.", Log::pack("file name", fileName));
         }
         else
         {
            logger_->warn("Non-existent stylesheet file.", Log::pack("file name", fileName));
         }
      }
   }
   else
   {
      logger_->warn("No style-sheet section specified.", Log::pack("missing node", StyleSheet::name));
   }

   return true;
}
