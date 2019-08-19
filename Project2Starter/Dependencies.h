#pragma once
/////////////////////////////////////////////////////////////////////
//  Dependencies.h - Handles dependencies between source code files//
//  ver 1.0                                                        //
//  Author:        Abinav Murugadass Syracuse University		   //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines the LinkerClass which accepts files, Parses them 
*  to do two main operations: 
*		- The file is parsed and dependencies of each file is extracted based on the # include statements
*		- The Abstract Syntax tree of each file is constructed and the method declarations are extracted from the tree
*		- A map of line numbers and their respective enum state is constructed and returned to the converter so that it can do further 
*       - on the file
*  Required Files:
* =======================
*  AbstrSynTree.h, AbstrSynTree.cpp, ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.h, ConfigureParser.cpp, Dependencies.h, 
*  Dependencies.cpp, FileSystem.h, FileSystem.cpp, GrammarHelpers.h, GrammarHelpers.cpp, ITokCollection.h, ITokCollection.cpp, Logger.h, 
*  Logger.cpp, Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp, Semi.h, Semi.cpp, Toker.h, Toker.cpp, Utilities.h, Utilities.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 15 March 2019
*  - first release
*/
#include<map>
enum types {
	classes, functions,endscope,singlelinefunction

};
class Linker{
public:
//------------------< parses file and adds the full path of dependant files based upon the include statements>--------
	std::vector<std::string> linker(const std::string& file);

	template<typename t>
//------------------< Adds Contents to the local type map based on the class and functions declarations  >--------
	void typedetector(t* node) {
		
		auto iter = node->children_.begin();
		while (iter != node->children_.end()) {
			if ((*iter)->type_ == "class") {
				myMap.insert({ (*iter)->startLineCount_,classes });
				myMap.insert({ (*iter)->endLineCount_-1,endscope });
				
			}
			else if ((*iter)->type_ == "function") {
				 size= static_cast<int>((*iter)->endLineCount_ - (*iter)->startLineCount_ + 1);
				 if (size <= 0) {
					 myMap.insert({ (*iter)->startLineCount_ ,singlelinefunction });
					 //size = 1;
				 }
				 else {
					 myMap.insert({ (*iter)->startLineCount_ ,functions });
					 myMap.insert({ (*iter)->endLineCount_, endscope });
				 }
				
			}
			typedetector(*iter);
			++iter;
		}
	}
//------------------<Returns the map of types >--------
	std::map< int, types> getMyMap() {
		return myMap;
	}
//------------------< Process the dependencies to obtain the full file path of the dependent files>--------
	std::vector<std::string> processdependencies(std::vector<std::string> & vectorofstatements, std::string& fileSpec) {
		std::vector<std::string> p;
		for (std::string str : vectorofstatements) {
			size_t pos = str.find('<');
			if (std::string::npos == pos) {
				unsigned f = str.find_first_of('\"');
				unsigned l = str.find_last_of('\"');
				if (f != std::string::npos) {
					std::string fileName = str.substr(f + 1, l - f - 1);
					std::string currentDirectory = FileSystem::Directory::getCurrentDirectory();
					std::string fileNames = FileSystem::Path::getFullFileSpec(currentDirectory + '/' + fileName);
					if (fileName.find('/') == std::string::npos) {
						l = fileSpec.find_last_of('\\');

						fileSpec = fileSpec.substr(0, l + 1);
						fileNames = fileSpec + fileName;
					}
					p.push_back(fileNames);
				}

			}

		}
		return p;
	}
private:
	std::map< int, types> myMap;
	int size;
};