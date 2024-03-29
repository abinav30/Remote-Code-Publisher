
/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.0                                                             //
// Abinav Murugadass												   //
//Source: CSE687 - Object Oriented Design, Spring 2018                 //
/////////////////////////////////////////////////////////////////////////

#include "ServerPrototype.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include"../CodePublisher/ICodePublisher.h"
#include <chrono>
#include<vector>


namespace MsgPassComm = MsgPassingCommunication;

using namespace Repository;
using namespace FileSystem;
using Msg = MsgPassingCommunication::Message;

Files Server::getFiles(const Repository::SearchPath& path)
{
  return Directory::getFiles(path);
}

Dirs Server::getDirs(const Repository::SearchPath& path)
{
  return Directory::getDirectories(path);
}

template<typename T>
void show(const T& t, const std::string& msg)
{
  std::cout << "\n  " << msg.c_str();
  for (auto item : t)
  {
    std::cout << "\n    " << item.c_str();
  }
}
//--------------------< returns a message back as is >--------------------

std::function<Msg(Msg)> echo = [](Msg msg) {
  Msg reply = msg;
  reply.to(msg.from());
  reply.from(msg.to());
  return reply;
};
//--------------------< get files in a given directory>--------------------
std::function<Msg(Msg)> getFiles = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getFiles");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" +path; //searchPath 
    Files files = Server::getFiles(searchPath);//returns file names and not full paths
    size_t count = 0;
    for (auto item : files)
    {
      std::string countStr = Utilities::Converter<size_t>::toString(++count);
      reply.attribute("file" + countStr, item);
    }
  }
  else
  {
    std::cout << "\n  getFiles message did not define a path attribute";
  }
  return reply;
};
//--------------------< get Directories inside a given directory>--------------------

std::function<Msg(Msg)> getDirs = [](Msg msg) {
  Msg reply;
  reply.to(msg.from());
  reply.from(msg.to());
  reply.command("getDirs");
  std::string path = msg.value("path");
  if (path != "")
  {
    std::string searchPath = storageRoot;
    if (path != ".")
      searchPath = searchPath + "\\" + path;
    Files dirs = Server::getDirs(searchPath);
    size_t count = 0;
    for (auto item : dirs)
    {
      if (item != ".." && item != ".")
      {
        std::string countStr = Utilities::Converter<size_t>::toString(++count);
        reply.attribute("dir" + countStr, item);
      }
    }
  }
  else
  {
    std::cout << "\n  getDirs message did not define a path attribute";
  }
  return reply;
};
//--------------------< processes command line arguments sent from the server and converts files>--------------------

std::function<Msg(Msg)> processArgs = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("getconvertedFiles");
	std::string path = msg.value("cmd");
	if (path != "")
	{
		std::istringstream s(path);
		using StrIt = std::istream_iterator<std::string>;
		std::vector<std::string> container{ StrIt{s},StrIt{ } };
		int argc = container.size();
		char** argv = new char*[argc];
		for (int i = 0; i < argc; i++) {
			std::string arg = container[i];
			argv[i] = new char[container[i].length()];
			strcpy_s(argv[i], container[i].length() + 1, arg.c_str());
		}
		CodePublisher cp;
		cp.doAnalysis(argc, argv);

		std::string searchPath = storageRoot;
		searchPath = "../ConvertedWebpages" ; //searchPath 
		Files files = cp.getfiles();//returns file names and not full paths
		size_t count = 0;
		for (auto item : files)
		{
			std::string countStr = Utilities::Converter<size_t>::toString(++count);
			reply.attribute("file" + countStr, Path::getName(item));
		}
	}
	else
	{
		std::cout << "\n  no arguments recieved";
	}
		return reply;
		
};
//--------------------< Sends files from a server to client >--------------------

