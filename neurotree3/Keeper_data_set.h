#ifndef KEEPERDATASET_H
#define KEEPERDATASET_H
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <valarray>

class Keeper_data_set
{ 
public:
	Keeper_data_set(const char *t_name_file_data, const int t_allow_ram_volume);
	~Keeper_data_set();
	double get_number_of_examples(const int t_dim);
	bool get_example_in_order(std::valarray<double > &v, const int dim);
	void get_example_in_random_order(const char *name_number_cluster,std::valarray<double > &v, const double t_num_exam,const int t_dim);
	void split_file_in_pieces(const char *name_number_cluster,const int t_dim);
private:
	const char *name_file_data;
	std::ifstream stream_data;
	const int allow_ram_volume; // in Mb
	std::vector<int> number_of_examples_in_files;
	void get_n_order_vector_in_file(const char *t_name_file_data,std::valarray<double > &v,const int t_num_exam,const int t_dim);
	std::deque<std::valarray<double > > data_block;
	std::deque<std::valarray<double > > update_data_block(const int t_dim);
};


#endif