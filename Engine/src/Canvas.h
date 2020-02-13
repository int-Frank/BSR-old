//@group UI

#ifndef CANVAS_H
#define CANVAS_H

//UI specific
#include <string>
#include "DgR2AABB.h"
#include "DgDoublyLinkedList.h"
#include "Message.h"

namespace Engine
{
  class UIWidget;

  typedef Dg::R2::AABB<float> AABB;

  class UICanvas
  {
  public:

    UICanvas();
    ~UICanvas();

    void Add(UIWidget*);

    void Clear();
    void HandleNewCursonPostion(float x, float y);
    void Activate();

  private:

    Dg::DoublyLinkedList<UIWidget*> m_widgets;
    UIWidget* m_pFocus;
  };

  class UIWidget
  {
  public:

    enum class Action
    {
      HoverOn,
      HoverOff,
      Activate,
    };

    static int const ACTION_COUNT = 3;

    UIWidget(std::string const & name, 
      vec3 trans, vec3 scale,
      bool isInteractive, int depth = 0
    );
    virtual ~UIWidget();

    bool IsInside(float x, float y);

    //return: handled
    virtual bool HandleNewCursonPostion(float x, float y);
    virtual void Activate() {} //Enter
    virtual void HoverOn() {}
    virtual void HoverOff() {}

    //Takes ownership of message
    void Bind(Action, Message *);

    void SetFocus(bool);
    bool HasFocus() const;

    virtual void Render() {}

    void Translate(float xMin, float yMin);
    void Scale(float sx, float sy);

    void SetTranslation(float xMin, float yMin);
    void SetScale(float sx, float sy);
    void SetDepth(int);
    int GetDepth() const;

    virtual void DepthHasChanged(UIWidget * child) {}

    void SetGlobalToLocal();
    mat3 GetGlobalToLocal() const;
    mat3 GetLocalToParent() const;
    AABB const & GetInteractiveAABB() const;
    bool IsInteractive() const;
    void SetIsInteractive(bool);
    void SetParent(UIWidget *);

    virtual void NewTransform() {}

  protected:
    std::string m_name;
    Message * m_bindings[3];
    bool m_isInteractive;
    AABB m_interactiveAABB;
    UIWidget* m_pParent;
    vec3 m_translation;
    vec3 m_scale;
    mat3 m_T_Global_to_Local;
    bool m_hasFocus;
    int m_depth;
  };

  class UIGroup : public UIWidget
  {
  public:

    UIGroup(std::string const& name, vec3 trans, vec3 scale);
    ~UIGroup();

    bool HandleNewCursonPostion(float x, float y) override;
    void Activate() override;

    virtual void Render();

    void Clear();
    void Add(UIWidget*);
    bool Remove(UIWidget*);

    void DepthHasChanged(UIWidget*);

  private:

    void SetInteractiveAABB();
    void NewTransform();

  private:
    Dg::DoublyLinkedList<UIWidget*> m_children;
    UIWidget * m_pFocus;
  };

  class UIButton : public UIWidget
  {
  public:

    UIButton(std::string const& name, vec3 trans, vec3 scale);
    ~UIButton();

    bool HandleNewCursonPostion(float x, float y) override;
    void Activate() override;
    void HoverOn() override;
    void HoverOff() override;

    void SetText(std::string const& text);
    void SetTextSize(int);
    void SetFont(uint32_t id);
    void SetColor(RGBA normal, RGBA hover);
    void SetTextColor(RGBA normal, RGBA hover);

  };

  class UISlider : public UIWidget
  {
  public:

  private:
  };

  class UICheckBox : public UIWidget
  {
  public:

  private:
  };

  class UIText : public UIWidget
  {
  public:

  private:
  };

  class UITextInput : public UIWidget
  {
  public:

  private:
  };
}

#endif