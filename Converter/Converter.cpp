///////////////////////////////////////////////////////////////////////////
// Converter.cpp : implements and provides test stub for Converter.h     //
// ver 1.0                                                               //
//                                                                       // 
// Application   : Project 3 OOD                                         //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Abinav Murugadass, Syracuse University			     //
// Source		 : Ammar Salman 313/788-4694, assalman@syr.edu           //
///////////////////////////////////////////////////////////////////////////

#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "Converter.h"
#include <algorithm>
#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Toker.h"
#include "../SemiExpression/Semi.h"
#include "../Project2Starter/Parser.h"
#include "../Project2Starter/ActionsAndRules.h"
#include "../Project2Starter/ConfigureParser.h"
#include "../Project2Starter/Dependencies.h"
#include <queue>
#include <string>

using namespace FileSystem;
using namespace Logger_code_publisher;
using namespace Lexer;
using namespace Utilities_code_publisher;

// -----< default ctor >--------------------------------------------------
CodeConverter::CodeConverter()
{
}

// -----< ctor to set dep table >-----------------------------------------
CodeConverter::CodeConverter(const DependencyTable & dt) : dt_(dt)
{
	Demo::write("\n  CodeConverter initialized with DependencyTable\n");
}

// -----< set dependency table function >---------------------------------
void CodeConverter::setDepTable(const DependencyTable & dt)
{
  dt_ = dt;
}

// -----< convert pre-set dependency table >------------------------------
std::vector<std::string> CodeConverter::convert()
{
  if (!createOutpurDir()) {
	  Demo::write("\n\n  Failed to start conversion due to inability to create output directory");
	  Demo::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
    return std::vector<std::string>();
  }

  Demo::write("\n\n  Converting files to webpages");
  Demo::write("\n\n  Converting files to webpages");

  Demo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  Demo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  for (auto entry : dt_) {
    if (convertFile(entry.first)) {
		Demo::write("\n  -- Converted: ");
		Demo::write("\n  -- Converted: ");
    }
    else {
		Demo::write("\n  -- Failed:    ");
		Demo::write("\n  -- Failed:    ");
    }

    std::string filename = Path::getName(entry.first);
	Demo::write(filename);
	Demo::write(filename);
  }
  Demo::write("\n");
  Demo::write("\n");
  return convertedFiles_;
}

// -----< convert single file given path >----------------------------------
std::string CodeConverter::convert(const std::string & filepath)
{
  if (!createOutpurDir()) {
	  Demo::write("\n\n  Failed to start conversion due to inability to create output directory");
	  Demo::write("\n  -- Output Directory: " + Path::getFullFileSpec(outputDir_));
    return "";
  }

  Demo::write("\n\n  Converting files to webpages");
  Demo::write("\n\n  Converting files to webpages");

  Demo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));
  Demo::write("\n  Output directory: " + Path::getFullFileSpec(outputDir_));

  if (convertFile(filepath)) {
	  Demo::write("\n  -- Converted: ");
	  Demo::write("\n  -- Converted: ");
  }
  else {
	  Demo::write("\n  -- Failed:    ");
	  Demo::write("\n  -- Failed:    ");
  }

  std::string filename = Path::getName(filepath);
  Demo::write(filename);
  Demo::write(filename);

  return filename + ".html";
}
//------<Convert a vector of files>---------------------------------------

std::vector<std::string> CodeConverter::convert(const std::vector<std::string>& files)
{
  clear();
  dt_ = DependencyTable(files);
  return convert();
}

// -----< get set output directory >----------------------------------------
const std::string & CodeConverter::outputDir() const
{
  return outputDir_;
}

// -----< set output directory >--------------------------------------------
void CodeConverter::outputDir(const std::string & dir)
{
  outputDir_ = dir;
}

// -----< create output directory >-----------------------------------------
/* returns success of creation. it could fail if dir is sys protected */
bool CodeConverter::createOutpurDir()
{
  if (!Directory::exists(outputDir_)) {
	  Demo::write("\n  Creating output directory. Path: " + Path::getFullFileSpec(outputDir_));
    return Directory::create(outputDir_);
  }
  Demo::write("\n  Output directory already exists. Proceeding...");
  outputDir_ = Path::getFullFileSpec(outputDir_);
  return true;
}

