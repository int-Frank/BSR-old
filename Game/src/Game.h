#ifndef GAME_H
#define GAME_H

#include "Types.h"

class Game
{
public:

  Game();
  ~Game();

  static void Init();
  static Game * Instance();
  static void ShutDown();
  static bool IsInitialised();

  void Run();

private:
  Game(Game const &) = delete;
  Game & operator=(Game const &) = delete;

private:

  static Game * s_instance;
};

#endif