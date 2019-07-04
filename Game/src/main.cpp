
#include <fstream>
#include <exception>

#include "Game.h"

int main( int argc, char* args[] )
{
  try
  {
    Game::Init();
  }
  catch (std::exception & e)
  {
    std::ofstream ofs("crash-report.txt");
    ofs << "Game failed to initialise: " << e.what();
  }
  catch (...)
  {
    std::ofstream ofs("crash-report.txt");
    ofs << "Game failed to intialise, no error reported.";
  }

  if (Game::IsInitialised())
  {
    try
    {
      Game::Instance()->Run();
    }
    catch (std::exception & e)
    {
      std::ofstream ofs("crash-report.txt");
      ofs << "Game has thrown exception: " << e.what();
    }
    catch (...)
    {
      std::ofstream ofs("crash-report.txt");
      ofs << "Game has thrown an unknown exception";
    }

    Game::ShutDown();
  }
}