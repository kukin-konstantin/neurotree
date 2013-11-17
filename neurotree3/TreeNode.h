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
#include <valarray>
using namespace std;


struct data_node // ������ ������������ � ����
{
	vector< valarray < double > > train_set; // ��������� ���������
	valarray<double> pos_clus; // ���������� ��������
	bool win; // �������� �� �������?
	int number_node;
	int number_node_vec;
	data_node(valarray<double> t_pos_clus):
	pos_clus(t_pos_clus)
	{
	   win=false;
	}
};

class TreeNode
{
    friend class Tree;
 
public:
    //TreeNode(); // ����������� �� ���������
    TreeNode(data_node &); // ����������� ������������
 
    data_node get_data() const; // ������ ������ ����������� ����
 
private:
    data_node _data; 
    TreeNode *_left;
    TreeNode *_right;
};

/*
TreeNode::TreeNode():
_left(0),
_right(0)
{
}
*/
 

TreeNode::TreeNode(data_node &data):
_data(data),
_left(0),
_right(0)
{
}
 

data_node TreeNode::get_data() const
{
    return _data;
}
 
#endif
