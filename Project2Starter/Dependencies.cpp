/////////////////////////////////////////////////////////////////////
//Dependencies.cpp - Handles dependencies between source code files//
//  ver 1.0														   //
//																   //
//	Author:        Abinav Murugadass, Syracuse university          //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "Dependencies.h"
#include "../FileSystem/FileSystem.h"
#include <queue>
#include <string>
#define Util StringHelper
using namespace Lexer;
using namespace Utilities_code_publisher;
using Demo = Logger_code_publisher::StaticLogger<1>;
using namespace CodeAnalysis;

//------------------<Finds all dependencies in a file based on the include statements and finds the files themselves  >--------
std::vector<std::string> Linker::linker(const std::string& file) 
{
  std::string fileSpec;
  std::vector<std::string> processedFiles;
    fileSpec = FileSystem::Path::getFullFileSpec(file);
    std::string msg = "Processing file" + fileSpec;
    Util::title(msg);

    ConfigParseForCodeAnal configure;
    Parser* pParser = configure.Build();

    std::string name;

    try
    {
      if (pParser)
      {
        name = FileSystem::Path::getName(file);
        if (!configure.Attach(fileSpec))
        {
          std::cout << "\n  could not open file " << name << std::endl;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
      }

      Repository* pRepo = Repository::getInstance();
      pRepo->package() = name;

      while (pParser->next())
      {
        pParser->parse();
      }
      std::cout << "\n";

      ASTNode* pGlobalScope = pRepo->getGlobalScope();
	  
	  std::vector<std::string> vectorOfStatements;
	  auto it = pGlobalScope->statements_.begin();
	  while (it != pGlobalScope->statements_.end()) {

		  vectorOfStatements.push_back((*it)->ToString());
		  ++it;
	  }
	  processedFiles = processdependencies(vectorOfStatements, fileSpec);

      complexityEval(pGlobalScope);
	  typedetector(pGlobalScope);
    }
    catch (std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
      std::cout << "\n  exception caught at line " << __LINE__ << " ";
      std::cout << "\n  in package \"" << name << "\"";
    }
    std::cout << "\n";

  return processedFiles;
}
#ifdef TEST_EXCECUTIVE
int main(int argc, char* argv[])
{
	Util::Title("Testing Project2Starter");
	putline();

	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	std::string fileSpec;
	std::vector<std::string> s;
	for (int i = 1; i < argc; ++i)  // iterate over files
	{
		Linker l;
		s=l.linker(argv[i]);
		for (auto j : s)
		{
			std::cout <<j<< std::endl;
		}

	}
}
#endif

