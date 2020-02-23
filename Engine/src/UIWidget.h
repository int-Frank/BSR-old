//@group UI

#ifndef UIWIDGET_H
#define UIWIDGET_H

#include "core_utils.h"
#include <string>

namespace Engine
{
  class Message;

  class UIWidget
  {
  public:

    enum class Action
    {
      HoverOn,
      HoverOff,
      Activate
    };

    static int const ACTION_COUNT = 3;

    UIWidget(std::string const& name,
      vec3 trans, vec3 scale,
      bool isInteractive, int depth = 0
    );
    virtual ~UIWidget();

    virtual bool IsInside(float x, float y);

    //return: handled
    virtual bool HandleNewCursonPostion(float x, float y);
    virtual void DoAction(Action)
    {
    }

    //Takes ownership of message
    void Bind(Action, Message*);

    void SetFocus(bool);
    bool HasFocus() const;

    virtual void Render()
    {
    }

    void Translate(float xMin, float yMin);
    void Scale(float sx, float sy);

    void SetTranslation(float xMin, float yMin);
    void SetScale(float sx, float sy);
    void SetDepth(int);

    vec3 GetTranslation() const;
    vec3 GetScale() const;
    int GetDepth() const;

    virtual void DepthHasChanged(UIWidget* child)
    {
    }

    void SetGlobalToLocal();
    mat3 GetGlobalToLocal() const;
    mat3 GetLocalToParent() const;
    bool IsInteractive() const;
    void SetIsInteractive(bool);
    void SetParent(UIWidget*);

    virtual void NewTransform()
    {
    }

  protected:
    std::string m_name;
    Message* m_bindings[3];
    bool m_isInteractive;
    UIWidget* m_pParent;
    vec3 m_translation;
    vec3 m_scale;
    mat3 m_T_Global_to_Local;
    bool m_hasFocus;
    int m_depth;
  };


}

#endif