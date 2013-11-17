/*
Norm.cpp This file is part of the neurotree software package.
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
#include "Norm.h"


double Norm::d_Evkl(const std::valarray<double> &a)
{
	double t_val=0;
	for (unsigned int i=0;i!=a.size();i++)
	{
		t_val=t_val+a[i]*a[i];
	}
	t_val=sqrt(t_val);
	return t_val;
}

double Norm::d_L1(const std::valarray<double> &a)
{
	double t_val=0;
	for (unsigned int i=0;i!=a.size();i++)
	{
		t_val=t_val+abs(a[i]);
	}
	return t_val;
}

double Norm::d_city_block(const std::valarray<double> &a)
{
	double t_val=0;
	int n=a.size();
	for (int i=0;i!=n;i++)
	{
		t_val=t_val+abs(a[i]);
	}
	t_val=t_val/n;
	return t_val;
}

double Norm::d_Gas(const std::valarray<double> &a)
{
	double t_val=0;
	double sigma=1.0; // не очень хорошо,
	for (unsigned int i=0;i!=a.size();i++)
	{
		t_val=t_val+a[i]*a[i];
	}
	t_val=1-exp(-t_val/(2*sigma*sigma));
	return t_val;
}
