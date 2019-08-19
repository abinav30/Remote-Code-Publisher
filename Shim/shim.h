#pragma once
/////////////////////////////////////////////////////////////////////////////
// Shim.h : defines a warpper for IExecutive which works with .NET code    //
// ver 1.0                                                                 //
// Author         : Abinav Murugadass                                      //
// Application    : CSE-687 C++/CLI Shim                                   //
// Platform       : Visual Studio 2017 Community Edition                   //
//                  Windows 10 Professional 64-bit                         //
//				    EECS Department, Syracuse University			       //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Description:
* ======================
*  This package defines Shim class which can be used by .NET applications.
*  In this demo, Shim is used in WPF project to allow making calls from
*  WPF C# code to CppClass native C++ code. The Shim is a mangaged (.NET)
*  wrapper around ICppClass interface which can make calls to native C++
*  classes and, in the same time, can be called by any .NET code (C# here).
*
*  Public Interface:
* ===================
*  Shim localShim;
*  localShim.processcmdargs(int argc, char**argv);
*  localShim.getcmd(List<String^>^ args);
*   localShim.returnOutputDir();
*
*  Required Files:
* =================
*  Shim.h Shim.cpp ICodePublisher.h
*
*  Build Command:
* ================
*  msbuild Shim.vcxproj
*
*  Maintainence History:
* =======================
*  ver 1.0 - April 14th 2018
*
*/
#include<string>
#include"../CodePublisher/ICodePublisher.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class shim
{
public:
	shim();
	~shim();
	void processcmdargs(int argc, char**argv);// wrapper around process comand line arguments 
	void getcmd(List<String^>^ args);         //gets commands from the wpf application
	String^ returnOutputDir();				  // returns the output directory from the converter
private:
	//String for the output directory
	String^ outputDir;
	// convert System::String to std::string
	std::string sysStrToStdStr(String^ str);
	// convert std::string to System::String
	String^ stdStrToSysStr(const std::string& str);
	// this will point to Executive when instantiated through the ObjectFactory
	IExecutive* client;
};

