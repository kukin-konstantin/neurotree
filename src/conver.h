/*
conver.h This file is part of the neurotree software package.
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
#ifndef CONVER_H
#define CONVER_H
#include "gener.h"
#include <vector>
using namespace std;

/*
	This class modeling game LOTO.
*/

class Conver
{
	public:
		Conver(const int n);
		int get_num(Gener &A);
		bool empty();
	private:
		int size;
		vector<int> v;
};
 
#endif