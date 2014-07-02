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
 
#include <fstream>
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "TreeNode.h"
#include "Norm.h"
#include "char_to_int.h"
#include "char_to_double.h"
#include "conver.h"
#include "gener.h"
using namespace std;



class Tree
{
public:
    Tree(const char *t_name_file_data,int t_dim,double t_accuar,int t_max_number_cluster,c_name_norma t_norm,int t_number_iter,bool t_numberORaccuar,int t_number_proc, int t_memory_size);//конструктор для обучения
    Tree(const char *t_name_file_tree); //конструктор для тестирования
	void learn(); // �������� ������
	void test(const char *t_name_file_data,const char *t_name_file_clusters,const char *t_name_file_result);
	void print_in_tree_file(const char *t_name_file_tree);
	//void copy_data_set(std::deque<valarray<double > > &t_train_set,data_node &data); // old version need to remove
	void copy_data_set(Keeper_data_set &keep,data_node &data);
	~Tree();
private:
	TreeNode *_root;
	vector<TreeNode *> last_layer; //
	vector<TreeNode *> tmp_layer;
	/*������ ���������*/
	const char *name_file_data;
	double size_data;
	int dim;
	double accuar;
	double val_func;
	double summa_acc;
	int max_number_cluster;
	c_name_norma norm;
	int number_iter;
	bool numberORaccuar;
	int memory_size;
	int last_number_clusters;
	int number_proc;

	boost::mutex layer_push_mutex;
	boost::mutex acc_mutex;
	/*������ ���������*/
    
    void delete_helper(TreeNode *);
	void print_in_tree_file_helper(TreeNode *node,ofstream &t_data_tree);
	void load_tree_from_file_helper(TreeNode *node,string &s,ifstream &t_data_tree, int &sh,int &sh_vec);
    deque<valarray<double > > read_data_to_memory();//old version, unuse
	valarray<double> def_pos_root_node(std::deque<valarray<double > > &t_train_set);//����������� ��������� ��������� �������
	valarray<double> def_pos_root_node(Keeper_data_set &keep);
	void expand_neuron_m_thread(int num_nodes,int num_threads);
	void expand_neuron(int first_index,int last_index,int num_calc_node); // � �������� ����������� ��������� ���������� map<TreeNode *,int>
	void learn_neuron_m_thread(int num_nodes,int num_threads,double (Norm::*f_norm) (const valarray<double>&));
	void learn_neuron(Gener &A,double (Norm::*f_norm) (const valarray<double>&),int first_index,int last_index); // ������� ������� ���������� ������
	void learn_acc(double (Norm::*f_norm) (const valarray<double>&));  //обучение по точности, временно не поддерживается
	void learn_max_number(double (Norm::*f_norm) (const valarray<double>&)); // �������� ���������� �������� ������������ ���������� ���������
	void learn_one_iteration(TreeNode * T_tmp,double (Norm::*f_norm) (const valarray<double>&),const char *name_number_cluster,valarray<double> &v_tmp,double x);
	void cond_exit_acc(double (Norm::*f_norm) (const valarray<double>&),int first_index,int last_index); // posible to split thread
	bool cond_exit_acc_m_thread(int num_nodes,int num_threads,double (Norm::*f_norm) (const valarray<double>&));
	bool cond_exit_max_number();
	double alpha(double &x);
	void del_dead_neuron(double (Norm::*f_norm) (const valarray<double>&),int first_index,int last_index);//posible to split thread (techology of mutex, problem - sh, require to block)
	void del_dead_m_thread(int num_nodes,int num_threads,double (Norm::*f_norm) (const valarray<double>&));
	void assign_num_node_layer();
	bool read_vec(ifstream &t_data_tree,valarray<double> &v);
	int get_near_cluster(valarray<double> &v,double (Norm::*f_norm) (const valarray<double>&));
	vector<int> get_near_cluster_vec(valarray<double> &v,double (Norm::*f_norm) (const valarray<double>&));
	int get_number_of_proc(int number_all_proc);
	void get_average_dist_in_clusters(vector<double> &average_cluster_rad,vector<int> &numbers_data_to_one_cluster);// future change: add possibility change name file
	void get_dist_between_clusters(double (Norm::*f_norm) (const valarray<double>&));
	void error();
	void get_random_list(std::vector<int> &v_random_list,Gener &A,Conver &pock);
};
 

