/*
gener.cpp This file is part of the neurotree software package.
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
#include "gener.h"
Gener::Gener()
{
	rng.seed(static_cast<unsigned> ( time(0)));
}

int Gener::get_num(int n)
{
		uniform_int<int> dist(0, n-1);
    	variate_generator<mt19937&, uniform_int<int> >  sampler(rng, dist);
		int tmp_i=sampler();
		return tmp_i;
}