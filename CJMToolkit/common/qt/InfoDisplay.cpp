#include "InfoDisplay.hpp"

#include "common/algorithm/utility.hpp"

namespace cjm::qt
{
   using cjm::io::Log;

   InfoDisplay::InfoDisplay(QWidget* parent) : QWidget(parent) {}

   bool InfoDisplay::init()
   {
      using cjm::alg::constructObj;
      using Panel = MainPanel;

      logger_ = cjm::io::Log::logger();
      if (logger_ == nullptr) return false;

      if (!constructObj(logger_, mainPanel_.layout))
      {
         logger_->error("Failed to create the layout.");
         return false;
      }

      mainPanel_.layout->setContentsMargins(Panel::margin, Panel::margin, Panel::margin, Panel::margin);
      mainPanel_.layout->setSpacing(Panel::spacing);
      setLayout(mainPanel_.layout);

      initialised_ = true;
      return true;
   }

   void InfoDisplay::insertLabel(std::string_view title, std::string_view value, int row)
   {
      using cjm::alg::constructObj;

      if (!initialised_)
      {
         logger_->error("InfoDisplay not initialised.");
         return;
      }

      if (row < 0)
      {
         logger_->warn("Cannot use a negative row number.", Log::pack("used row", row));
         return;
      }

      if (size_t currentSize{ mainPanel_.infoLabels.size() }; currentSize <= static_cast<unsigned int>(row))
      {
         mainPanel_.infoLabels.resize(row + 1);

         for (unsigned int i = currentSize; i < mainPanel_.infoLabels.size(); ++i)
         {
            if (!(constructObj(logger_, mainPanel_.infoLabels[i].first) &&
                  constructObj(logger_, mainPanel_.infoLabels[i].second)))
            {
               logger_->error(
                  "Failed to create the labels.",
                  Log::pack("requested row", row),
                  Log::pack("current number of rows", i));
               mainPanel_.infoLabels.resize(i);
               return;
            }
         }
      }

      mainPanel_.infoLabels[row].first->setText(title.data());
      mainPanel_.infoLabels[row].second->setText(value.data());
      mainPanel_.layout->addWidget(mainPanel_.infoLabels[row].first, row, 0, Qt::AlignLeft);
      mainPanel_.layout->addWidget(mainPanel_.infoLabels[row].second, row, 1, Qt::AlignRight);
   }

   void InfoDisplay::setValue(std::string_view value, int row)
   {
      if (!initialised_)
      {
         logger_->error("InfoDisplay not initialised.");
         return;
      }

      if (row < 0)
      {
         logger_->warn("Tried to access a negative row.", Log::pack("requested row", row));
         return;
      }

      if (mainPanel_.infoLabels.size() <= static_cast<unsigned int>(row))
      {
         logger_->warn(
            "Non-existent row.",
            Log::pack("requested row", row),
            Log::pack("number of rows", mainPanel_.infoLabels.size()));
         return;
      }

      mainPanel_.infoLabels[row].second->setText(value.data());
   }
} // namespace cjm::qt
