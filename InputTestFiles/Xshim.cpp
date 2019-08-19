/////////////////////////////////////////////////////////////////////////////
// Shim.cpp       : provides definitions and test stub for Shim.h          //
// ver 1.0                                                                 //
//                                                                         //
// Application    : CSE-687 C++/CLI Shim demostration                      //
// Platform       : Visual Studio 2017 Community Edition                   //
// Author         : Abinav Murugadass Syracuse University				   //
/////////////////////////////////////////////////////////////////////////////
#include "shim.h"
#include<iostream>

//------< shim constructor that instantiates the client using the object factory >-------------------------------
shim::shim()
{


	ObjectFactory factory;
	client = factory.createClient();
	Console::WriteLine("\n\t\t->The Shim is Created here!");
	Console::Write("\n\t\t\t-> Shim instantiated Executive as ICppClient* which owns an instance of code publisher\n");
}
//------<shim Destructor that deletes the client object >-------------------------------

shim::~shim()
{
	delete client;
	Console::WriteLine("\n the instance of executive is destroyed and shim is destroyed too!");
}
//-----< convert std:string to Sysem::String>---------

String^ shim::stdStrToSysStr(const std::string& str)
{
	return gcnew String(str.c_str());
}
//-----< convert System::String to std::string>---------

std::string shim::sysStrToStdStr(String^ str)
{
	std::string temp;
	for (int i = 0; i < str->Length; ++i) {
		temp += static_cast<char>(str[i]);
	}
	return temp;
}
//-----<Calls the process command line function of the client>---------

void shim::processcmdargs(int argc, char** argv)
{
	client->processcmdargs(argc, argv);
}

//-----<Gets the list of commands from the GUI and converts into the format that needs to be given to the Publisher>---------

void shim::getcmd(List<String^>^ args)
{
	int argc = args->Count;
	char** argv = new char*[argc];
	for (int i = 0; i < argc; i++) {
		std::string arg = sysStrToStdStr(args[i]);
		argv[i] = new char[args[i]->Length];
		strcpy_s(argv[i], args[i]->Length + 1, arg.c_str());
	}
	processcmdargs(argc, argv);
}

//------------------<Returns the output directory of the converted files from the converter for the GUI>----------------
String^ shim::returnOutputDir()
{
	outputDir = stdStrToSysStr(client->outputDir());
	return outputDir;
}

//--------<Test stub for the shim project>---------------
//--------<need to change proerties from dll to exe and the preprocessor to test the shim>-----------
#ifdef TEST_SHIM
int main(array<String^> ^args) {
	shim localShim;  // C++ style construction
  //Shim^ localshim = gcnew Shim(); // managed heap allocation and construction
	int argc = 5;
	char** argv = new char*[argc];
	argv[0] = "\0";
	argv[1] = "../";
	argv[2] = "/s";
	argv[3] = "*.h";
	argv[4] = "[A](.*)";
	localShim.processcmdargs(argc, argv);


	Console::Write("\n\n  Shim posting	 to CppClass:");
	Console::Write("\n =======================================\n");


	Console::Write("\n\n  Shim getting 10 messages from CppClass:");
	Console::Write("\n =========================================\n");


	Console::Write("\n\n  Finished demonstration.");
	Console::Write("\n  Shim dtor is automatically called after 'main' goes out of scope.\n");
	return 0;
}
#endif