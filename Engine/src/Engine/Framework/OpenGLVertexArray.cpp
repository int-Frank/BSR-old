
#include "glad/glad.h"

#include "../Renderer.h"
#include "../IVertexArray.h"
#include "core_Assert.h"

namespace Engine
{
  class OpenGLVertexArray : public IVertexArray
  {
  public:

    OpenGLVertexArray();
    ~OpenGLVertexArray() override;

    void Bind() const override;
    void Unbind() const override;

    void AddVertexBuffer(const std::shared_ptr<IVertexBuffer>& vertexBuffer) override;
    void SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer) override;

    const std::vector<std::shared_ptr<IVertexBuffer>>& GetVertexBuffers() const override;
    const std::shared_ptr<IIndexBuffer>& GetIndexBuffer() const override;

  private:
    RendererID m_rendererID = 0;
    uint32_t m_vertexBufferIndex = 0;
    std::vector<std::shared_ptr<IVertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IIndexBuffer> m_indexBuffer;
  };

  std::shared_ptr<IVertexArray> IVertexArray::Create()
  {
    return std::make_shared<OpenGLVertexArray>();
  }

  static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
  {
    switch (type)
    {
      case ShaderDataType::Float:    return GL_FLOAT;
      case ShaderDataType::Float2:   return GL_FLOAT;
      case ShaderDataType::Float3:   return GL_FLOAT;
      case ShaderDataType::Float4:   return GL_FLOAT;
      case ShaderDataType::Mat3:     return GL_FLOAT;
      case ShaderDataType::Mat4:     return GL_FLOAT;
      case ShaderDataType::Int:      return GL_INT;
      case ShaderDataType::Int2:     return GL_INT;
      case ShaderDataType::Int3:     return GL_INT;
      case ShaderDataType::Int4:     return GL_INT;
      case ShaderDataType::Bool:     return GL_BOOL;
    }

    BSR_ASSERT(false, "Unknown ShaderDataType!");
    return 0;
  }

  OpenGLVertexArray::OpenGLVertexArray()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::VertexArrayCreate);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]() mutable
      {
        glCreateVertexArrays(1, &rendererID);
      });
  }

  OpenGLVertexArray::~OpenGLVertexArray()
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::VertexArrayDelete);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]() mutable
      {
        glDeleteVertexArrays(1, &rendererID);
      });
  }

  void OpenGLVertexArray::Bind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::VertexArrayBind);

    RENDER_SUBMIT(state, [rendererID = m_rendererID]() mutable
      {
        glBindVertexArray(rendererID);
      });
  }

  void OpenGLVertexArray::Unbind() const
  {
    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::VertexArrayUnBind);

    RENDER_SUBMIT(state, []()
      {
        glBindVertexArray(0);
      });
  }

  void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<IVertexBuffer> const & vertexBuffer)
  {
    BSR_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

    Renderer::Instance()->BeginNewGroup();

    Bind();
    vertexBuffer->Bind();

    RenderState state = RenderState::Create();
    state.Set<RenderState::Attr::Type>(RenderState::Type::Command);
    state.Set<RenderState::Attr::Command>(RenderState::Command::VertexArrayAddVertexBuffer);

    BufferLayout const * pLayout = new BufferLayout(vertexBuffer->GetLayout());

    RENDER_SUBMIT(state, [layout = pLayout, vertexBufferIndex = m_vertexBufferIndex]() mutable
      {
        for (auto it = layout->begin(); it != layout->end(); it++)
        {
          auto glBaseType = ShaderDataTypeToOpenGLBaseType(it->type);
          glEnableVertexAttribArray(vertexBufferIndex);
          if (glBaseType == GL_INT)
          {
            glVertexAttribIPointer(vertexBufferIndex,
              it->GetComponentCount(),
              glBaseType,
              layout->GetStride(),
              (const void*)(intptr_t)it->offset);
          }
          else
          {
            glVertexAttribPointer(vertexBufferIndex,
              it->GetComponentCount(),
              glBaseType,
              it->normalized ? GL_TRUE : GL_FALSE,
              layout->GetStride(),
              (const void*)(intptr_t)it->offset);
          }
          vertexBufferIndex++;
        }
        delete layout;
      });

    Renderer::Instance()->EndCurrentGroup();
    /*
      Here is one of those cases where we just have to trust the Renderer will perform this
      RENDER_SUBMIT. Ideally we would have the Render call back to this VertexArray to 
      increment m_vertexBufferIndex, but that seems messy.
    */
    m_vertexBufferIndex += (uint32_t)vertexBuffer->GetLayout().GetElements().size();
    m_vertexBuffers.push_back(vertexBuffer);

  }

  void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IIndexBuffer>& indexBuffer)
  {
    Bind();
    indexBuffer->Bind();

    m_indexBuffer = indexBuffer;
  }
}