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

MainWindow::MainWindow(cjm::data::BaseSettings& settings, QWidget* parent) :
   QMainWindow{ parent }, settings_{ settings }
{
   logger_ = cjm::io::Log::logger();

   if (!settings_.valid())
   {
      logger_->error("Invalid settings.");
   }
   else
   {
      loadSizes_();
      loadStyleSheets_();
      initUI_();
   }
}

MainWindow::~MainWindow() {}

void MainWindow::initPageSelector_(PageSelectorPanel& pageSelectorPanel)
{
   using Panel = PageSelectorPanel;

   pageSelectorPanel.pageSelector_ = new cjm::qt::ButtonSelector(Panel::buttons);
}

void MainWindow::initUI_()
{
   using Panel = MainPanel;

   initPageSelector_(mainPanel_.pageSelectorPanel);

   mainPanel_.mainLayout = new QVBoxLayout();
   mainPanel_.mainLayout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
   mainPanel_.mainLayout->setSpacing(Panel::spacing);

   mainPanel_.mainLayout->insertWidget(
      static_cast<int>(Panel::Order::page_selector), mainPanel_.pageSelectorPanel.pageSelector_);

   mainPanel_.mainWidget = new QWidget();
   mainPanel_.mainWidget->setContentsMargins(0, 0, 0, 0);
   setCentralWidget(mainPanel_.mainWidget);
   mainPanel_.mainWidget->setLayout(mainPanel_.mainLayout);
}

void MainWindow::loadSizes_()
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
            logger_->info("Minimum window width set.", minimumWidth);
         }
         else
         {
            logger_->warn("No minimum width specified for the main window.", Size::node, Size::minimum, Size::width);
         }

         value = minimumSizeSettings(Size::height);
         if (value != BaseSettings::default_value)
         {
            int minimumHeight{ std::atoi(value.data()) };
            setMinimumHeight(minimumHeight);
            logger_->info("Mimimum window height set.", minimumHeight);
         }
         else
         {
            logger_->warn("No minimum height specified for the main window.", Size::node, Size::minimum, Size::height);
         }
      }
      else
      {
         logger_->warn("No minimum size section specified.", Size::node, Size::minimum);
      }
   }
   else
   {
      logger_->warn("No size section specified.", Size::node);
   }
}

void MainWindow::loadStyleSheets_()
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
            logger_->info("Style-sheet set.", fileName);
         }
         else
         {
            logger_->warn("Non-existent stylesheet file.", fileName);
         }
      }
   }
   else
   {
      logger_->warn("No style-sheet section specified.", StyleSheet::name);
   }
}