Tree::Tree(const char *t_name_file_data,int t_dim,double t_accuar,int t_max_number_cluster,c_name_norma t_norm,int t_number_iter,bool t_numberORaccuar,int t_number_proc,int t_memory_size):
_root(0),name_file_data(t_name_file_data),accuar(t_accuar),max_number_cluster(t_max_number_cluster),norm(t_norm),number_iter(t_number_iter),numberORaccuar(t_numberORaccuar),number_proc(t_number_proc),memory_size(t_memory_size)
{
	dim=t_dim;
	Keeper_data_set keep(name_file_data,t_memory_size);
	size_data=keep.get_number_of_examples(dim);
	valarray<double> v_pos_root=def_pos_root_node(keep);

    //deque<valarray<double > > t_train_set=read_data_to_memory(); //old version need to remove
	//size_data=t_train_set.size();                               //old version need to remove
	//valarray<double> v_pos_root=def_pos_root_node(t_train_set);//old version need to remove
	

	data_node data(v_pos_root,name_file_data,memory_size);
	copy_data_set(keep,data);
	//copy_data_set(t_train_set,data); //old version need to remove
	//t_train_set.clear(); //old version need to remove
	data.win=false;
	data.number_node=1;
	_root = new TreeNode(data);
	//copy_data_set(data.train_set,_root->_data); //old version need to remove
	last_layer.push_back(_root);
	val_func=0;
	last_number_clusters=0;
}
 
Tree::Tree(const char *t_name_file_tree):
_root(0)
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
			data_node data(v_pos_root,name_file_data,memory_size); //change check
			data.win=false;
			_root = new TreeNode(data);
			//copy_data_set(data.train_set,_root->_data); // old version need to remove
			data_tree>>s;
			if (s=="Left")
			{
				int sh=0;
				int sh_vec=0;
				load_tree_from_file_helper(_root,s,data_tree,sh,sh_vec);
			}
			else
			{
				error();
			}
		}
	}
	else
	{
		error();
	}
	data_tree.clear();
	data_tree.close();
}



/*void Tree::copy_data_set(std::deque<valarray<double> > &t_train_set,data_node &data) // old version need to remove
{
	int i=0;
	while (!t_train_set.empty())
	{
		data.train_set.push_back(t_train_set[i]);
		t_train_set.pop_front();
	}
	//std::copy(t_train_set.begin(),t_train_set.end(),std::back_inserter(data.train_set));
}*/

void Tree::copy_data_set(Keeper_data_set &keep,data_node &data)
{
	data.set_keep_data(keep);
}

void Tree::print_in_tree_file(const char *t_name_file_tree)
{
	ofstream data_tree(t_name_file_tree);
	data_tree<<dim<<"\n";
	cout<<dim<<"\n";
	if (norm==Evkl)
	{
		data_tree<<"Evkl"<<"\n";
		cout<<"Evkl"<<"\n";//add
	}
	else if (norm==L1)
	{
		data_tree<<"L1"<<"\n";
		cout<<"L1"<<"\n";//add
	}
	else if (norm==City)
	{
		data_tree<<"City"<<"\n";
		cout<<"City"<<"\n";//add
	}
	else if (norm==Gas)
	{
		data_tree<<"Gas"<<"\n";
		cout<<"Gas"<<"\n";//add
	}
	print_in_tree_file_helper(_root,data_tree);
	data_tree.clear();
	data_tree.close();
}

void Tree::learn()
{
	/*����� ����� ����� ��������� �� �������*/
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
	/*����� ����� ����� ��������� �� �������*/
	if (numberORaccuar)
	{
		learn_max_number(f_norm);
	}
	else
	{
		learn_acc(f_norm);
	}

}


