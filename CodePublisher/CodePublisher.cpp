///////////////////////////////////////////////////////////////////////////
// CodePublisher.cpp : Excecutes the Commands sent buy the GUI and       //
//					   sends the results to the GUI which publishes it	 //
// Version       1.1													 //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Abinav Murugadass, Syracuse University ,              //
//				   MS in Computer engineering, amurugad@syr.edu          //
// Source		 : Ammar Salman , Syracuse University 		             //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations :
*=======================
* This is the implementation of the CodePublisher, which uses the converter and Loader to convert 
* Source code into wenb pages and Suppots Displaying them as Web Pages.
* Version Notes(1.1)
*============================
* The IExecutive Class is inherited here and it's functions are defined here, 
* As a private member the Executive owns an instance of the code publisher
* When an instance of the executive is created and called, an instance of the code publisher is created 
* and is called by the executive to execute the functions of the interface
*
* 
*
*/

#include "ICodePublisher.h"
using namespace UtilitiesCodePublisher;
using namespace Logger_code_publisher;
ProcessCmdLine::Usage customUsage();
using Dbug = Logger_code_publisher::StaticLogger<2>;



//----------------<Code Publisher constructor>-----------------------------------
CodePublisher::CodePublisher() {
	std::cout << "\n\t -> Code Publisher is Created Here \n";
}
//---------------< Inherited instance of the IExecutive class >------------------------


// -----< process cmd to get info >------------------------------------
bool CodePublisher::processCommandLineArgs(int argc, char ** argv)
{
  pcl_ = new ProcessCmdLine(argc, argv); 
  pcl_->usage(customUsage());

  preface("Command Line: ");
  pcl_->showCmdLine();

  if (pcl_->parseError())
  {
    pcl_->usage();
    std::cout << "\n\n";
    return false;
  }
  dirIn_ = pcl_->path();
  return true;
}
// -----< return input directory - from PCL >----------------------------
const std::string & CodePublisher::workingDirectory() const
{
  return dirIn_;
}

// -----< set output directory >-----------------------------------------
void CodePublisher::outputDirectory(const std::string & dir)
{
  dirOut_ = dir; 
 // cconv_.outputDir(dir);
}

// -----< return output directory >--------------------------------------
const std::string & CodePublisher::outputDirectory() const
{
  return dirOut_;
}

// -----< extract files - after processing cmd >-------------------------
bool CodePublisher::extractFiles()
{
  DirExplorerN de(pcl_->path());

  for (auto patt : pcl_->patterns())
  {
    de.addPattern(patt);
  }

  if (pcl_->hasOption("s"))
  {
    de.recurse();
  }

  bool res = de.search() && de.match_regexes(pcl_->regexes());
  de.showStats();

  files_ = de.filesList();

  return res;
}


// -----< publish - files extracted from directory explorer Edited for project 3 to enalble only converting files >---------------
void CodePublisher::publish()
{
	files_=cconv_.convert(files_);
}

// -----< publish - single file >-------------------------------------------
void CodePublisher::publish(const std::string & file)
{
  display_.display(cconv_.convert(file));
}

// -----< publish - must provide list >-------------------------------------
void CodePublisher::publish(const std::vector<std::string>& files)
{
  display_.display(cconv_.convert(files));
}

// -----< gets display mode as set by PCL >---------------------------------
UtilitiesCodePublisher::DisplayMode CodePublisher::displayMode() const
{
  return pcl_->displayMode();
}


// -----< command line usage >----------------------------------------------
ProcessCmdLine::Usage customUsage()
{
  std::string usage;
  usage += "\n  Command Line: path [/option]* [/pattern]* [/regex]*";
  usage += "\n    path is relative or absolute path where processing begins";
  usage += "\n    [/option]* are one or more options of the form:";
  usage += "\n      /s     - walk directory recursively";
  usage += "\n      /demo  - run in demonstration mode (cannot coexist with /debug)";
  usage += "\n      /debug - run in debug mode (cannot coexist with /demo)";
  
  usage += "\n    [pattern]* are one or more pattern strings of the form:";
  usage += "\n      *.h *.cpp *.cs *.txt or *.*";
  usage += "\n    [regex] regular expression(s), i.e. [A-B](.*)";
  usage += "\n";
  return usage;
}

// -----< initialize loggers in proper display mode >------------------------
void initializeLoggers(DisplayMode dm) {
  if (dm == DisplayMode::Debug) {
    Demo::attach(&std::cout);
    Demo::start();
  }

  else if (dm == DisplayMode::Demo) {
    Dbug::attach(&std::cout);
    Dbug::start();
  }
}

// -----< stop loggers >-----------------------------------------------------
void stopLoggers() {
  Dbug::stop();
  Demo::stop();
}
//-----------------<The code to be used by the interface to do analysis> --------------

void CodePublisher::doAnalysis(int argc, char** argv) {
	
	int err=0;
	if (!processCommandLineArgs(argc, argv)) { err = 1; }
	if (!extractFiles()) { // if this fails, then there are no files to process at all
		err = 2; // the demo will end after this
	}
	if (err == 1) {
		std::cout << "\n  Invalid command line args.\n  Ending demonstration...\n" << std::endl;
		return ;
	}
	else if (err == 2) {
		std::cout << "\n  The given path has no files matching patterns and/or regexes." << std::endl;
		std::cout << "\n  Ending demonstration...\n" << std::endl;
		return ;
	}
	publish();
	dirOut_ = cconv_.outputDir();
}

//--------------------< Object factory is implemented here>--------------------



#ifdef DEMO_CP

#include <iostream>





// -----< demonstrate requirements  5 >---------------------------------------
int demoReq45(CodePublisher& cp, int argc, char ** argv) {
  
  if (argc < 2) return 1; // even before processing, if no path, just reject it

  if (!cp.processCommandLineArgs(argc, argv)) return 1; // just in case
  if (!cp.extractFiles()) { // if this fails, then there are no files to process at all
    return 2; // the demo will end after this
  }
  return 0;
}

// -----< demonstrate requirements 6 and 7 >------------------------------
void demoReq67(CodePublisher& cp) {
  cp.publish();
}


// -----< demonstrate requirement 8 >-------------------------------------

// -----< DEMONSTRATION MAIN >---------------------------------------------
int main(int argc, char ** argv) {
	
  CodePublisher cp;

  int err = demoReq45(cp, argc, argv);
  if (err == 1) {
	  std::cout << "\n  Invalid command line args.\n  Ending demonstration...\n" << std::endl;
    return 1;
  }
  else if (err == 2) {
   std::cout<<"\n  The given path has no files matching patterns and/or regexes."<<std::endl;
    std::cout<<"\n  Ending demonstration...\n"<<std::endl;
    return 2;
  }
  
  demoReq67(cp);
  getchar();
  getchar();
  stopLoggers();
  
  return 0;
}

#endif

#ifdef USE_CP

#include <iostream>

// -----< NORMAL USE MAIN > ------------------------------------------
int main(int argc, char ** argv) {
  CodePublisher cp;
  if (!cp.processCommandLineArgs(argc, argv)) {
    std::cout << "\n  Failed to process command line arguments. Terminating\n\n";
    return 1;
  }

  initializeLoggers(cp.displayMode());

  if (!cp.extractFiles()) {
    stopLoggers();
    std::cout << "\n  Found no files to process. Terminating.\n\n";
    return 0; 
  }

  cp.publish();
  */
  stopLoggers();
  return 0;
}

#endif