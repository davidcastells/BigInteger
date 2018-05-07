/*
 * Copyright (C) 2017 Universitat Autonoma de Barcelona - David Castells-Rufas <david.castells@uab.cat>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   to_string.h
 * Author: dcr
 *
 * Created on May 4, 2018, 4:01 PM
 */

#ifndef TO_STRING_H
#define TO_STRING_H

#include <string>
#include <sstream> 
#include <iostream>

template <typename T>
std::string to_string(T value)
{
  //create an output string stream
  std::ostringstream os ;

  //throw the value into the string stream
  os << value ;

  //convert the string stream into a string and return
  return os.str() ;
}

template <typename T>
std::string to_hex_string(T value)
{
  //create an output string stream
  std::ostringstream os ;

  //throw the value into the string stream
  os << std::hex << value ;

  //convert the string stream into a string and return
  return os.str() ;
}


#endif /* TO_STRING_H */