void Tree::test(const char *t_name_file_data,const char *t_name_file_clusters,const char *t_name_file_result)
{
	name_file_data=t_name_file_data;
    deque<valarray<double > > t_test_set=read_data_to_memory(); // исключить keep_data_set
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
	/*������ ��������� �������*/
	/*average distance in cluster*/
	int numbers_of_clusters=last_layer.size();
	vector<double> average_cluster_rad(numbers_of_clusters,0.0);
	vector<int>    numbers_data_to_one_cluster(numbers_of_clusters,0);
	valarray<double> shift(0.0,dim);
	Norm *o_norm=0;
	double dist;
	/*average distance in cluster*/
	valarray<double> v_tmp(0.0,dim);
	ofstream data_result(t_name_file_result);
	cout<<"result clustering"<<"\n";//add
	for (unsigned int i=0;i!=t_test_set.size();i++) // заменить цикл на keep_data_set
	{
		v_tmp=t_test_set[i];
		/*data_result<<get_near_cluster(v_tmp,f_norm)<<"\n";* old*/
		/*new console*/
		int i_tmp=get_near_cluster(v_tmp,f_norm);
		vector<int> v_near_clusters=get_near_cluster_vec(v_tmp,f_norm);
		/*изменение 13.11.2012*/
		for (int j=0;j!=v_near_clusters.size();j++)
		{
			data_result<<v_near_clusters[j]<<"\t";
		}
		data_result<<"\n";
		//data_result<<i_tmp<<"\n";
		/*изменение 13.11.2012*/
		cout<<i_tmp<<"\n";
		/*new console*/
		//average dist
		numbers_data_to_one_cluster[i_tmp]+=1;
		shift=v_tmp-last_layer[i_tmp]->_data.pos_clus;
		dist=(o_norm->*f_norm)(shift);
		average_cluster_rad[i_tmp]+=dist;
		//average dist
	}
	data_result.clear();
	data_result.close();
	/*������ ��������� �������*/
	TreeNode *T_tmp;
	ofstream data_clusters(t_name_file_clusters);
	vector<TreeNode *>::iterator it=last_layer.begin();
	cout<<"data cluster"<<"\n";//add
	while (it!=last_layer.end())
	{
		T_tmp=(*it);
		for (unsigned int i=0;i!=T_tmp->_data.pos_clus.size();i++)
		{
			/*data_clusters<<T_tmp->_data.pos_clus[i]<<"\t";//add  old*/
			/*new console*/
			double d_tmp=T_tmp->_data.pos_clus[i];
			data_clusters<<d_tmp<<"\t";
			cout<<d_tmp<<"\t";
			/*new console*/
		}
		data_clusters<<"\n";
		cout<<"\n";//add
		it++;
	}
	data_clusters.clear();
	data_clusters.close();
	//new possibility,get distance between clusters and between clusters and data_set
	get_average_dist_in_clusters(average_cluster_rad,numbers_data_to_one_cluster);
	get_dist_between_clusters(f_norm);
	//new possibility,get distance between clusters and between clusters and data_set
}

Tree::~Tree()
{
    delete_helper(_root);
}
 

void Tree::delete_helper(TreeNode *node)
{
    if (node != 0)
    {
        delete_helper(node->_left);
        delete_helper(node->_right);
 
        delete node;
    }
}
 
deque<valarray<double > > Tree::read_data_to_memory()
{
	int i=0;
	double i_tmp;
	valarray<double> v_tmp(0.0,dim);
    deque<valarray<double > > t_train_set;
	ifstream stream_data;
	stream_data.open(name_file_data);
	while (stream_data!=NULL)
	{
		stream_data>>i_tmp;
		v_tmp[i]=i_tmp;
		if (i!=dim-1)
		{
			i++;
		}
		else
		{
			t_train_set.push_back(v_tmp);
			i=0;
		}
	}
	stream_data.clear();
	stream_data.close();
	return t_train_set;
}

