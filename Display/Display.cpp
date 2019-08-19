///////////////////////////////////////////////////////////////////////////
// Display.cpp   : defines webpage display using browser functions       //
// ver 1.0                                                               //
//                                                                       // 
// Application   : Project 3 OOD Spring 2019                             //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Abinav Murugadass, Syracuse University                //
// Source        : Ammar Salman 313/788-4694, assalman@syr.edu           //
///////////////////////////////////////////////////////////////////////////

#include "Display.h"
#include "../Process/Process.h"
#include "../Logger/Logger.h"
#include "../FileSystem/FileSystem.h"

using namespace Logger_code_publisher;

// -----< default ctor >--------------------------------------------------
Display::Display() { }

// -----< display single file >-------------------------------------------
void Display::display(const std::string & file)
{
  Demo::write("\n  Displaying file: " + FileSystem::Path::getName(file) + " in browser");
  Dbug::write("\n  Displaying file: " + FileSystem::Path::getName(file) + " in browser");

  CBP callback = []() {
    Demo::write("\n  --- child browser exited with this message ---");
    Dbug::write("\n  --- child browser exited with this message ---");
  };

  Process p;
  p.application("C:\\Windows\\System32\\cmd.exe");
  std::string cmd = "/C " + FileSystem::Path::getFullFileSpec(file);

  p.commandLine(cmd);
  p.create();
  p.setCallBackProcessing(callback);
  p.registerCallback();
  WaitForSingleObject(p.getProcessHandle(), INFINITE);
}

// -----< display multiple files  >---------------------------------------
void Display::display(const std::vector<std::string>& files)
{
  for (auto file : files) 
    display(file);
}

#ifdef TEST_DISPLAY

#include <iostream>

int main() {
  LoggerDebug::attach(&std::cout);
  LoggerDebug::start();

  std::vector<std::string> files;
  files.push_back("..\\ConvertedWebpages\\Converter.h.html");
  files.push_back("..\\ConvertedWebpages\\Converter.cpp.html");

  Display d;
  d.display(files);

  LoggerDebug::stop();
}

#endif