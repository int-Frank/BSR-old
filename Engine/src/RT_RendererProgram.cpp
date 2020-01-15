//@group Renderer/RenderThread

/*
  Original Copyright Yan Chernikov <github.com/TheCherno/Hazel-dev> and contributors.

  The following code is a derivative work of the code from the GitHub project 'Hazel-dev',
  which is licensed under: 

                                  Apache License
                             Version 2.0, January 2004
                          http://www.apache.org/licenses

  This code therefore is also licensed under the terms
  of the Apache License Version 2.0

  Copyright 2017-2019 Frank Hart <frankhart010@gmail.com>
*/

#include <fstream>
#include <regex>

#include "glad/glad.h"

#include "RT_RendererProgram.h"
#include "core_ErrorCodes.h"
#include "core_Log.h"
#include "core_Assert.h"

//TODO Parse uniform blocks, shader storage blocks

namespace Engine
{
  //--------------------------------------------------------------------------------------------------
  // Parsing helper functions
  //--------------------------------------------------------------------------------------------------
  static char const * FindToken(char const * a_str, std::string const & a_token)
  {
    char const * t = a_str;
    while (t = strstr(t, a_token.c_str()))
    {
      bool left = a_str == t || isspace(t[-1]);
      bool right = !t[a_token.size()] || isspace(t[a_token.size()]);
      if (left && right)
        return t;

      t += a_token.size();
    }
    return nullptr;
  }

  static char const * FindToken(std::string const & a_string, std::string const & a_token)
  {
    return FindToken(a_string.c_str(), a_token);
  }

  static std::vector<std::string> SplitString(std::string const & a_string, std::string const & a_delimiters)
  {
    size_t start = 0;
    size_t end = a_string.find_first_of(a_delimiters);

    std::vector<std::string> result;

    while (end <= std::string::npos)
    {
      std::string token = a_string.substr(start, end - start);
      if (!token.empty())
        result.push_back(token);

      if (end == std::string::npos)
        break;

      start = end + 1;
      end = a_string.find_first_of(a_delimiters, start);
    }

    return result;
  }

  static std::vector<std::string> SplitString(std::string const & a_string, char const a_delimiter)
  {
    return SplitString(a_string, std::string(1, a_delimiter));
  }

  static std::vector<std::string> Tokenize(std::string const & a_string)
  {
    return SplitString(a_string, " \t\n");
  }

  static std::vector<std::string> GetLines(std::string const & a_string)
  {
    return SplitString(a_string, "\n");
  }

  static std::string GetBlock(char const * a_str, char const ** a_outPosition)
  {
    const char* end = strstr(a_str, "}");
    if (!end)
      return a_str;

    if (a_outPosition)
      *a_outPosition = end;
    uint32_t length = end - a_str + 1;
    return std::string(a_str, length);
  }

  static std::string GetStatement(char const * a_str, char const ** a_outPosition)
  {
    const char* end = strstr(a_str, ";");
    if (!end)
      return a_str;

    if (a_outPosition)
      *a_outPosition = end;
    uint32_t length = end - a_str + 1;
    return std::string(a_str, length);
  }

  static bool StartsWith(std::string const & a_string, const std::string& a_start)
  {
    return a_string.find(a_start) == 0;
  }

  //--------------------------------------------------------------------------------------------------
  // Helper functions
  //--------------------------------------------------------------------------------------------------

  static bool IsTypeStringResource(const std::string& type)
  {
    if (type == "sampler2D")		return true;
    if (type == "samplerCube")		return true;
    if (type == "sampler2DShadow")	return true;
    return false;
  }

  static uint32_t GetShaderOpenGLType(int a_index)
  {
    switch (a_index)
    {
      case static_cast<uint32_t>(ShaderDomain::Vertex):
        return GL_VERTEX_SHADER;
      case static_cast<uint32_t>(ShaderDomain::Fragment):
        return GL_FRAGMENT_SHADER;
      case static_cast<uint32_t>(ShaderDomain::Geometry):
        return GL_GEOMETRY_SHADER;
    }
    BSR_ASSERT(false, "Shader index not recognised!");
    return 0xFFFFFFFF;
  }

