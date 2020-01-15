//@group Renderer

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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

namespace Engine
{
  enum class TextureFormat
  {
    None = 0,
    RGB = 1,
    RGBA = 2,
  };

  enum class TextureWrap
  {
    None = 0,
    Clamp = 1,
    Repeat = 2
  };

  enum class ResizeMethod
  {
    HQx,
    BRz
  };

  class OpenGLTexture2D
  {
  public:
    OpenGLTexture2D(TextureFormat format, unsigned int width, unsigned int height, TextureWrap wrap);
    OpenGLTexture2D(void *); //Load from data file
    virtual ~OpenGLTexture2D();

    virtual void Bind(uint32_t slot = 0) const;

    TextureFormat GetFormat() const
    {
      return m_Format;
    }
    uint32_t GetWidth() const
    {
      return m_Width;
    }
    uint32_t GetHeight() const
    {
      return m_Height;
    }

    void Lock();
    void Unlock();

    void Resize(uint32_t width, uint32_t height);
    //Buffer GetWriteableBuffer();

    virtual RendererID GetRendererID() const override
    {
      return m_RendererID;
    }
  private:
    RendererID m_RendererID;
    TextureFormat m_Format;
    TextureWrap m_Wrap = TextureWrap::Clamp;
    uint32_t m_Width, m_Height;

    Buffer m_ImageData;

    bool m_Locked = false;
  };
}

#endif