#pragma once

///////////////////////////////////////////////////////////////////////////
// ICodePublisher.h : defines facade/executive for OOD Pr3 S19           //
// ver 1.0                                                               //
//                                                                       // 
// Application   : OOD-S19 Project 3									 //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Abinav Murugadass, Syracuse University                //
// Source		 : Ammar Salman  313/788-4694, assalman@syr.edu          //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This Package defines the  Code Publisher class. 
*
*  Required Files:
* =======================
*  ICodePublisher.h CodePublisher.cpp CodeUtilities.h
*  Converter.h Converter.cpp DirExplorerN.h DirExplorerN.cpp
*  Display.h Display.cpp Logger.h Logger.cpp 
*
*  Maintainence History:
* =======================
*  ver 1.0 - 14 April 2019
*  - first release
*/

#include <string>
#include <vector>
#include "../Converter/Converter.h"
#include "../Display/Display.h"
#include "../DirExplorer-Naive/DirExplorerN.h"
#include "../UtilitiesCodePublisher/CodeUtilities/CodeUtilities.h"
#include "../Logger/Logger.h"


using namespace UtilitiesCodePublisher;
//using namespace Logger_code_publisher;
using namespace FileSystem;

class CodePublisher
{
public:
	CodePublisher();
	std::vector<std::string> getfiles() { return files_; }
	bool processCommandLineArgs(int argc, char ** argv);

	void workingDirectory(const std::string& dir);
	const std::string& workingDirectory() const;

	void outputDirectory(const std::string& dir);
	const std::string& outputDirectory() const;

	bool extractFiles();

	void publish();
	void publish(const std::string& file);
	void publish(const std::vector<std::string>& files);
	//std::vector<std::string> getFiles();

	void doAnalysis(int argc, char** argv);

	UtilitiesCodePublisher::DisplayMode displayMode() const;
	UtilitiesCodePublisher::ProcessCmdLine *pcl_;
	//~ICodePublisher() override;


private:

	CodeConverter cconv_;
	Display display_;

	std::string dirIn_;

	std::string dirOut_;
	std::vector<std::string> files_;
};

