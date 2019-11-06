#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <stdint.h>

class ByteBuffer
{
public:

  typedef uint8_t byte;

public:

  ByteBuffer();
  ByteBuffer();
  ~ByteBuffer();

  ByteBuffer(ByteBuffer const &);
  ByteBuffer & operator=(ByteBuffer const &);

  ByteBuffer(ByteBuffer &&);
  ByteBuffer & operator=(ByteBuffer &&);

private:

};

#endif