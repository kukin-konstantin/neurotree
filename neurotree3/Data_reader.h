/*
Data_reader.h This file is part of the neurotree software package.
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
#ifndef DATAREADER_H
#define DATAREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include "char_to_int.h"
#include "Norm.h"
using namespace std;

class Data_reader
{ 
public:
	Data_reader(const char *t_name_file_settings,const char *t_name_file_error);
	bool isDataCorrect();
	bool get_method();
	int get_dim();
	c_name_norma get_norm();
	int get_max_number();
	int get_number_it();
	string get_file_data_in();
	string get_tree();
	string get_cluster();
	string get_result();
	void error_exist(const char *t);
	bool exist_file(const char *t_name_file);
	void close_data_error();
private:
	const char *name_file_error;
	ofstream data_error;
	bool cond;
	string s;
	bool method; // true - testing, false - learning
	int dimension;
	int number_it;
	c_name_norma norm;
	int max_number;
	string file_data_in;
	string tree;
	string cluster;
	string result;
	bool get_tag(string &s_buff,ifstream &t_data_sett);
	void read_method(ifstream &t_data_sett);
	void read_dim(ifstream &t_data_sett);
	void read_norma(ifstream &t_data_sett);
	void read_max_number(ifstream &t_data_sett);
	void read_number_it(ifstream &t_data_sett);
	void read_data_file(ifstream &t_data_sett,string &str);
	void read_data_in(ifstream &t_data_sett);
	void read_data_tree(ifstream &t_data_sett);
};
 

 
#endif
