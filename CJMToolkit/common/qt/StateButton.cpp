#include "StateButton.hpp"

#include <QtWidgets>

namespace cjm::qt
{
   StateButton::StateButton(const Data& data, QWidget* parent) : QPushButton(parent)
   {
      setText(data.label.data());
      value_ = data.value;

      connect(this, &QPushButton::clicked, this, [this]() { emit sigButtonClicked(value_); });
   }
} // namespace cjm::qt