// -----< get list of converted files >-------------------------------------
const std::vector<std::string> CodeConverter::convertedFiles() const
{
  return convertedFiles_;
}

void CodeConverter::clear()
{
  convertedFiles_.clear();
  dt_.clear();
}
// -----< private - read file and create webpage >--------------------------
bool CodeConverter::convertFile(std::string file)
{

  std::ifstream in(file);
  if (!in.is_open() || !in.good()) {
	  Demo::write("Error -- unable to read file, path may be invalid.");
    in.close();
    return false;
  }
   std::cout << Path::getFullFileSpec(file) << std::endl;

  //linker(Path::getFullFileSpec(file));
  
  std::string filename = Path::getName(file);
  std::string outputPath = outputDir_ + filename + ".html";
  out_.open(outputPath, std::ofstream::out);
  if (!out_.is_open() || !out_.good()) {
	  Demo::write("Error -- unable to open output file for writing.");
    in.close();
    return false;
  }
	  Linker l;
	  std::vector<std::string> files;
	  files = l.linker(file);
	  for (auto fil : files) {
		  dt_.addDependency(file,fil);
	  }
	  std::map<int, types> m;
	  m = l.getMyMap();
	  size_t count = 0;
  addPreCodeHTML(filename);
  addDependencyLinks(file);//adds dependency links /////
  addPreTag();
  Buttons(out_);// adds the buttons for classes comments and functions in the html page
  while (in.good()) {
    std::string line;
    while (std::getline(in, line)) {
		count++;
		
		std::map< int, types>::const_iterator it;
		
		//l.typedetector()
      skipSpecialChars(line); 
	  it = m.find(count); // finds a line number in the map given by the dependencies function for class start and function start
	 
	  if (it != m.end()) {
		  if ((*it).second == 0) {
			  handleClass(line);
		  }
		  if ((*it).second == 1) {
			  handlefunction(line);
		  }
		  if ((*it).second == 2) {
			  handleendscope(line);
		  }
		  if ((*it).second == 3) {
			  handleSingleLineFunctions(line);
		  }
	  }
	  handleComments(line);
	  handleTrailingComments(commentstate, line);
      out_ << line << std::endl;
    }
  }

  addClosingTags();
  out_.close();

  convertedFiles_.push_back(outputPath);
  return true;
}

// -----< private - add generic HTML preliminary markup >-------------------
void CodeConverter::addPreCodeHTML(const std::string& title)
{
  out_ << "<DOCTYPE !HTML>" << std::endl;
  out_ << "<html>" << std::endl;
  out_ << "  <head>" << std::endl;
  out_ << "    <Title>" << title << "</Title>" << std::endl;
  out_ << "    <style>" << std::endl;
  out_ << "      body {" << std::endl;
  out_ << "        padding:15px 40px;" << std::endl;
  out_ << "        font-family: Consolas;" << std::endl;
  out_ << "        font-size: 1.25em;" << std::endl;
  out_ << "        font-weight: normal;" << std::endl;
  out_ << "     }" << std::endl;
  out_ << "      </style>" << std::endl;
  out_ << "      <script src=\"../script.js\"></script>" << std::endl;//changed ../script.js to script.js
  

  out_ << "  </head>" << std::endl << std::endl;
  out_ << "  <body>" << std::endl;
}

// -----< private - add pre tag >------------------------------------------
/* seperated into seperate function to allow for dependencies addition
*  before the actual code of the file */
void CodeConverter::addPreTag()
{
  out_ << "    <pre>" << std::endl;
}

// -----< private - add depedency links markup code >----------------------
void CodeConverter::addDependencyLinks(std::string file)
{
  std::string filename = Path::getName(file);
  if (!dt_.has(file)) { // in case of single file conversion
	  Demo::write("\n    No entry found in DependencyTable for [" + filename + "]. Skipping dependency links..");
    return;
  }

  if (dt_[file].size() == 0) { // in case the file has no dependencies
	  Demo::write("\n    No dependencies found for [" + filename + "]. Skipping dependency links..");
    return;
  }

  out_ << "    <h3>Dependencies: " << std::endl;
  for (auto dep : dt_[file]) {
    out_ << "      <a href=\"" << Path::getName(dep) << ".html\">" << Path::getName(dep) << "</a>" << std::endl;
  }
  out_ << "    </h3>";
}

