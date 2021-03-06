/*
char_to_int.h This file is part of the neurotree software package.
Neurontree is method of clustering.
    
Copyright (C) 2011 by Kukin K.A.
			  kostikkykin@mail.ru 

    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CHAR_TO_INT_H
#define CHAR_TO_INT_H
#include<string>
#include <sstream>
using namespace std;
class Char_to_int
{
	private:
		string s;
	public :
		Char_to_int(string &s_tmp);
		bool operator>>(int &i_tmp) ;
		~Char_to_int(){};
};
#endif