#pragma once
///////////////////////////////////////////////////////////////////////////
// DependencyTable.h : defines dependency table structure                //
// ver 1.0                                                               //
//                                                                       // 
// Application   : Project 3 Object Oriented Design                      //
// Platform      : Visual Studio Community 2017, Windows 10 Pro x64      //
// Author        : Abinav Murugadass, Syracuse University                //            
// Source		 : Ammar Salman, Syracuse University					 //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
* =======================
*  This package defines DependencyTable class which is used to hold files
*  relationships after dependency analysis is performed. It is simply a
*  wrapper around std::unordered_map which uses string key and vector
*  value. Therefore, every file in the analysis will point to a vector
*  of all the files it depends upon. 
*
*
*  Required Files:
* =======================
*  DependencyTable.h DependencyTable.cpp Logger.h Logger.cpp
*
*  Maintainence History:
* =======================
*  ver 1.0 - 11 Feb 2019
*  - first release
*/

#include <unordered_map>
#include <string>
#include <vector>

class DependencyTable
{
public:
  using file = std::string;
  using files = std::vector<file>;
  using dependencies = std::vector<file>;
  using iterator = std::unordered_map<file, dependencies>::iterator;


  DependencyTable();
  DependencyTable(const files& list);
  ~DependencyTable();

  bool has(const std::string& file);

  void addDependency(file filename, file dependency);
  void removeDependency(file filename, file dependency);

  void removeEntry(file filename);

  const dependencies& getDependencies(file filename) const;
  const dependencies& operator[](const file& filename) const;

  // these two functions allow 'for' loops in the form of
  // for(auto entry : dt) {...}
  iterator begin();
  iterator end();
  void clear() { table_.clear(); }

  files getFiles() const;

  void display();

private:
  std::unordered_map<file, dependencies> table_;
};

