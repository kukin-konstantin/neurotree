/*
Tree.h This file is part of the neurotree software package.
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
#ifndef TREE_H
#define TREE_H
#include "TreeNode.h"
#include "Norm.h"
#include "char_to_int.h"
#include "char_to_double.h"

using namespace std;



class Tree
{
public:
    Tree(const char *t_name_file_tree);
    void get_response_online(valarray<double> &v,vector<int> &response);
    ~Tree();
private:
    TreeNode *_root;
    c_name_norma norm;
    int dim;
    void delete_helper(TreeNode *);
    void load_tree_from_file_helper(TreeNode *node,string &s,ifstream &t_data_tree, int &sh,int &sh_vec);
    bool read_vec(ifstream &t_data_tree,valarray<double> &v);
    int get_near_cluster(valarray<double> &v,double (Norm::*f_norm) (const valarray<double>&));
    vector<int> get_near_cluster_vec(valarray<double> &v,double (Norm::*f_norm) (const valarray<double>&));
    void error();
};
 
Tree::Tree(const char *t_name_file_tree)
{
	int t_dim;
	string s;
	ifstream data_tree;
	data_tree.open(t_name_file_tree);
	data_tree>>s;
	Char_to_int a(s);
	if (a>>t_dim)
	{
		dim=t_dim;
		data_tree>>s;
		if (s=="Evkl")
		{
			norm=Evkl;
		}
		else if (s=="L1")
		{
			norm=L1;
		}
		else if (s=="City")
		{
			norm=City;
		}
		else if (s=="Gas")
		{
			norm=Gas;
		}
		valarray<double> v_pos_root(0.0,dim);
		if (read_vec(data_tree,v_pos_root))
		{
			data_node data(v_pos_root,"shit.txt",10.0); //change check
			data.win=false;
			_root = new TreeNode(data);
			data_tree>>s;
			if (s=="Left")
			{
				int sh=0;
				int sh_vec=0;
				//std::cout<<"Left"<<"\t";
				load_tree_from_file_helper(_root,s,data_tree,sh,sh_vec);
			}
			else
			{
				std::cout<<"not Left "<<s<<"\n";
				error();
			}
		}
	}
	else
	{
		std::cout<<"not dim "<<"\n";
		error();
	}
	data_tree.clear();
	data_tree.close();
}


Tree::~Tree()
{
    delete_helper(_root);
}
 

void Tree::delete_helper(TreeNode *node)
{
	if (node != nullptr)
    {
        delete_helper(node->_left);
        delete_helper(node->_right);
        delete node;
    }
}
 
void Tree::load_tree_from_file_helper(TreeNode *node,string &s,ifstream &t_data_tree, int &sh,int &sh_vec)
{
	if (s=="Left")
	{
		t_data_tree>>s;
		if (s=="NULL")
		{
			t_data_tree>>s;
			t_data_tree>>s;
			t_data_tree>>s;
			node->_data.number_node=sh;
			sh++; 
			node->_left=nullptr;
			node->_right=nullptr;
		}
		else
		{
			//std::cout<<"Left NULL "<<"\t";
			Char_to_double b(s);
			double d_tmp;
			if (b>>d_tmp)
			{
				valarray<double> v_tmp(0.0,dim-1);
				valarray<double> v_pos_root(0.0,dim);
				if (read_vec(t_data_tree,v_tmp))
				{
					v_pos_root[0]=d_tmp;
					for (unsigned int i=0;i!=v_tmp.size();i++)
					{
						v_pos_root[i+1]=v_tmp[i];
					}
					data_node data(v_pos_root,"shit.txt",10.0); //change check
					data.win=false;
					//изменение 13.11.12
					data.number_node_vec=sh_vec;
					sh_vec++;
					//изменение 13.11.12
					node->_left = new TreeNode(data);
					t_data_tree>>s;
					load_tree_from_file_helper(node->_left,s,t_data_tree,sh,sh_vec);
					load_tree_from_file_helper(node,s,t_data_tree,sh,sh_vec);
				}
			}
		}
	}
	else if (s=="Right")
	{
		valarray<double> v_pos_root(0.0,dim);
		//std::cout<<"Right "<<"\t";
		if (read_vec(t_data_tree,v_pos_root))
		{	
			data_node data(v_pos_root,"shit,txt",10.0); //change check
			data.win=false;
			//изменение 13.11.12
			data.number_node_vec=sh_vec;
			sh_vec++;
			//изменение 13.11.12
			node->_right = new TreeNode(data);
			t_data_tree>>s;
			load_tree_from_file_helper(node->_right,s,t_data_tree,sh,sh_vec);
		}
	}
	else
	{
		//std::cout<<"not Right "<<s<<"\n";
		error();
	}
}


bool Tree::read_vec(ifstream &t_data_tree,valarray<double> &v)
{
	string s;
	double d_tmp;
	for (unsigned int i=0;i!=v.size();i++)
	{
		t_data_tree>>s;
		Char_to_double b(s);
		if (b>>d_tmp)
		{
			v[i]=d_tmp;
		}
		else
		{
			std::cout<<"# i="<<i<<"\n";
			for (int k=0;k!=i;k++)
			{
				std::cout<<v[k]<<"\t";
			}
			std::cout<<"b "<<s<<"\n";
			error();
			return false;
		}
	}
	return true;
}


int Tree::get_near_cluster(valarray<double> &v,double (Norm::*f_norm) (const valarray<double>&))
{
	Norm *o_norm=0;
	valarray<double> pos_clus_left(0.0,dim);
	valarray<double> pos_clus_right(0.0,dim);
	valarray<double> shift_clus_left(0.0,dim);
	valarray<double> shift_clus_right(0.0,dim);
	double norma_l;
	double norma_r;
	TreeNode *node=_root;
	while (node->_left!=nullptr)
	{
		pos_clus_left=node->_left->_data.pos_clus;
		pos_clus_right=node->_right->_data.pos_clus;
		shift_clus_left=v-pos_clus_left;
		shift_clus_right=v-pos_clus_right;
		norma_l=(o_norm->*f_norm)(shift_clus_left);
		norma_r=(o_norm->*f_norm)(shift_clus_right);
		if (norma_l<=norma_r)
		{
			node=node->_left;
		}
		else
		{
			node=node->_right;
		}
	}
	int zn=node->_data.number_node;
	return zn;
}

vector<int> Tree::get_near_cluster_vec(std::valarray<double> &v, double (Norm::* f_norm)(const std::valarray<double> &))
{
	vector<int> v_near_clusters;
	Norm *o_norm=0;
	valarray<double> pos_clus_left(0.0,dim);
	valarray<double> pos_clus_right(0.0,dim);
	valarray<double> shift_clus_left(0.0,dim);
	valarray<double> shift_clus_right(0.0,dim);
	double norma_l;
	double norma_r;
	TreeNode *node=_root;
	while (node->_left!=nullptr)
	{
		pos_clus_left=node->_left->_data.pos_clus;
		pos_clus_right=node->_right->_data.pos_clus;
		shift_clus_left=v-pos_clus_left;
		shift_clus_right=v-pos_clus_right;
		norma_l=(o_norm->*f_norm)(shift_clus_left);
		norma_r=(o_norm->*f_norm)(shift_clus_right);
		if (norma_l<=norma_r)
		{
			node=node->_left;
		}
		else
		{
			node=node->_right;
		}
		v_near_clusters.push_back(node->_data.number_node_vec);
	}
	return v_near_clusters;
}

void Tree::error()
{
	cout<<"Use the correct Tree file"<<"\n";
}

void Tree::get_response_online(valarray<double> &v,vector<int> &response)
{
  double (Norm::*f_norm) (const valarray<double>&) =0;
  if (norm==Evkl)
  {
      f_norm=&Norm::d_Evkl;
  }
  else if (norm==L1)
  {
      f_norm=&Norm::d_L1;
  }
  else if (norm==City)
  {
      f_norm=&Norm::d_city_block;
  }
  else if (norm==Gas)
  {
      f_norm=&Norm::d_Gas;
  }
  response=get_near_cluster_vec(v,f_norm);
}

#endif
