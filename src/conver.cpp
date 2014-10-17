/*
conver.cpp This file is part of the neurotree software package.
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
#include "conver.h"

using namespace boost;
Conver::Conver(const int n)
{
	size=n;
	for (int i=0;i!=n;i++)
	{
		v.push_back(i);
	}
}

int Conver::get_num(Gener &A)
{
	if (size!=0)
	{
		int tmp_i=A.get_num(size);
		int number=v[tmp_i];
		size--;
		v[tmp_i]=v[size];
		v.pop_back();
		return number;
	}
	else
	{
		return -1;
	}
}

bool Conver::empty()
{
	if (size!=0)
		return false;
	else
		return true;
}
