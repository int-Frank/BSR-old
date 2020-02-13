#include "Canvas.h"
#include "DgR2QueryPointAABB.h"

namespace Engine
{
  //------------------------------------------------------------------------------------
  //  Helper functions
  //------------------------------------------------------------------------------------
  static void InsertWidget(UIWidget* a_pWgt, Dg::DoublyLinkedList<UIWidget*> & a_out)
  {
    bool inserted = false;
    for (auto it = a_out.begin(); it != a_out.end(); it++)
    {
      if (a_pWgt->GetDepth() <= (*it)->GetDepth())
      {
        a_out.insert(it, a_pWgt);
        inserted = true;
      }
    }

    if (!inserted)
      a_out.push_back(a_pWgt);
  }

  //------------------------------------------------------------------------------------
  //  UICanvas
  //------------------------------------------------------------------------------------
  UICanvas::UICanvas()
    : m_pFocus(nullptr)
  {

  }

  UICanvas::~UICanvas()
  {
    Clear();
  }

  void UICanvas::Clear()
  {
    for (UIWidget * ptr : m_widgets)
      delete ptr;

    m_widgets.clear();
    m_pFocus = nullptr;
  }

  void UICanvas::Add(UIWidget* a_pWgt)
  {
    InsertWidget(a_pWgt, m_widgets);
  }

  void UICanvas::HandleNewCursonPostion(float a_x, float a_y)
  {
    for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
    {
      if ((*it)->HandleNewCursonPostion(a_x, a_y))
      {
        m_pFocus = *it;
        return;
      }
    }
    m_pFocus = nullptr;
  }

  void UICanvas::Activate()
  {
    if (m_pFocus)
    {
      m_pFocus->Activate();
    }
  }

  //------------------------------------------------------------------------------------
  //  UIWidget
  //------------------------------------------------------------------------------------

  UIWidget::UIWidget(std::string const& a_name, 
    vec3 a_trans, vec3 a_scale,
    bool a_isInteractive, int a_depth)
    : m_hasFocus(false)
    , m_depth(a_depth)
    , m_pParent(nullptr)
    , m_translation(a_trans)
    , m_scale(a_scale)
    , m_name(a_name)
    , m_isInteractive(a_isInteractive)
    , m_bindings{0}
  {
    SetGlobalToLocal();
  }

  UIWidget::~UIWidget()
  {
    for (int i = 0; i < ACTION_COUNT; i++)
      delete m_bindings[i];
  }

  bool UIWidget::IsInside(float a_x, float a_y)
  {
    vec3 point(a_x, a_y, 1.0f);
    point = point * m_T_Global_to_Local;

    bool result = point.x() > -1.0f;
    result = result && point.x() < 1.0f;
    result = result && point.y() > -1.0f;
    result = result && point.y() < 1.0f;

    return result;
  }

  void UIWidget::SetFocus(bool a_val)
  {
    m_hasFocus = a_val;
  }

  bool UIWidget::HandleNewCursonPostion(float a_x, float a_y)
  {
    return false;
  }

  void UIWidget::Bind(Action a_action, Message * a_pMsg)
  {
    int index = static_cast<int>(a_action);
    delete m_bindings[index];
    m_bindings[index] = a_pMsg;
  }

  bool UIWidget::HasFocus() const
  {
    return m_hasFocus;
  }

  void UIWidget::Translate(float a_x, float a_y)
  {
    m_translation += vec3(a_x, a_y, 0.0f);
    SetGlobalToLocal();
    NewTransform();
  }

  void UIWidget::Scale(float a_x, float a_y)
  {
    m_scale += vec3(a_x, a_y, 0.0f);
    SetGlobalToLocal();
    NewTransform();
  }

  void UIWidget::SetTranslation(float a_x, float a_y)
  {
    m_translation.Set(a_x, a_y, 0.0f);
    SetGlobalToLocal();
    NewTransform();
  }

  void UIWidget::SetScale(float a_x, float a_y)
  {
    m_scale.Set(a_x, a_y, 0.0f);
    SetGlobalToLocal();
    NewTransform();
  }

  void UIWidget::SetDepth(int a_val)
  {
    m_depth = a_val;
    if (m_pParent)
      m_pParent->DepthHasChanged(this);
  }

  int UIWidget::GetDepth() const
  {
    return m_depth;
  }