// -----< private - add generic HTML markup closing tags >-----------------
void CodeConverter::addClosingTags()
{
  out_ << "    </pre>" << std::endl;
  out_ << "  </body>" << std::endl;
  out_ << "</html>" << std::endl;
}

// -----< private - replace HTML special chars >---------------------------
/* note: the function uses while loop to make sure ALL special characters
*  are replaced instead of just the first encounter. */
void CodeConverter::skipSpecialChars(std::string & line)
{
  size_t pos = line.npos;
  while((pos = line.find('<')) != line.npos) 
    line.replace(pos, 1, "&lt;");

  while ((pos = line.find('>')) != line.npos)
    line.replace(pos, 1, "&gt;");
}
//----<handle comments>-----------------------------
void CodeConverter::handleComments(std::string& line) {
	size_t pos = line.find('/');
	if (pos == line.npos)return;
	if (line[pos+1]== '/') {
		line.insert(pos, "<div class = \"toggleMethodDisplay toggleComments\" style= \"display = none;\">");
		line += "</div>";

	}//Comments
	if (line[pos + 1] == '*') {
		line.insert(pos, "<div class = \"toggleMethodDisplay toggleComments\" style= \"display = none;\">");
		commentstate = true;
		return;

	}
}
// -----< Handle class defenitions >------------------------------
void CodeConverter::handleClass(std::string& line) {
		line="<div class = \"toggleMethodDisplay toggleClass\" style= \"display = none;\">"+line; //Class
}
// -----< Handle Function Definitions >------------------------------
void CodeConverter::handlefunction(std::string& line) {
	line= "<div class = \"toggleMethodDisplay toggleFunction\" style= \"display = none;\">"+line; //Function
}
// -----< Handle ending scopes >------------------------------
void CodeConverter::handleendscope(std::string& line) {
	line += "</div>";
}
// -----< Handle single line functions >------------------------------
void CodeConverter::handleSingleLineFunctions(std::string& line) {
	line = "<div class = \"toggleMethodDisplay toggleFunction\" style= \"display = none;\">" + line;
	line.append("</div>");
}
// -----< Handles closing of multi line comments >------------------------------
void CodeConverter::handleTrailingComments(bool commentState, std::string& line) {
	size_t pos = line.find('*');
	if (pos == line.npos)return;
	if (commentState == true){
		if (line[pos + 1] == '/') {
			commentState = false;
			line += "</div>";
		}
		else return;

	}
}
// -----< sets buttons to handle class function and comment divs >------------------------------
void CodeConverter::Buttons(std::ostream& out_) {
	out_ << "<button onclick = ";
	out_ << "showHideClass('toggleMethodDisplay')";
	out_ << "> Show or Hide Method Contents";
	out_ << "</button>" << std::endl;
	out_ << " <div class = \" absolute\"> " << std::endl;
	out_ << "<button onclick = ";
	out_ << "showHideClass('toggleClass')";
	out_ << "> Classes";
	out_ << "</button>" << std::endl;
	out_ << "<button onclick = ";
	out_ << "showHideClass('toggleFunction')";
	out_ << "> Function";
	out_ << "</button>" << std::endl;
	out_ << "<button onclick = ";
	out_ << "showHideClass('toggleComments')";
	out_ << "> Comments";
	out_ << "</button>" << std::endl;
	out_ << "</div>" << std::endl;
}

#ifdef TEST_CONVERTER

// -----< test stub for converter class >-----------------------------------
int main() {
	Demo::attach(&std::cout);
	Demo::start();

	Demo::title("Testing CodeConverter functions");

	Demo::stop(); // do not log DependencyTable logs
	

  DependencyTable dt;

  dt.addDependency("Converter.h", "Converter.cpp");
  dt.addDependency("Converter.cpp", "Converter.h");
  dt.addDependency("A.h", "A.h");
  
  Demo::start(); // log from now on

  CodeConverter cc(dt);
  dt.display();
  Demo::write("\n");
  //cc.outputDir("C:\\windows\\system32\\"); // will trigger errors which are handled
  cc.convert();

  Demo::stop();
  return 0;
}
#endif