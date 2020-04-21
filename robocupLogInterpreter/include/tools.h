// Class used to read from Robocup log and save interpreted data into csv.
// Copyright (C) 2020  Arthur Demarchi

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#ifndef INCLUDE_TOOLS_H_
#define INCLUDE_TOOLS_H_

// Includes
#include <vector>
#include <filesystem>
#include <string>
#include <iostream>

// function to list all files of one specified extension into
// a root directory
// Args:
//  const std::string &rootDir: Root directory from wich the tree will be made.
//  const std::string &extension: Wich extension to look foward
// Return:
//  std::vector<std::string>: vector with all file paths.
// Example:
//  std::vector<std::string> listOfTextFilesPaths = listFiles("My/DIR/WITH/FLES", ".txt");
std::vector<std::string> listFiles(const std::string &rootDir, const std::string &extension);

// function to facilitate making a boolean question to the user
// Args:
//  const std::string &question: string ot the question you want ot make.
// Return:
//  bool: Returns the user answer to the question
// Example:
//  bool users_choise = booleanQuestion("Do you want ice cream?");
bool booleanQuestion(const std::string &question);

// function to facilitate making a multi alternative question to the user
// Args:
//  const std::string &question: string ot the question you want ot make.
//  const std::vector<Strd::string> alternatives: vector of strings that are the alternatives to choose from.
// Return:
//  int: returns a integer that is the index to the alternative answered by the user.
// Example:
//  std::vector<std::string> alternatives;
//  alternatives.reserve(3);
//  alternatives.push_back("Chocolate");
//  alternatives.push_back("Strawberry");
//  alternatives.push_back("No Flavour");
//  int users_choice = multipleAlternativeQuestion("Wich ice cream flavour do you like?", alternatives);
//  bool users_choise = booleanQuestion("Do you want ice cream?");/
int multipleAlternativeQuestion(const std::string &question, std::vector<std::string> alternatives);

#endif // INCLUDE_TOOLS_H_