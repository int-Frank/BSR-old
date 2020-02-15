
#include <chrono>

#include "Engine.h"
#include "Layer_InputHandler.h"
#include "Renderer.h"
#include "RT_RendererAPI.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "RendererProgram.h"

#include "Canvas.h"
#include "EngineMessages.h"

class GameLayer : public Engine::Layer
{
public:

  ASSIGN_ID(256);
  GameLayer()
  {

  }

  void OnAttach() override
  {
    Engine::Renderer::SetClearColor(1.0f, 0.0f, 1.0f);

    float verts[] = 
    {
      -0.5f, 0.5f,
      0.5f, 0.5f,
      0.5f, 0.0f,
      -0.5f, 0.0f
    };

    int indices[] = {0, 1, 2, 0, 2, 3};

    m_vb = Engine::VertexBuffer::Create(verts, SIZEOF32(verts));
    m_vb->SetLayout(
      {
        { Engine::ShaderDataType::VEC2, "a_Position" }
      });

    m_ib = Engine::IndexBuffer::Create(indices, SIZEOF32(indices));
    m_va = Engine::VertexArray::Create();

    m_va->AddVertexBuffer(m_vb);
    m_va->SetIndexBuffer(m_ib);

    m_prog = Engine::RendererProgram::Create(
      {
        { Engine::ShaderDomain::Vertex, Engine::StrType::Path, "D:/dev/projects/BSR/Game/src/vs.glsl" },
        { Engine::ShaderDomain::Fragment, Engine::StrType::Path, "D:/dev/projects/BSR/Game/src/fs.glsl" }
        //{ Engine::ShaderDomain::Fragment, Engine::StrType::Path, "D:/dev/projects/BSR/Game/src/test_shader.glsl" }
      });

    int t = 1;
    int f = 0;

    int vals[3] = {1, 0, 1};

    m_prog->UploadUniform("u_myStruct.myBool", vals, 4 * 3);

    Engine::UIGroup* pg0 = new Engine::UIGroup("g0", vec3(0.25f, 0.25f, 0.0f), vec3(0.5f, 0.5f, 0.0f));
    Engine::UIButton* btn0 = new Engine::UIButton("btn0", vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.5f, 0.0f));
    //btn0->Bind(Engine::UIWidget::Action::HoverOn);
    pg0->Add(btn0);
    m_canvas.Add(pg0);

  }

  void HandleMessage(Engine::Message* a_pMsg) override
  {
    DISPATCH_MESSAGE(Engine::Message_GUI_MouseMove);
    DISPATCH_MESSAGE(Engine::Message_GUI_MouseButtonDown);
  }

  void HandleMessage(Engine::Message_GUI_MouseMove* a_pMsg)
  {
    float x, y;
    if (Engine::Application::Instance()->NormalizeWindowCoords(a_pMsg->x, a_pMsg->y, x, y))
    {
      m_canvas.HandleNewCursonPostion(x, y);
    }
  }

  void HandleMessage(Engine::Message_GUI_MouseButtonDown* a_pMsg)
  {
    if (a_pMsg->button != Engine::InputCode::IC_MOUSE_BUTTON_LEFT)
      return;

    float x, y;
    if (Engine::Application::Instance()->NormalizeWindowCoords(a_pMsg->x, a_pMsg->y, x, y))
    {
      m_canvas.HandleNewCursonPostion(x, y);
      m_canvas.Activate();
    }
  }

  void OnDetach() override
  {

  }

  void Update(float a_dt) override
  {
    //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    //LOG_DEBUG("ASDF");
  }

  void Render() override
  {
    Engine::Renderer::Clear(1.0f, 0.0f, 1.0f);

    m_prog->Bind();
    m_va->Bind();

    Engine::Renderer::DrawIndexed(6, false);

  }

private:

  Engine::Ref<Engine::VertexBuffer>     m_vb;
  Engine::Ref<Engine::IndexBuffer>      m_ib;
  Engine::Ref<Engine::VertexArray>      m_va;
  Engine::Ref<Engine::RendererProgram>  m_prog;
  Engine::UICanvas                      m_canvas;
};

class Game : public Engine::Application
{
public:
  Game(Opts const & a_opts)
    : Application(a_opts)
  {
    PushLayer(new GameLayer());

    Engine::Layer_InputHandler * layer = static_cast<Engine::Layer_InputHandler*>(GetLayer(Engine::Layer_InputHandler::GetStaticID()));
    if (!layer)
      LOG_ERROR("Couldn't find input layer!");
    else
    {
      layer->SetBindings(
        {
          {Engine::IC_MOUSE_BUTTON_LEFT,  Engine::IE_BUTTON_UP,   new Engine::Message_GUI_MouseButtonUp()},
          {Engine::IC_MOUSE_BUTTON_LEFT,  Engine::IE_BUTTON_DOWN, new Engine::Message_GUI_MouseButtonDown()},
          {Engine::IC_MOUSE_BUTTON_RIGHT, Engine::IE_BUTTON_UP,   new Engine::Message_GUI_MouseButtonUp()},
          {Engine::IC_MOUSE_BUTTON_RIGHT, Engine::IE_BUTTON_DOWN, new Engine::Message_GUI_MouseButtonDown()},
          {Engine::IC_MOUSE_MOTION,                               new Engine::Message_GUI_MouseMove()},
          {Engine::IC_MOUSE_WHEEL_UP,                             new Engine::Message_GUI_MouseWheelUp()},
          {Engine::IC_MOUSE_WHEEL_DOWN,                           new Engine::Message_GUI_MouseWheelDown()},
          {Engine::IC_TEXT,                                       new Engine::Message_GUI_Text()}
        });
    }

    LOG_TRACE("Game initialised!");
    //RequestQuit();
  }

  ~Game()
  {

  }

};

Engine::Application* Engine::CreateApplication()
{
  Application::Opts opts;
  return new Game(opts);
}
