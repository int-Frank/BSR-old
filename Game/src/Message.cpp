#include "Message.h"

uint32_t CombineMessageParts(uint32_t a_class, uint32_t a_type)
{
  return ((a_class << 16) | a_type);
}

uint32_t GetMessageClass(uint32_t a_class)
{
  return (a_class >> 16);
}

uint32_t GetMessageType(uint32_t a_type)
{
  return (a_type & 0xFFFF);
}