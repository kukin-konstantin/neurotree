/*
main.cpp This file is part of the neurotree software package.
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
#include <iostream>
#include <valarray>
#include <sstream>
#include "Tree.h"
#include "TreeNode.h"
#include "char_to_int.h"
#include "Data_reader.h"

using namespace std;

int main()
{
	clock_t start, end;


    cout<<"Input name setting data files"<<"\n"; //otkomentit' pri norm zapuske
     
	string s_setting;	   //otkomentit' pri norm zapuske
    cin >> s_setting;    //otkomentit' pri norm zapuske
    //s_setting="sett_learn.txt";
    
	cout<<"Enter the number of processors"<<"\n";
	int number_proc;
	cin>>number_proc;
	//number_proc=2;
    
	const char *setting=s_setting.c_str();
    Data_reader sett(setting,"error.txt");
    if (sett.isDataCorrect())
    {
            if (sett.get_method()) // test
            {
					cout<<"testing"<<"\n";
					/* old version input data*/
                    string s_name_file_in=sett.get_file_data_in();
                    string s_name_file_tree=sett.get_tree();
					const char *name_file_in=s_name_file_in.c_str();
                    const char *name_file_tree=s_name_file_tree.c_str();
					
					/*new version input data*/
					/*
					cout<<"Input name of data file"<<"\n";
					string s_name_file_in;
					cin >> s_name_file_in;
					const char *name_file_in=s_name_file_in.c_str();
					
					cout<<"Input name of tree file"<<"\n";
					string s_name_file_tree;
					cin >> s_name_file_tree;
					const char *name_file_tree=s_name_file_tree.c_str();
					*/
					/*new version input data*/
                    
					string s_name_file_cluster=sett.get_cluster();
                    string s_name_file_result=sett.get_result();
                    const char *name_file_cluster=s_name_file_cluster.c_str();
                    const char *name_file_result=s_name_file_result.c_str();
                    if ((sett.exist_file(name_file_in))&&(sett.exist_file(name_file_tree)))
                    {
                            Tree der(name_file_in,name_file_tree,sett.get_memory_size());
                            der.test(name_file_cluster,name_file_result);
                    }
                    else
                    {
                            sett.error_exist("Input and/or tree data file can not exist");
                    }
            }
            else // learning
            {
					cout<<"learning"<<"\n";
					/*old data input*/
                    string s_name_file=sett.get_file_data_in();
					const char *name_file=s_name_file.c_str();
					
					/*new version input data*/
					/*
					cout<<"Input name of data file"<<"\n";
					
					//string s_name_file="data1.txt";
					string s_name_file;	//otkomentit' pri norm zapuske
					cin >> s_name_file;	//otkomentit' pri norm zapuske

					const char *name_file=s_name_file.c_str();
					*/
					/*new version input data*/

                    string s_name_file_tree=sett.get_tree();
                    //const char *name_file_tree=s_name_file_tree.c_str();
                    if (sett.exist_file(name_file))
                    {
						Tree der(name_file,sett.get_dim(),0.01,sett.get_max_number(),sett.get_norm(),sett.get_number_it(),true, number_proc,sett.get_memory_size());
							//Tree der(name_file,sett.get_dim(),0.001,sett.get_max_number(),sett.get_norm(),sett.get_number_it(),false);
							der.learn();
							cout<<"neuron tree file"<<"\n";//add
                            //der.print_in_tree_file(name_file_tree);
							der.print_in_tree_file(s_name_file_tree);
                    }
                    else
                    {
                            sett.error_exist("Input data file can not exist");
                    }
            }
    }
	sett.close_data_error();
	std::system("pause");
    return 0;
}





