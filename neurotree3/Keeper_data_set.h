#ifndef KEEPERDATASET_H
#define KEEPERDATASET_H
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <valarray>
#include <memory>
#include <assert.h>
#include "conver.h"

class Keeper_data_set 
{ 
public:
	Keeper_data_set(const char *t_name_file_data, const int t_allow_ram_volume);
	Keeper_data_set(const Keeper_data_set &t);
	Keeper_data_set &operator=(const Keeper_data_set& t);
	~Keeper_data_set();
	virtual double get_number_of_examples(const int t_dim)=0;
	virtual bool get_example_in_order(std::valarray<double > &v, const int dim)=0;
	//не подходит,разные сигнатуры//virtual void get_example_in_random_order_whole_piece(std::valarray<double > &v, const int num_exam)=0;
	//virtual bool get_example_in_random_order(std::valarray<double > &v, int t_num_exam,const int t_dim)=0;
	virtual void re_open_stream()=0;
	virtual bool prepare_out_in_random_order(const char *name_number_cluster,std::vector<int> &v_random_list,const int t_dim)=0;//разбивает на файлы в соответсвии с подаваемым порядком, если true - не разбивает и монжно работать в памяти, если false - вне памяти
	virtual void  split_file_in_pieces(const char *name_number_cluster,const int t_dim)=0;
	virtual void clear(const char *name_number_cluster)=0;
	bool get_top_element_data_block(std::valarray<double > &v);
	const char * get_name_file_data() const;
	int get_allow_ram_volume() const;
protected:
	std::string s_name_file_data;
	std::ifstream stream_data;
	int allow_ram_volume; // in Mb
	std::deque<std::valarray<double > > data_block;
	std::vector<int> number_of_examples_in_files;
	virtual std::deque<std::valarray<double > > update_data_block(const int t_dim)=0;
};


class Keeper_data_set_text:public Keeper_data_set  
{ 
public:
	Keeper_data_set_text(const char *t_name_file_data, const int t_allow_ram_volume);
	Keeper_data_set_text(const Keeper_data_set_text &t);
	Keeper_data_set_text &operator=(const Keeper_data_set_text& t);
	~Keeper_data_set_text();
	double get_number_of_examples(const int t_dim);
	bool get_example_in_order(std::valarray<double > &v, const int dim);
	void get_example_in_random_order_whole_piece(std::valarray<double > &v, const int num_exam);
	bool prepare_out_in_random_order(const char *name_number_cluster,std::vector<int> &v_random_list,const int t_dim);//разбивает на файлы в соответсвии с подаваемым порядком, если true - не разбивает и монжно работать в памяти, если false - вне памяти 
	bool get_example_in_random_order(std::valarray<double > &v, int t_num_exam,const int t_dim);
	void split_file_in_pieces(const char *name_number_cluster,const int t_dim);
	void clear(const char *name_number_cluster);
	void re_open_stream();
	//get-interface
	std::vector<int> get_number_of_examples_in_files() const;
	std::deque<std::valarray<double > > get_data_block() const;
	void link_vector_stream_with_files(const char *name_number_cluster);
	void reboot_vector_stream_with_files(const char *name_number_cluster);
	void clear_vector_stream_with_files(const char *name_number_cluster);
private:
	std::vector<std::shared_ptr<std::ifstream> > v_files_in_random;
	std::pair<int,int> get_num_of_file_and_num_of_line(const double t_num_exam); // по номеру примера в большом файле отыскивает маленький файл и номер примера в маленьком файле
	//void get_n_order_vector_in_file(const char *t_name_file_data,std::valarray<double > &v,const int t_num_exam,const int t_dim);
	std::deque<std::valarray<double > > update_data_block(const int t_dim);
};

class Keeper_data_set_bin:public Keeper_data_set
{
	public:
		Keeper_data_set_bin(const char *t_name_file_data, const int t_allow_ram_volume);
		Keeper_data_set_bin(const Keeper_data_set_bin &t);
		Keeper_data_set_bin &operator=(const Keeper_data_set_bin& t);
		~Keeper_data_set_bin();
		double get_number_of_examples(const int t_dim);
		bool get_example_in_order(std::valarray<double > &v, const int dim);
		void get_example_in_random_order_whole_piece(std::valarray<double > &v, const int num_exam,const int t_dim);
		void re_open_stream();
		bool prepare_out_in_random_order(const char *name_number_cluster,std::vector<int> &v_random_list,const int t_dim);
		//bool get_example_in_random_order(std::valarray<double > &v, int t_num_exam,const int t_dim)=0;
		void  split_file_in_pieces(const char *name_number_cluster,const int t_dim);
		void clear(const char *name_number_cluster);
	private:
		std::deque<std::valarray<double > > update_data_block(const int t_dim);
};

#endif