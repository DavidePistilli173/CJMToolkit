#include "DebugInfoPanel.hpp"

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
   using Panel = LeftPanel;
   LeftPanel& panel{ mainPanel_.leftPanel };

   panel.infoDisplay = new cjm::qt::InfoDisplay();
   if (panel.infoDisplay == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }
   if (!panel.infoDisplay->init())
   {
      logger_->error("Failed to initialise the information display panel.");
      return false;
   }

   panel.infoDisplay->insertLabel(
      Panel::app_version, appVersion.getString(), static_cast<int>(Panel::Info::app_version));
   panel.infoDisplay->insertLabel(
      Panel::library_version, libVersion.getString(), static_cast<int>(Panel::Info::lib_version));

   panel.infoLayout = new QVBoxLayout();
   if (panel.infoLayout == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }

   panel.infoLayout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
   panel.infoLayout->setSpacing(Panel::spacing);
   panel.infoLayout->addWidget(panel.infoDisplay);

   panel.infoBox = new QGroupBox(Panel::info_display_title.data(), this);
   if (panel.infoBox == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }
   panel.infoBox->setContentsMargins(0, 0, 0, 0);
   panel.infoBox->setLayout(panel.infoLayout);

   panel.layout = new QVBoxLayout();
   if (panel.layout == nullptr)
   {
      logger_->error("Memory allocation failed.");
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

   mainPanel_.layout = new QHBoxLayout();
   if (mainPanel_.layout == nullptr)
   {
      logger_->error("Memory allocation failed.");
      return false;
   }

   mainPanel_.layout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
   mainPanel_.layout->setSpacing(Panel::spacing);

   mainPanel_.layout->insertLayout(
      static_cast<int>(Panel::Order::left), mainPanel_.leftPanel.layout, Panel::left_stretch);

   setLayout(mainPanel_.layout);

   return true;
}