valarray<double> Tree::def_pos_root_node(std::deque<valarray<double > > &t_train_set)
{
	valarray<double> v_tmp(0.0,dim);
	valarray<double> v_cent(0.0,dim);
	double k=0;
	double umn;
	for (unsigned int i=0;i!=t_train_set.size();i++)
	{
		v_tmp=t_train_set[i];
		v_cent=v_cent+v_tmp;
		k++;
	}
	umn=1.0/k;
	v_cent=umn*v_cent;
	return v_cent;
}

valarray<double> Tree::def_pos_root_node(Keeper_data_set &keep)
{
	double umn_pr=keep.get_number_of_examples(dim);
	size_data=umn_pr;
	valarray<double> v(0.0,dim);
	valarray<double> v_cent(0.0,dim);
	while (keep.get_example_in_order(v,dim))
	{
		v_cent=v_cent+v;
	}
	v_cent=v_cent/umn_pr;
	return v_cent;
}
int Tree::get_number_of_proc(int number_all_proc)
{
	if (number_proc>number_all_proc)
	{
		return number_all_proc;
	}
	else
	{
		return number_proc;
	}
}

void Tree::learn_acc(double (Norm::*f_norm) (const valarray<double>&))
{
	/*����������� ������ ���� ������ ������: ���� �� ������ ������, ������ �� ������ ������*/
	int num_of_avail_proc= boost::thread::hardware_concurrency();
	size_t num_threads=get_number_of_proc(num_of_avail_proc);
	int num_nodes=last_layer.size();//define number of nodes
	while (!cond_exit_acc_m_thread(num_nodes,num_threads,f_norm)) // ����� ������� ���� ��������
	{
		expand_neuron_m_thread(num_nodes,num_threads);
		learn_neuron_m_thread(num_nodes,num_threads,f_norm);
		del_dead_m_thread(num_nodes,num_threads,f_norm);
		num_nodes=last_layer.size();//define number of nodes
	}
}

void Tree::learn_max_number(double (Norm::*f_norm) (const valarray<double>&))
{
	/*����������� ������ ���� ������ ������: ���� �� ������ ������, ������ �� ������ ������*/
	int num_of_avail_proc= boost::thread::hardware_concurrency();
	size_t num_threads=get_number_of_proc(num_of_avail_proc);
	while (!cond_exit_max_number()) // Если количество кластеров превышено или количество примеров в кластере меньше допустимого,то выход из цикла 
	{
		int num_nodes=last_layer.size();//define number of nodes
		expand_neuron_m_thread(num_nodes,num_threads);
		learn_neuron_m_thread(num_nodes,num_threads,f_norm);
		del_dead_m_thread(num_nodes,num_threads,f_norm);
	}
}

void Tree::expand_neuron(int first_index,int last_index, int num_calc_node) 
{
	TreeNode * T_tmp;
	TreeNode * T_tmp_left;
	TreeNode * T_tmp_right;
	for (int i = first_index; i < last_index; ++i)
	{
		T_tmp=last_layer[i];
		const char *t_name_file_data=T_tmp->_data.keep_data.get_name_file_data(); 
		std::string s_name_file_data(t_name_file_data);
		s_name_file_data=s_name_file_data.substr(0,s_name_file_data.size()-4); // 4 соответсвует - .txt
		std::string s_name_file_data_l=s_name_file_data+"_l.txt";
		std::string s_name_file_data_r=s_name_file_data+"_r.txt";
		data_node data_l(T_tmp->_data.pos_clus,s_name_file_data_l.c_str(),memory_size/(2*num_calc_node)); // деление выделенной памяти на два
		data_node data_r(T_tmp->_data.pos_clus,s_name_file_data_r.c_str(),memory_size/(2*num_calc_node)); // деление выделенной памяти на два
		data_l.win=false;
		data_r.win=false;
		T_tmp_left = new TreeNode(data_l);
		T_tmp_right = new TreeNode(data_r);
		T_tmp->_left=T_tmp_left;
		T_tmp->_left->_data.keep_data.re_open_stream();
		T_tmp->_right=T_tmp_right;
		T_tmp->_right->_data.keep_data.re_open_stream();
	}
}

