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

#include "common/algorithm/utility.hpp"

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

bool DebugPanel::initPages_()
{
   using cjm::alg::constructObj;
   using cjm::alg::makeObj;
   PagePanel& panel{ mainPanel_.pagePanel };

   if (!makeObj(logger_, panel.infoPage))
   {
      logger_->error("Failed ot create the DEBUG::INFO page.");
      return false;
   }

   if (!constructObj(logger_, panel.layout))
   {
      logger_->error("Failed to create the page layout.");
      return false;
   }

   panel.layout->addWidget(panel.infoPage);
   panel.layout->setCurrentWidget(panel.infoPage);

   return true;
}

bool DebugPanel::initPageSelector_(PageSelectorPanel& panel)
{
   using cjm::alg::makeObj;
   using Panel = PageSelectorPanel;

   if (!makeObj(logger_, panel.pageSelector, std::tuple<>(), std::tie(Panel::buttons)))
   {
      logger_->error("Failed to create the page selector.");
      return false;
   }

   return true;
}

bool DebugPanel::initUI_()
{
   using cjm::alg::constructObj;
   using Panel = MainPanel;

   if (!initPageSelector_(mainPanel_.pageSelectorPanel))
   {
      logger_->error("Failed to initialise the page selector.");
      return false;
   }

   if (!initPages_())
   {
      logger_->error("Failed to initialise the pages.");
      return false;
   }

   if (!constructObj(logger_, mainPanel_.layout))
   {
      logger_->error("Failed to create the main layout.");
      return false;
   }

   mainPanel_.layout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
   mainPanel_.layout->setSpacing(Panel::spacing);
   setLayout(mainPanel_.layout);

   mainPanel_.layout->insertWidget(
      static_cast<int>(Panel::Order::page_selector),
      mainPanel_.pageSelectorPanel.pageSelector,
      Panel::selector_stretch);
   mainPanel_.layout->insertLayout(
      static_cast<int>(Panel::Order::page), mainPanel_.pagePanel.layout, Panel::page_stretch);

   return true;
}
