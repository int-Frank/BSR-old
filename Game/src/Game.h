#ifndef GAME_H
#define GAME_H

#include "Types.h"
#include "MessageBus.h"
#include "LayerStack.h"
#include "Framework/Framework.h"

class Layer;
class IWindow;

class Game
{
public:

  Game();
  ~Game();

  static Game * Instance();

  static void Init();
  static void ShutDown();
  static bool IsInitialised();

  void Run();
  void RequestQuit();

private:

  void _Init();

  Game(Game const &) = delete;
  Game & operator=(Game const &) = delete;

  void InitWindow();

private:

  static Game * s_instance;

private:

  bool m_shouldQuit;
  IWindow * m_window;

  LayerStack  m_layerStack;
  MessageBus  m_msgBus;
};

#endif