void Tree::expand_neuron_m_thread(int num_nodes,int num_threads)
{
	int num_calc_node;// nummber of calculating node
	if (num_nodes<num_threads)
	{
		num_calc_node=num_nodes;
	}
	else
	{
		num_calc_node=num_threads;
	}
	boost::thread_group threads;
	for (int i=0; i<num_calc_node; ++i)
		threads.create_thread(boost::bind(&Tree::expand_neuron,this,i*num_nodes/num_calc_node, (i+1)*num_nodes/num_calc_node,num_calc_node));
	threads.join_all();
}

void Tree::learn_neuron(Gener &A,double (Norm::*f_norm) (const valarray<double>&),int first_index,int last_index)
{
	TreeNode * T_tmp;
	int size_train_data;
	double x;
	double dob=1.0986;//alpha 0.25
	std::vector<int> v_random_list;
	for (int i_num_node = first_index; i_num_node < last_index; ++i_num_node) 
	{
		T_tmp=last_layer[i_num_node];
		std::string  s_tmp=std::to_string(T_tmp->_data.number_node);
		const char *name_number_cluster=s_tmp.c_str();
		T_tmp->_data.keep_data.split_file_in_pieces(name_number_cluster,dim); // здесь ошибка 02/07, почему не поделил и не создал файлов?
		size_train_data=int(T_tmp->_data.keep_data.get_number_of_examples(dim));
		for (int j=0;j!=number_iter;j++) 
		{
			x=dob+(j*(log((1.0/accuar)-1.0)-dob))/number_iter;
			Conver pock(size_train_data);//изменить keep_data_set
			get_random_list(v_random_list,A,pock);  
			if (T_tmp->_data.keep_data.prepare_out_in_random_order(name_number_cluster,v_random_list,dim))
			{
				valarray<double> v_tmp(0.0,dim);
				while (T_tmp->_data.keep_data.get_top_element_data_block(v_tmp))
				{
					learn_one_iteration(T_tmp,f_norm,name_number_cluster,v_tmp,x);
				}
			}
			else // нельзя поместить в память
			{
				valarray<double> v_tmp(0.0,dim);
				T_tmp->_data.keep_data.link_vector_stream_with_files(name_number_cluster);
				for (int random_num:v_random_list)
				{
					assert(T_tmp->_data.keep_data.get_example_in_random_order(v_tmp,random_num,dim)==true);//выдача случайного вектора из файлов по запросу- его порядковому номеру
					learn_one_iteration(T_tmp,f_norm,name_number_cluster,v_tmp,x);
				}
			}
		}
		T_tmp->_data.keep_data.clear(name_number_cluster);
	}
}


void Tree::learn_one_iteration(TreeNode * T_tmp,double (Norm::*f_norm) (const valarray<double>&),const char *name_number_cluster,valarray<double> &v_tmp,double x)
{
	valarray<double> pos_clus_left(0.0,dim);
	valarray<double> pos_clus_right(0.0,dim);
	valarray<double> shift_clus_left(0.0,dim);
	valarray<double> shift_clus_right(0.0,dim);
	Norm *o_norm=0;
	double norma_l;
	double norma_r;
	pos_clus_left=T_tmp->_left->_data.pos_clus;
	pos_clus_right=T_tmp->_right->_data.pos_clus;
	
	shift_clus_left=v_tmp-pos_clus_left;
	shift_clus_right=v_tmp-pos_clus_right;
	norma_l=(o_norm->*f_norm)(shift_clus_left);
	norma_r=(o_norm->*f_norm)(shift_clus_right);
	if (norma_l<=norma_r)
	{
		T_tmp->_left->_data.pos_clus=T_tmp->_left->_data.pos_clus+alpha(x)*shift_clus_left;
		T_tmp->_left->_data.win=true;
	}
	else
	{
		T_tmp->_right->_data.pos_clus=T_tmp->_right->_data.pos_clus+alpha(x)*shift_clus_right;
		T_tmp->_right->_data.win=true;
	}
}


