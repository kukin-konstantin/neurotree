#include "Keeper_data_set.h"


Keeper_data_set::Keeper_data_set(const char *t_name_file_data,const int t_allow_ram_volume):
name_file_data(t_name_file_data),allow_ram_volume(t_allow_ram_volume)
{
	stream_data.open(t_name_file_data);
}

Keeper_data_set::~Keeper_data_set()
{
	stream_data.clear();
	stream_data.close();
}

double Keeper_data_set::get_number_of_examples(const int t_dim)
{
	double d_number_of_examples=0;
	std::ifstream stream_data_tmp;
	stream_data_tmp.open(name_file_data);
	int i=0;
	double i_tmp;
	while (stream_data_tmp)
	{
		stream_data_tmp>>i_tmp;
		if (i!=t_dim-1)
		{
			i++;
		}
		else
		{
			i=0;
			d_number_of_examples+=1;
		}
	}
	stream_data_tmp.clear();
	stream_data_tmp.close();
	return d_number_of_examples;
}

bool Keeper_data_set::get_example_in_order(std::valarray<double > &v, const int t_dim)
{
	if (!data_block.empty())
	{
		v=data_block.front();
		data_block.pop_front();
		return true;
	}
	else if (stream_data)
	{
		data_block=this->update_data_block(t_dim);
		v=data_block.front();
		data_block.pop_front();
		return true;
	}
	else
	{
		return false;
	}
}

std::deque<std::valarray<double > > Keeper_data_set::update_data_block(const int t_dim)
{
	std::deque<std::valarray<double > > t_data_block;
	std::valarray<double > v_tmp(0.0,t_dim);
	const int mb_in_byte=1048576; // 1 mb = 1 048 575 bytes
	int number_of_example=(mb_in_byte*allow_ram_volume)/(t_dim*sizeof(double));
	while ((stream_data)&&(number_of_example!=0))
	{
		bool ok=true;
		int i=0;
		double i_tmp;
		while ((stream_data)&&(ok))
		{
			stream_data>>i_tmp;
			v_tmp[i]=i_tmp;
			if (i!=t_dim-1)
			{
				i++;
			}
			else
			{
				ok=false;
			}
		}
		if (i==t_dim-1)
			t_data_block.push_back(v_tmp);
		number_of_example--;
	}
	return t_data_block;
}

void  Keeper_data_set::split_file_in_pieces(const char *name_number_cluster,const int t_dim)
{
	std::valarray<double > v_tmp(0.0,t_dim);
	const int mb_in_byte=1048576; // 1 mb = 1 048 576 bytes
	const int number_of_example=(mb_in_byte*allow_ram_volume)/(t_dim*sizeof(double));
	int sh_part_of_files=0;
	while (stream_data)
	{
		int t_number_of_example=number_of_example;
		std::stringstream str;
		sh_part_of_files++;
		str<<sh_part_of_files;
		std::string s_name_file_data_tmp1(name_number_cluster);
		std::string s_name_file_data_tmp2(name_file_data);
		std::string s_name_file_data=s_name_file_data_tmp1+"_part_"+str.str()+"_"+s_name_file_data_tmp2;
		std::ofstream t_file_out(s_name_file_data);
		while ((stream_data)&&(t_number_of_example!=0))
		{
			bool ok=true;
			int i=0;
			double i_tmp;
			while ((stream_data)&&(ok))
			{
				stream_data>>i_tmp;
				v_tmp[i]=i_tmp;
				if (i!=t_dim-1)
				{
					i++;
				}
				else
				{
					ok=false;
				}
			}
			if (i==t_dim-1)
			{
				for (int k=0;k!=t_dim-1;k++)
				{
					t_file_out<<v_tmp[k]<<"\t";
				}
				t_file_out<<v_tmp[t_dim-1]<<"\n";
				t_number_of_example--;
			}
		}
		number_of_examples_in_files.push_back(number_of_example-t_number_of_example);
		t_file_out.clear();
		t_file_out.close();
	}
}

void Keeper_data_set::get_example_in_random_order(std::valarray<double > &v, const double t_num_exam,const int t_dim)
{
	int k=0;
	bool usl=true;
	double tmp_num_exam_mem=t_num_exam;
	double tmp_num_exam;
	while ((k!=number_of_examples_in_files.size())&&(usl))
	{
		tmp_num_exam=tmp_num_exam_mem-number_of_examples_in_files[k];
		if (tmp_num_exam<0)
		{
			usl=false;
		}
		else
		{
			tmp_num_exam_mem=tmp_num_exam;
			k++;
		}
	}

	
}

void Keeper_data_set::get_n_order_vector_in_file(const char *t_name_file_data,std::valarray<double > &v,const int t_num_exam,const int t_dim)
{
	std::ifstream t_stream_data;
	std::string s;
	std::valarray<double > v_tmp(0.0,t_dim);
	t_stream_data.open(t_name_file_data);
	for (int i=0;i!=t_num_exam-1;i++)
	{
		std::getline(t_stream_data,s);// вероятный источник ошибки при выползании за стандартную длину
	}
	double i_tmp;
	for (int k=0;k!=t_dim;k++)
	{
		t_stream_data>>i_tmp;
		v_tmp[k]=i_tmp;
	}
	t_stream_data.clear();
	t_stream_data.close();
	v=v_tmp;
}
