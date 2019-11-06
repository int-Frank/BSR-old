#include "glad/glad.h"

#include "Framework.h"
#include "../IShader.h"
#include "core_ErrorCodes.h"
#include "core_Log.h"
#include "core_Assert.h"

namespace Engine
{
  class OpenGLShader : public IShader
  {
  public:

    OpenGLShader();
    ~OpenGLShader();

    void Bind() const;
    void Unbind() const;

    Core::ErrorCode Init(std::string const & vs, std::string const & fs);

  private:
    GLuint m_program;
  };

  std::shared_ptr<IShader> IShader::Create(std::string const & a_vs, std::string const & a_fs)
  {
    OpenGLShader * temp = new OpenGLShader();
    BSR_ASSERT(temp->Init(a_vs, a_fs) == Core::EC_None);
    return std::shared_ptr<IShader>(temp);
  }

  OpenGLShader::OpenGLShader()
    : m_program(0)
  {
  }

  OpenGLShader::~OpenGLShader()
  {
    glDeleteProgram(m_program);
  }

  void OpenGLShader::Bind() const
  {
    glUseProgram(m_program);
  }

  void OpenGLShader::Unbind() const
  {
    glUseProgram(0);
  }

  Core::ErrorCode OpenGLShader::Init(std::string const & a_vs, std::string const & a_fs)
  {
    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar *source = (const GLchar *)a_vs.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

      // We don't need the shader anymore.
      glDeleteShader(vertexShader);

      LOG_ERROR("{0}", infoLog.data());

      return Core::EC_Error;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = (const GLchar *)a_fs.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

      // We don't need the shader anymore.
      glDeleteShader(fragmentShader);
      // Either of them. Don't leak shaders.
      glDeleteShader(vertexShader);

      LOG_ERROR("{0}", infoLog.data());

      return Core::EC_Error;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    m_program = glCreateProgram();

    // Attach our shaders to our program
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);

    // Link our program
    glLinkProgram(m_program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(m_program, maxLength, &maxLength, &infoLog[0]);

      // We don't need the program anymore.
      glDeleteProgram(m_program);
      m_program = 0;
      // Don't leak shaders either.
      glDeleteShader(vertexShader);
      glDeleteShader(fragmentShader);

      LOG_ERROR("{0}", infoLog.data());

      // In this simple program, we'll just leave
      return Core::EC_Error;
    }

    // Always detach shaders after a successful link.
    glDetachShader(m_program, vertexShader);
    glDetachShader(m_program, fragmentShader);

    return Core::EC_None;
  }

}