void Tree::learn_neuron_m_thread(int num_nodes,int num_threads,double (Norm::*f_norm) (const valarray<double>&))
{
	int num_calc_node;// nummber of calculating node
	if (num_nodes<num_threads)
	{
		num_calc_node=num_nodes;
	}
	else
	{
		num_calc_node=num_threads;
	}
	boost::thread_group threads;
	for (int i=0; i<num_calc_node; ++i)
	{
		Gener A;
		threads.create_thread(boost::bind(&Tree::learn_neuron,this,A,f_norm,i*num_nodes/num_calc_node, (i+1)*num_nodes/num_calc_node));
	}
	threads.join_all();
}

void Tree::del_dead_neuron(double (Norm::*f_norm) (const valarray<double>&),int first_index,int last_index)//правлю 30.06
{
	TreeNode * T_tmp;
	TreeNode * T_tmp_left;
	TreeNode * T_tmp_right;
	valarray<double> pos_clus_left(0.0,dim);
	valarray<double> pos_clus_right(0.0,dim);
	valarray<double> shift_clus_left(0.0,dim);
	valarray<double> shift_clus_right(0.0,dim);
	Norm *o_norm=0;
	double norma_l;
	double norma_r;
	valarray<double> v_tmp(0.0,dim);
	vector<TreeNode *> tmp_layer_local;
	for (int i_num_node = first_index; i_num_node < last_index; ++i_num_node)
	{
		T_tmp=last_layer[i_num_node]; // здесь теряется информация
		T_tmp_left=T_tmp->_left;
		T_tmp_right=T_tmp->_right;
		pos_clus_left=T_tmp_left->_data.pos_clus;
		pos_clus_right=T_tmp_right->_data.pos_clus;
		if (T_tmp_left->_data.win==0) // источник возможных ошибок при возникновении мертвых кластеров, проверить на модельной ситуации
		{
			T_tmp_left=0;
			T_tmp->_data.pos_clus=T_tmp->_right->_data.pos_clus;
			T_tmp_right=0;
			T_tmp->_right=NULL;
			tmp_layer_local.push_back(T_tmp);
		}
		else if (T_tmp_right->_data.win==0) // источник возможных ошибок при возникновении мертвых кластеров, проверить на модельной ситуации
		{
			T_tmp->_right=0;
			T_tmp->_data.pos_clus=T_tmp->_left->_data.pos_clus;
			T_tmp_left=0;
			T_tmp->_left=NULL;
			tmp_layer_local.push_back(T_tmp);
		}
		else
		{
			const char *t_name_data_file=T_tmp->_data.keep_data.get_name_file_data();
			std::string s_tmp_name_data_file_w_txt(t_name_data_file);
			std::string s_tmp_name_data_file;
			s_tmp_name_data_file=s_tmp_name_data_file_w_txt.substr(0,s_tmp_name_data_file_w_txt.size()-4);
			valarray<double> v_tmp(0.0,dim);
			std::string s_tmp_name_data_file_l=s_tmp_name_data_file+"_l.txt";
			std::ofstream t_st_left(s_tmp_name_data_file_l.c_str());
			std::string s_tmp_name_data_file_r=s_tmp_name_data_file+"_r.txt";
			std::ofstream t_st_right(s_tmp_name_data_file_r.c_str());
			T_tmp->_data.keep_data.re_open_stream();
			while (T_tmp->_data.keep_data.get_example_in_order(v_tmp,dim))//последовательный перебор
			{
				shift_clus_left=v_tmp-pos_clus_left;
				shift_clus_right=v_tmp-pos_clus_right;
				norma_l=(o_norm->*f_norm)(shift_clus_left);
				norma_r=(o_norm->*f_norm)(shift_clus_right);
				if (norma_l<=norma_r)
				{
					for (double x:v_tmp)
					{
						t_st_left<<x<<"\t";
					}
					t_st_left<<"\n";
					//T_tmp_left->_data.train_set.push_back(v_tmp); // добавить объект к левому узлу
				}
				else
				{
					for (double x:v_tmp)
					{
						t_st_right<<x<<"\t";
					}
					t_st_right<<"\n";
					//T_tmp_right->_data.train_set.push_back(v_tmp); // добавить объект к правому узлу
				}
			}
			t_st_left.clear();
			t_st_left.close();
			t_st_right.clear();
			t_st_right.close();
			std::string  s_tmp=std::to_string(T_tmp->_data.number_node);
		    const char *name_number_cluster=s_tmp.c_str();
			T_tmp->_data.keep_data.clear(name_number_cluster); //T_tmp->_data.train_set.clear();
			tmp_layer_local.push_back(T_tmp_left);// здесь должно быть оформление keep_data_set
			tmp_layer_local.push_back(T_tmp_right);// здесь должно быть оформление keep_data_set
		}
	}
	boost::mutex::scoped_lock lock(layer_push_mutex);
	tmp_layer.insert(tmp_layer.end(), tmp_layer_local.begin(), tmp_layer_local.end());
}

