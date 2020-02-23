//@group UI

#include "UIButton.h"
#include "MessageBus.h"

namespace Engine
{
  UIButton::UIButton(std::string const& a_name, vec3 a_trans, vec3 a_scale)
    : UIWidget(a_name, a_trans, a_scale, true)
  {

  }

  UIButton::~UIButton()
  {

  }

  bool UIButton::HandleNewCursonPostion(float a_x, float a_y)
  {
    if (IsInside(a_x, a_y))
      return true;
    return false;
  }

  void UIButton::DoAction(Action a_action)
  {
    int index = static_cast<int>(a_action);

    switch (a_action)
    {
      case Action::Activate:
      {
        LOG_ERROR("ACTIVATE");
        break;
      }
      case Action::HoverOn:
      {
        LOG_WARN("HOVER ON");
        break;
      }
      case Action::HoverOff:
      {
        LOG_DEBUG("HOVER OFF");
        break;
      }
    }

    if (m_bindings[index])
      MessageBus::Instance()->Register(m_bindings[index]->CloneAsTRef());
  }

  void UIButton::Render()
  {

  }
}