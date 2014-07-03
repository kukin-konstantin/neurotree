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

class Keeper_data_set // УДАЛИТЬ ВСЕ НЕ используемые функции и переменные
{ 
public:
	Keeper_data_set(const char *t_name_file_data, const int t_allow_ram_volume);
	Keeper_data_set(const Keeper_data_set &t);
	Keeper_data_set &operator=(const Keeper_data_set& t);
	~Keeper_data_set();
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
	bool get_top_element_data_block(std::valarray<double > &v);
	const char * get_name_file_data() const;
	int get_allow_ram_volume() const;
	void link_vector_stream_with_files(const char *name_number_cluster);
private:
	//const char *name_file_data; // заменить на string
	std::string s_name_file_data;
	std::ifstream stream_data;
	int allow_ram_volume; // in Mb
	std::vector<int> number_of_examples_in_files;
	std::vector<std::shared_ptr<std::ifstream> > v_files_in_random;
	std::pair<int,int> get_num_of_file_and_num_of_line(const double t_num_exam); // по номеру примера в большом файле отыскивает маленький файл и номер примера в маленьком файле
	void get_n_order_vector_in_file(const char *t_name_file_data,std::valarray<double > &v,const int t_num_exam,const int t_dim);
	std::deque<std::valarray<double > > data_block;
	std::deque<std::valarray<double > > update_data_block(const int t_dim);
	//std::deque<std::valarray<double > > get_examples_from_file(std::string t_name_file,const int t_dim);
};


#endif