  mat3 UIWidget::GetGlobalToLocal() const
  {
    return m_T_Global_to_Local;
  }

  mat3 UIWidget::GetLocalToParent() const
  {
    mat3 mt, ms;
    mt.Translation(m_translation);
    ms.Scaling(m_scale);
    return ms * mt;
  }

  AABB const & UIWidget::GetInteractiveAABB() const
  {
    return m_interactiveAABB;
  }

  bool UIWidget::IsInteractive() const
  {
    return m_isInteractive;
  }

  void UIWidget::SetIsInteractive(bool a_val)
  {
    m_isInteractive = a_val;
  }

  void UIWidget::SetGlobalToLocal()
  {
    mat3 T_Local_to_Parent = GetLocalToParent();
    mat3 T_Local_to_Global;
    if (m_pParent)
    {
      mat3 T_Parent_to_Global = m_pParent->GetGlobalToLocal().GetInverse();
      T_Local_to_Global = T_Local_to_Parent * T_Parent_to_Global;
    }
    else
      T_Local_to_Global = T_Local_to_Parent;

    m_T_Global_to_Local = T_Local_to_Global.GetInverse();
  }

  void UIWidget::SetParent(UIWidget* a_pParent)
  {
    m_pParent = a_pParent;
  }

  //------------------------------------------------------------------------------------
  //  UIGroup
  //------------------------------------------------------------------------------------
  UIGroup::UIGroup(std::string const&a_name, vec3 a_trans, vec3 a_scale)
    : UIWidget(a_name, a_trans, a_scale, false)
    , m_pFocus(nullptr)
  {

  }

  UIGroup::~UIGroup()
  {
    Clear();
  }

  bool UIGroup::HandleNewCursonPostion(float a_x, float a_y)
  {
    if (!IsInside(a_x, a_y))
      return false;

    if (m_pFocus)
      m_pFocus->HoverOff();
    m_pFocus = nullptr;

    for (UIWidget * wgt : m_children)
    {
      if (wgt->HandleNewCursonPostion(a_x, a_y))
      {
        m_pFocus = wgt;
        return true;
      }
    }
    return false;
  }

  void UIGroup::Activate()
  {
    if (m_pFocus)
      m_pFocus->Activate();
  }

  void UIGroup::Render()
  {
    for (UIWidget* child : m_children)
      child->Render();
  }

  void UIGroup::Clear()
  {
    for (UIWidget* child : m_children)
      delete child;
    m_children.clear();
    m_pFocus = nullptr;
    SetInteractiveAABB();
  }

  void UIGroup::Add(UIWidget* a_pWgt)
  {
    InsertWidget(a_pWgt, m_children);
    a_pWgt->SetParent(this);
    a_pWgt->SetGlobalToLocal();
    a_pWgt->NewTransform();
    SetInteractiveAABB();
  }

  bool UIGroup::Remove(UIWidget* a_pWgt)
  {
    for (auto it = m_children.begin(); it != m_children.end(); it++)
    {
      if ((*it) == a_pWgt)
      {
        m_children.erase(it);
        SetInteractiveAABB();
        return true;
      }
    }
    return false;
  }

  void UIGroup::DepthHasChanged(UIWidget* a_pWgt)
  {
    if (Remove(a_pWgt))
      Add(a_pWgt);
  }

  void UIGroup::NewTransform()
  {
    for (UIWidget* child : m_children)
      child->SetGlobalToLocal();
  }

  void UIGroup::SetInteractiveAABB()
  {
    m_isInteractive = false;
    Dg::DoublyLinkedList<UIWidget*>::iterator it = m_children.begin();
    for (; it != m_children.end(); it++)
    {
      if ((*it)->IsInteractive())
      {
        m_isInteractive = true;
        m_interactiveAABB = (*it)->GetInteractiveAABB();
        it++;
        break;
      }
    }

    for (; it != m_children.end(); it++)
    {
      if ((*it)->IsInteractive())
        m_interactiveAABB += (*it)->GetInteractiveAABB();
    }
  }

  //------------------------------------------------------------------------------------
  //  UIButton
  //------------------------------------------------------------------------------------

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

  void UIButton::Activate()
  {
  }

  void UIButton::HoverOn()
  {
  }

  void UIButton::HoverOff()
  {
  }
}