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

#include "DebugPanel.hpp"

DebugPanel::DebugPanel(QWidget* parent) : QWidget(parent) {}

bool DebugPanel::init()
{
   // Initialise the logger.
   logger_ = cjm::io::Log::logger();
   if (logger_ == nullptr) return false;

   // Initialise the UI.
   if (!initUI_()) return false;

   return true;
}

bool DebugPanel::initPageSelector_(PageSelectorPanel& panel)
{
   using Panel = PageSelectorPanel;

   panel.pageSelector = new cjm::qt::ButtonSelector(Panel::buttons);
   if (panel.pageSelector == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }

   return true;
}

bool DebugPanel::initUI_()
{
   using Panel = MainPanel;

   if (!initPageSelector_(mainPanel_.pageSelectorPanel))
   {
      logger_->error("Failed to initialise the page selector.");
      return false;
   }

   mainPanel_.layout = new QVBoxLayout();
   if (mainPanel_.layout == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }

   mainPanel_.layout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
   mainPanel_.layout->setSpacing(Panel::spacing);
   setLayout(mainPanel_.layout);

   mainPanel_.layout->insertWidget(
      static_cast<int>(Panel::Order::page_selector), mainPanel_.pageSelectorPanel.pageSelector);

   return true;
}