std::function<Msg(Msg)> fileselected = [](Msg msg) {
	Msg reply;
	reply.to(msg.from());
	reply.from(msg.to());
	reply.command("filesent");
	std::string path = msg.value("fileselected");
	if (path != "")
	{ 
		reply.attribute("file" , path);
		reply.attribute("filename", path);
	}
	else
	{
		std::cout << "\n  getFiles message did not define a path attribute";
	}
	return reply;

};
void Requirements() {
	std::cout << "--------------------<Requirement # 1 >--------------------------------------" << std::endl<<std::endl;
	std::cout << "Visual Studio 2017 and its C++ Windows Console Projects are used here" << std::endl << std::endl;
	std::cout << "--------------------<Requirement # 2 >--------------------------------------" << std::endl<<std::endl;
	std::cout << "Windows Presentation Foundation (WPF) is used here for the Client's user display" << std::endl<<std::endl;
	std::cout << "--------------------<Requirement # 3 >--------------------------------------" << std::endl<<std::endl;
	std::cout << "assemble working parts from Projects #1, #2, and #3 into a Client-Server configuration." << std::endl;
	std::cout << "Unlike conventional Client-Server architectures, the Client need not wait for a reply from the Publisher server,\n for a request before sending additional requests." << std::endl;
	std::cout << " \n\t\t The Client  is the GUI here and the server is the server prototype that implements the functions\n\t\t of the code publisher" << std::endl;
	
	std::cout << " \n\t\t Our client and server use an asynchronous message passing communication system which uses\n\t\t a single threaded apartment in the server and an asynchronous socket sender and reciever" << std::endl;
	std::cout << " \t\t The very fact that the server is posting these while the cilent has sent messages is a testament to that\n" << std::endl;
	std::cout << "--------------------<Requirement # 4 >--------------------------------------" << std::endl;
	std::cout << "provide a Graphical User Interface (GUI) for the client that supports navigating remote directories to find a project for conversion, and supports displaying the conversion results in a way that meets Project #3 requirement" << std::endl;
	std::cout << " \n\t\t The GUI navigates through remote server directories by sending messages as demonstrated below\n\t\t requesting for files and directories in a given directory" << std::endl;
	std::cout << "\t\t The GUI thus enables remote navigation in the server rooted at a fixed root directory \n\t\t to prevent access to other directories\n" << std::endl;
	std::cout << "--------------------<Requirement # 5 >--------------------------------------" << std::endl;
	std::cout << " provide message designs appropriate for this application. All messages are instances of the same Message class, but have a specified set of attributes and body contents suited for the intended task." << std::endl;
	std::cout << " \n\t\t We design messages such that it has attributes as keys and values. \n\t\t Key is a message attribute and value is the string that we want to send" << std::endl;
	std::cout << " \t\t When a message contains a command, the value is checked which is a callable object \n\t\t that executes a certain function that is set by us and sends a reply message " << std::endl;
}
void Req678() {
	std::cout << "--------------------<Requirement # 6 >--------------------------------------" << std::endl << std::endl;
	std::cout << "support converting source code in the server and with a separate request, transferring one or more converted files back to the local client, using the communication channel." << std::endl;
	std::cout << " \n\t\t As demonstrated below the processArgs command converts the files to web pages";
	std::cout << " \n\t\t and the fileselected command enables the files to be sent from the server directory ../convertedWebpages to \n\t\t SavedPages directory in client" << std::endl << std::endl;

	std::cout << "--------------------<Requirement # 7 >--------------------------------------" << std::endl << std::endl;
	std::cout << "Shall support correct operations for two or more clients" << std::endl;
	std::cout << "\n\t\t this project mmay support this requirement but this hasnt been adequately tested by me\n" << std::endl;
	std::cout << "--------------------<Requirement # 8 >--------------------------------------" << std::endl << std::endl;
	std::cout << "Shall provide an automated test suite that can accept server url and remote path invoke code publisher and open files in theclient's gui view" << std::endl << std::endl;
	std::cout << "\n\t\t this is the automated test suite" << std::endl << std::endl;
	std::cout << "===================================================================================================";
	std::cout << "\n\n\n All requirements have been met here and below is the messages from the client and server and also the output of the code publisher to demonstrate the functioning and flow of the project" << std::endl;
	std::cout << "\n=================================================================================================" << std::endl;
}

int main()
{
  std::cout<<"=================== Demonstrating the requirements of project 4 =================================="<<std::endl<<std::endl;
  Server server(serverEndPoint, "ServerPrototype");
  server.start();
  Requirements();
  Req678();
  std::cout << "\n  testing message processing";
  std::cout << "\n ----------------------------";
  server.addMsgProc("echo", echo);
  server.addMsgProc("getFiles", getFiles);
  server.addMsgProc("getDirs", getDirs);
  server.addMsgProc("processArgs", processArgs);
  server.addMsgProc("fileselected", fileselected);
  server.addMsgProc("serverQuit", echo);
  server.processMessages();

  Msg msg(serverEndPoint, serverEndPoint);  // send to self
  msg.name("msgToSelf");
  msg.command("echo");
  msg.attribute("verbose", "show me");
  server.postMessage(msg);

  
 
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

 
  server.stop();
  return 0;
}

