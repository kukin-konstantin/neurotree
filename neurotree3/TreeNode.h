/*
TreeNode.h This file is part of the neurotree software package.
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
#ifndef TREENODE_H
#define TREENODE_H

#include <vector>
#include <deque>
#include <valarray>
#include "Keeper_data_set.h"
using namespace std;


struct data_node // ������ ������������ � ����
{
	//vector< valarray < double > > train_set; // ��������� ���������
	//deque< valarray < double > > train_set;
	Keeper_data_set keep_data;
	valarray<double> pos_clus; // ���������� ��������
	bool win; // �������� �� �������?
	int number_node;
	int number_node_vec; //номер кластера
	data_node(valarray<double> t_pos_clus,const char *t_name_file_data,const int t_memory_size):
	pos_clus(t_pos_clus),keep_data(t_name_file_data,t_memory_size)
	{
	   win=false;
	}
	data_node(const data_node& data):
	keep_data(data.keep_data)
	{
		pos_clus=data.pos_clus;
		win=data.win;
		number_node=data.number_node;
		number_node_vec=data.number_node_vec;
	}
	data_node & operator=(const data_node& t)
	{
		pos_clus=t.pos_clus;
		win=t.win;
		number_node=t.number_node;
		number_node_vec=t.number_node_vec;
		keep_data=t.keep_data;
		return *this;
	}
	void set_keep_data(Keeper_data_set &t_keep_data)
	{
	   keep_data=t_keep_data;
	}
	int get_memory_size()
	{
		return keep_data.get_allow_ram_volume();
	}
};

class TreeNode
{
    friend class Tree;
 
public:
    //TreeNode(); // ����������� �� ���������
    TreeNode(data_node &); // ����������� ������������
	TreeNode &operator=(const TreeNode& t);
    data_node get_data() const; // ������ ������ ����������� ����
	//void copy_data_set(std::deque<valarray<double > > &t_train_set); //old_version need to remove
private:
    data_node _data; 
    TreeNode *_left;
    TreeNode *_right;
};



TreeNode::TreeNode(data_node &data):
_data(data),
_left(0),
_right(0)
{

}

TreeNode & TreeNode::operator=(const TreeNode& t)
{
	_data=t._data; // правильно используется? data_node operator= не надо писать?
	_left=t._left;
	_right=t._right;
	return *this;
}

data_node TreeNode::get_data() const
{
    return _data;
}

/*void TreeNode::copy_data_set(std::deque<valarray<double > > &t_train_set)//old_version need to remove
{
	int i=0;
	while (!t_train_set.empty())
	{
		_data.train_set.push_back(t_train_set[i]);
		t_train_set.pop_front();
	}
}*/

#endif
