#include "Message.h"

Message::Message()
  : type(0)
{

}

void Message::SetType(uint32_t a_class, uint32_t a_type)
{
  type = ((a_class << 16) | a_type);
}

uint32_t Message::GetClass() const
{
  return (type >> 16);
}

uint32_t Message::GetType() const
{
  return (type & 0xFFFF);
}

bool Message::IsOfClass(MessageClass a_class) const
{
  return (GetClass() & a_class) != 0;
}