  static uint32_t GetShaderIndex(uint32_t a_shaderType)
  {
    switch (a_shaderType)
    {
      case GL_VERTEX_SHADER:
        return static_cast<uint32_t>(ShaderDomain::Vertex);
      case GL_FRAGMENT_SHADER:
        return static_cast<uint32_t>(ShaderDomain::Fragment);
      case GL_GEOMETRY_SHADER:
        return static_cast<uint32_t>(ShaderDomain::Geometry);
    }
    BSR_ASSERT(false, "Shader type not recognised!");
    return static_cast<uint32_t>(ShaderDomain::INVALID);
  }

  static uint32_t GetShaderIndex(std::string const& a_shaderType)
  {
    if (a_shaderType == "vertex")
      return static_cast<uint32_t>(ShaderDomain::Vertex);
    if (a_shaderType == "fragment")
      return static_cast<uint32_t>(ShaderDomain::Fragment);
    if (a_shaderType == "geometry")
      return static_cast<uint32_t>(ShaderDomain::Geometry);

    BSR_ASSERT(false, "Shader type not recognised!");
    return static_cast<uint32_t>(ShaderDomain::INVALID);
  }

  //--------------------------------------------------------------------------------------------------
  // RT_RendererProgram
  //--------------------------------------------------------------------------------------------------

  RT_RendererProgram::RT_RendererProgram()
    : m_rendererID(0)
    , m_uniformBuffers{nullptr, nullptr, nullptr}
    , m_loaded(false)
  {

  }

  RT_RendererProgram::~RT_RendererProgram()
  {
    
  }

  void RT_RendererProgram::Destroy()
  {
    if (m_loaded)
    {
      glDeleteProgram(m_rendererID);
      m_rendererID = 0;

      for (int i = 0; i < SD32(COUNT); i++)
      {
        delete m_uniformBuffers[i];
        m_uniformBuffers[i] = nullptr;

        m_shaderSource[i].clear();
      }

      for (auto ptr : m_resources)
        delete ptr;

      for (auto ptr : m_structs)
        delete ptr;

      m_resources.clear();
      m_structs.clear();

      m_loaded = false;
    }
  }

  void RT_RendererProgram::Bind() const
  {
    if (m_loaded)
      glUseProgram(m_rendererID);
  }

  void RT_RendererProgram::Unbind() const
  {
    glUseProgram(0);
  }

  bool RT_RendererProgram::InitFromString(std::string const& a_source)
  {
    return Load(a_source);
  }

  bool RT_RendererProgram::InitFromFilePath(std::string const& a_filepath)
  {
    std::string content;
    std::ifstream ifs(a_filepath);
    if (ifs)
    {
      content.assign((std::istreambuf_iterator<char>(ifs)),
                     (std::istreambuf_iterator<char>()));
    }
    else
    {
      LOG_WARN("Could not read shader file {0}", a_filepath.c_str());
      return false;
    }

    return InitFromString(content);
  }

  bool RT_RendererProgram::Load(std::string const& a_source)
  {
    Destroy();
    SplitSource(a_source);
    Parse();

    if (m_uniformBuffers[0])
      m_uniformBuffers[0]->Log();
    if (m_uniformBuffers[1])
      m_uniformBuffers[1]->Log();

    for (auto ptr : m_structs)
      ptr->Log();

    //if (!CompileAndUploadShader())
    //  return false;

    //ResolveUniforms();
    //ValidateUniforms();

    m_loaded = true;
    return m_loaded;
  }

