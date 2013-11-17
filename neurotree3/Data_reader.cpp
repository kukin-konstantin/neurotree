/*
Data_reader.cpp This file is part of the neurotree software package.
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
#include "Data_reader.h"

Data_reader::Data_reader(const char *t_name_file_settings,const char *t_name_file_error):
name_file_error(t_name_file_error)
{
	cond=true;
	ifstream data_setting;
	data_error.open(name_file_error);
	data_setting.open(t_name_file_settings);
	if (data_setting!=NULL)
	{
		data_setting>>s;
		read_method(data_setting);
		data_setting>>s;
		if (!method)
		{
			read_dim(data_setting);
			data_setting>>s;
			read_norma(data_setting);
			data_setting>>s;
			read_max_number(data_setting);
			data_setting>>s;
			read_number_it(data_setting);
		}
		else
		{
			data_setting>>s;
			data_setting>>s;
			data_setting>>s;
		}
		data_setting>>s;
		read_data_file(data_setting,file_data_in);
		data_setting>>s;
		read_data_file(data_setting,tree);
		if (method)
		{
			data_setting>>s;
			read_data_file(data_setting,cluster);
			data_setting>>s;
			read_data_file(data_setting,result);
		}
		data_setting.clear();
		data_setting.close();
	}
	else
	{
		cond=false;
		error_exist("Setting data file can not exist");
	}
}

void Data_reader::error_exist(const char *t)
{
	cond=false;
	data_error<<t<<"\n";
}

void Data_reader::close_data_error()
{
	data_error.clear();
	data_error.close();
}


bool Data_reader::exist_file(const char *t_name_file)
{
	bool ok;
	ifstream file;
	file.open(t_name_file);
	if (file!=NULL)
	{
		ok=true;
	}
	else
	{
		ok=false;
	}
	file.clear();
	file.close();
	return ok;
}

c_name_norma Data_reader::get_norm()
{
	return norm;
}

bool Data_reader::isDataCorrect()
{
	return cond;
}



bool Data_reader::get_method()
{
	return method;
}

int Data_reader::get_dim()
{
	return dimension;
}

int Data_reader::get_number_it()
{
	return number_it;
}

int Data_reader::get_max_number()
{
	return max_number;
}

string Data_reader::get_file_data_in()
{
	return file_data_in;
}

string Data_reader::get_cluster()
{
	return cluster;
}

string Data_reader::get_tree()
{
	return tree;
}

string Data_reader::get_result()
{
	return result;
}



bool Data_reader::get_tag(string &s_buff,ifstream &t_data_sett)
{
	size_t found_begin;
	size_t found_end;
	found_begin=s.find(">");
	found_end=s.find("/");
	if ((found_begin!=string::npos)&&(found_end!=string::npos))
	{
		s_buff=s.substr(found_begin+1,found_end-found_begin-2);
		return true;
	}
	else
	{
		error_exist("Use the correct config file (uncorrect tag)");
		return false;
	}
}

void Data_reader::read_method(ifstream &t_data_sett)
{
	string s_learn ("learning");
	string s_test("testing");
	size_t found;
	found=s.find(s_learn);
	if (found!=string::npos)
	{
		method=false;
	}
	else
	{
		found=s.find(s_test);
		if (found!=string::npos)
		{
			method=true;
		}
		else
		{
			error_exist("Use the correct config file (unknown method)");
		}
	}
}

void Data_reader::read_dim(ifstream &t_data_sett)
{
	int num;
	string s_dim;
	if (get_tag(s_dim,t_data_sett))
	{
		Char_to_int a(s_dim);
		if (a>>num)
		{
			dimension=num;
		}
		else
		{
			error_exist("Use the correct config file (uncorrect dimension)");
		}
	}
}


void Data_reader::read_norma(ifstream &t_data_sett)
{
	/*переписать эту функции не очень хорошая ситуация*/
	/*нужно воспользоваться табличными методами*/
	string s_evkl ("Evkl");
	string s_l1("L1");
	string s_gas("Gas");
	size_t found;
	found=s.find(s_evkl);
	if (found!=string::npos)
	{
		norm=Evkl;
	}
	else
	{
		found=s.find(s_l1);
		if (found!=string::npos)
		{
			norm=L1;
		}
		else
		{
			found=s.find(s_gas);
			if (found!=string::npos)
			{
				norm=Gas;
			}
			else
			{
				error_exist("Use the correct config file (uncorrect norma)");
			}
		}
	}
}


void Data_reader::read_max_number(ifstream &t_data_sett)
{
	int num;
	string s_clust;
	if (get_tag(s_clust,t_data_sett))
	{
		Char_to_int a(s_clust);
		if (a>>num)
		{
			max_number=num;
		}
		else
		{
			error_exist("Use the correct config file (uncorrect max_number)");
		}
	}
}

void Data_reader::read_number_it(ifstream &t_data_sett)
{
	int num;
	string s_it;
	if (get_tag(s_it,t_data_sett))
	{
		Char_to_int a(s_it);
		if (a>>num)
		{
			number_it=num;
		}
		else
		{
			error_exist("Use the correct config file (uncorrect iter)");
		}
	}
}

void Data_reader::read_data_file(ifstream &t_data_sett,string &str)
{
	string t_s;
	if (get_tag(t_s,t_data_sett))
	{
		str=t_s;
	}
}

void Data_reader::read_data_in(ifstream &t_data_sett)
{
	string s_data_in;
	if (get_tag(s_data_in,t_data_sett))
	{
		file_data_in=s_data_in;
	}
}

void Data_reader::read_data_tree(ifstream &t_data_sett)
{
	string s_data_tree;
	if (get_tag(s_data_tree,t_data_sett))
	{
		tree=s_data_tree;
	}
}




