
#include <fstream>
#include <exception>

#include "Application.h"
#include "Options.h"

//int main( int argc, char* args[] )
//{
//  //Clear crash report file
//  std::ofstream ofs;
//  ofs.open(CRASH_REPORT_FILE, std::ofstream::out | std::ofstream::trunc);
//  ofs.close();
//
//  try
//  {
//    Game::Init();
//  }
//  catch (std::exception & e)
//  {
//    std::ofstream ofs(CRASH_REPORT_FILE);
//    ofs << "Game failed to initialise: " << e.what();
//  }
//  catch (...)
//  {
//    std::ofstream ofs(CRASH_REPORT_FILE);
//    ofs << "Game failed to intialise, no error reported.";
//  }
//
//  if (Game::IsInitialised())
//  {
//    try
//    {
//      Game::Instance()->Run();
//    }
//    catch (std::exception & e)
//    {
//      std::ofstream ofs(CRASH_REPORT_FILE);
//      ofs << "Game has thrown exception: " << e.what();
//    }
//    catch (...)
//    {
//      std::ofstream ofs(CRASH_REPORT_FILE);
//      ofs << "Game has thrown an unknown exception";
//    }
//
//    Game::ShutDown();
//  }
//
//  return 0;
//}