void Tree::assign_num_node_layer ()
{
	for (int i=0;i!=tmp_layer.size();i++)
	{
		tmp_layer[i]->_data.number_node=i+1;
	}
}

void Tree::del_dead_m_thread(int num_nodes,int num_threads,double (Norm::*f_norm) (const valarray<double>&))
{
	int num_calc_node;// nummber of calculating node
	if (num_nodes<num_threads)
	{
		num_calc_node=num_nodes;
	}
	else
	{
		num_calc_node=num_threads;
	}
	boost::thread_group threads;
	tmp_layer.clear();
	num_calc_node=1; // one thread (debug)
	for (int i=0; i<num_calc_node; ++i)
	{
		threads.create_thread(boost::bind(&Tree::del_dead_neuron,this,f_norm,i*num_nodes/num_calc_node, (i+1)*num_nodes/num_calc_node));
	}
	threads.join_all();
	assign_num_node_layer();
	last_layer=tmp_layer;
}

double Tree::alpha(double &x)
{
	return 1.0/(1.0+exp(x));
}


void Tree::cond_exit_acc(double (Norm::*f_norm) (const valarray<double>&),int first_index,int last_index)
{
	TreeNode * T_tmp;
	valarray<double> v_tmp(0.0,dim);
	valarray<double> v_pos_clus(0.0,dim);
	valarray<double> v_shift(0.0,dim);
	double d_tmp;
	Norm *o_norm=0;
	for (int i_num_node = first_index; i_num_node < last_index; ++i_num_node)
	{
		T_tmp=last_layer[i_num_node];
		v_pos_clus=T_tmp->_data.pos_clus;
		/*for (unsigned int i=0;i!=T_tmp->_data.train_set.size();i++) //изменить keep_data_set
		{
				v_tmp=T_tmp->_data.train_set[i];
				v_shift=v_tmp-v_pos_clus;
				d_tmp=(o_norm->*f_norm)(v_shift);
				boost::mutex::scoped_lock lock(acc_mutex);
				summa_acc+=(d_tmp*d_tmp)/size_data;
		}*/
	}
}

bool Tree::cond_exit_acc_m_thread(int num_nodes,int num_threads,double (Norm::*f_norm) (const valarray<double>&))
{
	int num_calc_node;// nummber of calculating node
	if (num_nodes<num_threads)
	{
		num_calc_node=num_nodes;
	}
	else
	{
		num_calc_node=num_threads;
	}
	boost::thread_group threads;
	summa_acc=0;
	for (int i=0; i<num_calc_node; ++i)
	{
		threads.create_thread(boost::bind(&Tree::cond_exit_acc,this,f_norm,i*num_nodes/num_calc_node, (i+1)*num_nodes/num_calc_node));
	}
	threads.join_all();
	if (abs(val_func-summa_acc)<=accuar) 
	{
		cout<<"closure:\t"<<abs(val_func-summa_acc)<<"\n";
		return true;
	}
	else
	{
		cout<<"closure:\t"<<abs(val_func-summa_acc)<<"\n";
		val_func=summa_acc;
		return false;
	}
}


