#pragma once
///////////////////////////////////////////////////////////////////////////
// Converter.h   : defines source code conversion to webpage functions   //
// ver 1.0                                                               //
//                                                                       // 
// Application   : Project #3 OOD Spring 2019	                         //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Abinav Murugadass, Syracuse University                //
// Source        : Ammar Salman											 //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines CodeConverter class which accepts DependencyTable
*  and uses its relationships to create linked webpages that point to each
*  other based on dependencies. It can also convert single files if needed.
*  The conversion process filters HTML special characters before printing
*  them into output files. The resulting output of this converter is a list
*  of files (vector<string>) of the created webpages. 
*  Update: The converter now is modified to handle insertion of buttons for 
*  toggling methodsso now it links to the dependencies package that finds 
*  dependencies in files,the converter creates dependency links between the 
*  files based on the # include statements moreover the buttons mentioned 
*  above are able to toggle between displaying classes, functions comments
*  and compress them all together. so we have functions for adding the appropriate divs
*  for comments functions and classes
*
*  Required Files:
* =======================
*  Converter.h Converter.cpp Dependencies.h Dependencies.cpp DependencyTable.h DependencyTable.cpp
*  Logger.h Logger.cpp FileSystem.h FileSystem.cpp 
*
*  Maintainence History:
* =======================
*  ver 1.2 14th April 2019
*  -output directory changed to support gui
*  ver 1.1 - 15 Mar 2019
*  - Added functions to handle comments
*  - Added functions to handle add dependencies
*  - Added Functions to handle classes
*  - Added Functions to handle functions
*  ver 1.0 - 11 Feb 2019
*  - first release
*/

#include "../DependencyTable/DependencyTable.h"
#include <vector>
#include <string>
#include <fstream>

class CodeConverter
{
public:
  CodeConverter();                                   //Converter Default Constructor
  CodeConverter(const DependencyTable& dt);			//Initializing converter with dependency table

  const std::string& outputDir() const;				//Return the Output Directory
  void outputDir(const std::string& dir);			//Sets the output Directory
  bool createOutpurDir();							//Creates the output directory

  void setDepTable(const DependencyTable& dt);		//Sets the default values of thedependency table

  std::vector<std::string> convert();				//Converts and returns a vector of files
  std::string convert(const std::string& filepath); //Converts a given file
  std::vector<std::string> convert(const std::vector<std::string>& files); // converts a vector of files
  
  const std::vector<std::string> convertedFiles() const; //Returns the vector of converted files

  void clear();

private:
  bool convertFile(std::string file); 
  void addPreCodeHTML(const std::string& title); //adds the html pre codes
  void addPreTag();								 //Adds the html Pre tag
  void addDependencyLinks(std::string file);     //Adds the dependencies of a particular file
  void addClosingTags();						 //Adds closing tags to a file
  void skipSpecialChars(std::string& line);		 // Adds Special Characters to the file stream
  void handleComments(std::string& line);//handles adding divs to single line comments
  void handleClass(std::string& line); // handles adding divs to classes
  void handlefunction(std::string& line);// handles adding divs to functions
  void handleendscope(std::string& line);// // handles closing divs to functions and classes
  void handleSingleLineFunctions(std::string& line);//handles adding divs to single line functions
  void  handleTrailingComments(bool commentState, std::string& line); // handles trailing divs for multi line comments
  void Buttons(std::ostream& out_); // adds buttons in the appropriate places
private:
  DependencyTable dt_; //Depepndency table instance
  std::string outputDir_ = "../ConvertedWebpages/"; //relative path of the converted files
  std::vector<std::string> convertedFiles_; //Vector of converted files
  std::ofstream out_;//Output stream
  bool commentstate;
};

