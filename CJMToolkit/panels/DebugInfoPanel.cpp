#include "DebugInfoPanel.hpp"

#include "common/algorithm/utility.hpp"
#include "common/version_info.hpp"
#include "version_info.hpp"

DebugInfoPanel::DebugInfoPanel(QWidget* parent) : QWidget(parent) {}

bool DebugInfoPanel::init()
{
   logger_ = cjm::io::Log::logger();
   if (logger_ == nullptr) return false;

   if (!initUI_())
   {
      logger_->error("Failed to initialise the UI.");
      return false;
   }

   return true;
}

bool DebugInfoPanel::initLeftPanel_()
{
   using cjm::alg::constructObj;
   using cjm::alg::makeObj;
   using Panel = LeftPanel;
   LeftPanel& panel{ mainPanel_.leftPanel };

   if (!makeObj(logger_, panel.infoDisplay))
   {
      logger_->error("Failed to create the information display panel.");
      return false;
   }

   panel.infoDisplay->insertLabel(
      Panel::app_version, appVersion.getString(), static_cast<int>(Panel::Info::app_version));
   panel.infoDisplay->insertLabel(
      Panel::library_version, libVersion.getString(), static_cast<int>(Panel::Info::lib_version));

   if (!constructObj(logger_, panel.infoLayout))
   {
      logger_->error("Failed to create the information panel layout.");
      return false;
   }
   panel.infoLayout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
   panel.infoLayout->setSpacing(Panel::spacing);
   panel.infoLayout->addWidget(panel.infoDisplay);

   const char* str{ Panel::info_display_title.data() };
   if (!constructObj(logger_, panel.infoBox, std::tie(str)))
   {
      logger_->error("Failed to create the information panel box.");
      return false;
   }
   panel.infoBox->setContentsMargins(0, 0, 0, 0);
   panel.infoBox->setLayout(panel.infoLayout);

   if (!constructObj(logger_, panel.layout))
   {
      logger_->error("Failed to create the layout.");
      return false;
   }

   panel.layout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
   panel.layout->setSpacing(Panel::spacing);
   panel.layout->insertWidget(
      static_cast<int>(Panel::Order::info), panel.infoBox, Panel::info_stretch, Qt::AlignCenter);

   return true;
}

bool DebugInfoPanel::initRightPanel_()
{
   return true;
}

bool DebugInfoPanel::initUI_()
{
   using cjm::alg::constructObj;
   using Panel = MainPanel;

   if (!initLeftPanel_())
   {
      logger_->error("Failed to initialise the left panel.");
      return false;
   }

   if (!initRightPanel_())
   {
      logger_->error("Failed to initialise the right panel.");
      return false;
   }

   if (!constructObj(logger_, mainPanel_.layout))
   {
      logger_->error("Failed to create the main layout.");
      return false;
   }

   mainPanel_.layout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
   mainPanel_.layout->setSpacing(Panel::spacing);

   mainPanel_.layout->insertLayout(
      static_cast<int>(Panel::Order::left), mainPanel_.leftPanel.layout, Panel::left_stretch);

   setLayout(mainPanel_.layout);

   return true;
}
