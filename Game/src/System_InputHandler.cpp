
#include "System_InputHandler.h"
#include "Framework/Framework.h"

#include "IEventPoller.h"
#include "IMouseController.h"

System_InputHandler::System_InputHandler(MessageBus * a_pMsgBus)
  : System(a_pMsgBus)
  , m_eventPoller(Framework::Instance()->GetEventPoller())
  , m_mouseController(Framework::Instance()->GetMouseController())
{

}

System_InputHandler::~System_InputHandler()
{

}

template<>
void System_InputHandler::SetProfile<System_InputHandler::BP_None>()
{

}

template<>
void System_InputHandler::SetProfile<System_InputHandler::BP_Loading>()
{

}

template<>
void System_InputHandler::SetProfile<System_InputHandler::BP_Menu>()
{

}

template<>
void System_InputHandler::SetProfile<System_InputHandler::BP_DebugOverlay>()
{

}

template<>
void System_InputHandler::SetProfile<System_InputHandler::BP_Game>()
{

}

template<>
void System_InputHandler::SetProfile<System_InputHandler::BP_Elevator>()
{

}