bool Tree::cond_exit_max_number()
{
	int n;
	n=last_layer.size();
	cout<<n<<"\n"<<"\n";
	if ((max_number_cluster>n)&&(last_number_clusters!=n))
	{
		last_number_clusters=n;
		return false;
	}
	else
	{
		return true;
	}
}





void Tree::print_in_tree_file_helper(TreeNode *node,ofstream &t_data_tree)
{
	if (node != 0)
    {
		for (unsigned int i=0;i!=node->_data.pos_clus.size();i++)
		{
			t_data_tree<<node->_data.pos_clus[i]<<"\t";
			cout<<node->_data.pos_clus[i]<<"\t";//add
		}
		t_data_tree<<"Left"<<"\n";
		cout<<"Left"<<"\n";//add
        print_in_tree_file_helper(node->_left,t_data_tree);
		t_data_tree<<"Right"<<"\n";
		cout<<"Right"<<"\n";//add
        print_in_tree_file_helper(node->_right,t_data_tree);
    }
	else
	{
		t_data_tree<<"NULL"<<"\n";
		cout<<"NULL"<<"\n";//add
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
			last_layer.push_back(node);
			sh++; 
			node->_left=0;
			node->_right=0;
		}
		else
		{
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
					data_node data(v_pos_root,name_file_data,memory_size); //change check
					data.win=false;
					//изменение 13.11.12
					data.number_node_vec=sh_vec;
					sh_vec++;
					//изменение 13.11.12
					node->_left = new TreeNode(data);
					//copy_data_set(data.train_set,node->_left->_data); //изменить keep_data_set
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
		if (read_vec(t_data_tree,v_pos_root))
		{	
			data_node data(v_pos_root,name_file_data,memory_size); //change check
			data.win=false;
			//изменение 13.11.12
			data.number_node_vec=sh_vec;
			sh_vec++;
			//изменение 13.11.12
			node->_right = new TreeNode(data);
			//copy_data_set(data.train_set,node->_right->_data); // old version need to remove
			t_data_tree>>s;
			load_tree_from_file_helper(node->_right,s,t_data_tree,sh,sh_vec);
		}
	}
	else
	{
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
	while (node->_left!=NULL)
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
	while (node->_left!=NULL)
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

void Tree::get_average_dist_in_clusters(vector<double> &average_cluster_rad,vector<int> &numbers_data_to_one_cluster)
{
	ofstream data_aver_dist("average_dist_in_clusters.txt");
	for (int i=0;i!=average_cluster_rad.size();i++)
	{
		average_cluster_rad[i]=average_cluster_rad[i]/numbers_data_to_one_cluster[i];
		data_aver_dist<<average_cluster_rad[i]<<"\n";
	}
	data_aver_dist.clear();
	data_aver_dist.close();
}

void Tree::get_dist_between_clusters(double (Norm::*f_norm) (const valarray<double>&))
{
	valarray<double> shift(0.0,dim);
	valarray<double> pos_clus_left(0.0,dim);
	valarray<double> pos_clus_right(0.0,dim);
	Norm *o_norm=0;
	double dist;
	int n=last_layer.size();
	ofstream data_dist_between_clust("dist_between_clusters.txt");
	for (int i=0;i!=n;i++)
	{
		pos_clus_left=last_layer[i]->_data.pos_clus;
		for (int j=0;j!=n;j++)
		{
			pos_clus_right=last_layer[j]->_data.pos_clus;
			shift=pos_clus_left-pos_clus_right;
			dist=(o_norm->*f_norm)(shift);
			data_dist_between_clust<<dist<<"\t";
		}
		data_dist_between_clust<<"\n";
	}
	data_dist_between_clust.clear();
	data_dist_between_clust.close();
}

void Tree::error()
{
	cout<<"Use the correct Tree file"<<"\n";
}

void Tree::get_random_list(std::vector<int> &v_random_list,Gener &A,Conver &pock)
{
	int i;
	v_random_list.clear();
	while (!pock.empty())
	{
		i=pock.get_num(A);
		v_random_list.push_back(i);
	}
}

#endif