  void RT_RendererProgram::SplitSource(std::string const & a_source)
  {
    const char* typeToken = "//@type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = a_source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
      size_t eol = a_source.find_first_of("\r\n", pos);
      BSR_ASSERT(eol != std::string::npos, "Syntax error");
      size_t begin = pos + typeTokenLength + 1;
      std::string type = a_source.substr(begin, eol - begin);
      uint32_t shaderIndex = GetShaderIndex(type);
      BSR_ASSERT(shaderIndex != static_cast<uint32_t>(ShaderDomain::INVALID), "Invalid shader type specified");

      size_t nextLinePos = a_source.find_first_not_of("\r\n", eol);
      pos = a_source.find(typeToken, nextLinePos);
      m_shaderSource[shaderIndex] = a_source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? a_source.size() - 1 : nextLinePos));
    }
  }

  void RT_RendererProgram::Parse()
  {
    for (int i = 0; i < SD32(COUNT); i++)
    {
      char const* token;
      char const* str = m_shaderSource[i].c_str();

      while (token = FindToken(str, "struct"))
        ParseStruct(GetBlock(token, &str), ShaderDomain(i));

      str = m_shaderSource[i].c_str();
      while (token = FindToken(str, "uniform"))
        ParseUniform(GetStatement(token, &str), ShaderDomain(i));
    }
  }

  void RT_RendererProgram::ParseStruct(std::string const & a_block, ShaderDomain a_domain)
  {
    std::vector<std::string> tokens = Tokenize(a_block);

    uint32_t index = 0;
    index++; // struct
    std::string name = tokens[index++];
    ShaderStruct* uniformStruct = new ShaderStruct(name);
    index++; // {
    while (index < tokens.size())
    {
      if (tokens[index] == "}")
        break;

      std::string type = tokens[index++];
      std::string name = tokens[index++];

      // Strip ; from name if present
      //TODO if the next character is a '{' we have a uniform block
      //     In fact we need to look for:
      //          layout (std140) uniform Block {
      if (const char* s = strstr(name.c_str(), ";"))
        name = std::string(name.c_str(), s - name.c_str());

      uint32_t count = 1;
      const char* namestr = name.c_str();
      if (const char* s = strstr(namestr, "["))
      {
        name = std::string(namestr, s - namestr);

        const char* end = strstr(namestr, "]");
        std::string c(s + 1, end - s);
        count = atoi(c.c_str());
      }

      ShaderUniformDeclaration* field = nullptr;
      ShaderDataType dataType = StringToShaderDataType(type);
      if (dataType == ShaderDataType::NONE) //might be a previously defined struct
      {
        ShaderStruct * pStruct = FindStruct(type);
        if (pStruct)
          field = new ShaderUniformDeclaration(a_domain, pStruct, name, count);
        else
        {
          LOG_WARN("Unrecognised field '{}' in struct '{}' while parsing glsl struct.", type.c_str(), name.c_str());
          continue;
        }
      }
      else
        field = new ShaderUniformDeclaration(a_domain, StringToShaderDataType(type), name, count);
      uniformStruct->AddField(field);
    }
    m_structs.push_back(uniformStruct);
  }

  void RT_RendererProgram::ParseUniform(std::string const & a_statement, ShaderDomain a_domain)
  {
    std::vector<std::string> tokens = Tokenize(a_statement);
    uint32_t index = 0;

    index++; // "uniform"
    std::string typeString = tokens[index++];
    std::string name = tokens[index++];
    // Strip ; from name if present
    if (const char* s = strstr(name.c_str(), ";"))
      name = std::string(name.c_str(), s - name.c_str());

    std::string n(name);
    int32_t count = 1;
    const char* namestr = n.c_str();
    if (const char* s = strstr(namestr, "["))
    {
      name = std::string(namestr, s - namestr);

      const char* end = strstr(namestr, "]");
      std::string c(s + 1, end - s);
      count = atoi(c.c_str());
    }

    if (IsTypeStringResource(typeString))
    {
      ShaderResourceDeclaration* declaration = new ShaderResourceDeclaration(StringToShaderResourceType(typeString), name, count);
      m_resources.push_back(declaration);
    }
    else
    {
      ShaderDataType t = StringToShaderDataType(typeString);
      ShaderUniformDeclaration* declaration = nullptr;

      if (t == ShaderDataType::NONE)
      {
        do 
        {
          ShaderStruct* pStruct = FindStruct(typeString);
          if (pStruct)
          {
            declaration = new ShaderUniformDeclaration(a_domain, pStruct, name, count);
            break;
          }

          //TODO uniform block...

          continue;
        } while (false);
      }
      else
        declaration = new ShaderUniformDeclaration(a_domain, t, name, count);

      //TODO remove this. We found a uniform block but not implemented yet
      if (!declaration)
        return;

      uint32_t ind = static_cast<uint32_t>(a_domain);

      if (m_uniformBuffers[ind] == nullptr)
        m_uniformBuffers[ind] = new ShaderUniformDeclarationBuffer("", a_domain);
      m_uniformBuffers[ind]->PushUniform(declaration);
    }
  }

  ShaderStruct * RT_RendererProgram::FindStruct(std::string const& a_name)
  {
    for (ShaderStruct * ptr : m_structs)
    {
      if (a_name == ptr->GetName())
        return ptr;
    }
    return nullptr;
  }

  bool RT_RendererProgram::CompileAndUploadShader()
  {
    bool result = true;
    Dg::DynamicArray<GLuint> shaderRendererIDs;

    GLuint program = glCreateProgram();
    for (int i = 0; i < SD32(COUNT); i++)
    {
      GLenum type = GetShaderOpenGLType(i);
      std::string const & source = m_shaderSource[i];

      if (source.empty())
        continue;

      GLuint shaderRendererID = glCreateShader(type);
      GLchar const * sourceCstr = (GLchar const *)source.c_str();
      glShaderSource(shaderRendererID, 1, &sourceCstr, 0);

      glCompileShader(shaderRendererID);

      GLint isCompiled = 0;
      glGetShaderiv(shaderRendererID, GL_COMPILE_STATUS, &isCompiled);
      if (isCompiled == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetShaderiv(shaderRendererID, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shaderRendererID, maxLength, &maxLength, &infoLog[0]);

        LOG_ERROR("Shader compilation failed:\n{0}", &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(shaderRendererID);

        BSR_ASSERT(false, "Failed");
        result = false;
      }

      shaderRendererIDs.push_back(shaderRendererID);
      glAttachShader(program, shaderRendererID);
    }

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
      GLint maxLength = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

      // The maxLength includes the NULL character
      std::vector<GLchar> infoLog(maxLength);
      glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
      LOG_ERROR("Shader compilation failed:\n{0}", &infoLog[0]);

      // We don't need the program anymore.
      glDeleteProgram(program);
      // Don't leak shaders either.
      for (auto id : shaderRendererIDs)
        glDeleteShader(id);
      result = false;
    }

    // Always detach shaders after a successful link.
    for (auto id : shaderRendererIDs)
      glDetachShader(program, id);

    m_rendererID = program;
    return result;
  }

  void RT_RendererProgram::ResolveUniforms()
  {
    /*glUseProgram(m_rendererID);

    for (int i = 0; i < SD_COUNT; i++)
    {
      ShaderUniformDeclarationBuffer * decl = m_uniformBuffers[i];
      if (decl)
      {
        ShaderUniformList const & uniforms = decl->GetUniformDeclarations();
        for (size_t j = 0; j < uniforms.size(); j++)
        {
          ShaderUniformDeclaration * uniform = uniforms[j];
          if (uniform->GetType() == ShaderDataType::STRUCT)
          {
            ShaderStruct const & s = uniform->GetShaderUniformStruct();
            Dg::DynamicArray<ShaderUniformDeclaration*> const & fields = s.GetFields();
            for (size_t k = 0; k < fields.size(); k++)
            {
              ShaderUniformDeclaration * field = fields[k];
              field->SetLocation(GetUniformLocation(uniform->GetName() + "." + field->GetName()));
            }
          }
          else
            uniform->SetLocation(GetUniformLocation(uniform->GetName()));
        }
      }
    }

    uint32_t sampler = 0;
    for (size_t i = 0; i < m_resources.size(); i++)
    {
      ShaderResourceDeclaration* resource = m_resources[i];
      int32_t location = GetUniformLocation(resource->GetName());

      if (resource->GetCount() == 1)
      {
        resource->SetRegister(sampler);
        if (location != -1)
          UploadUniformInt(location, sampler);

        sampler++;
      }
      else if (resource->GetCount() > 1)
      {
        resource->SetRegister(0);
        uint32_t count = resource->GetCount();
        int* samplers = new int[count];
        for (uint32_t s = 0; s < count; s++)
          samplers[s] = s;
        UploadUniformIntArray(resource->GetName(), samplers, count);
        delete[] samplers;
      }
    }*/
  }

  void RT_RendererProgram::ValidateUniforms()
  {

  }
}
