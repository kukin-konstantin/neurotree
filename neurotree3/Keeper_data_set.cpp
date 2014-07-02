#include "Keeper_data_set.h"


Keeper_data_set::Keeper_data_set(const char *t_name_file_data,const int t_allow_ram_volume):
s_name_file_data(t_name_file_data),allow_ram_volume(t_allow_ram_volume)
{
	assert(allow_ram_volume>0); // ���� ���������� ������ ������ ����, �� ���������� ���������� ���������
	stream_data.open(t_name_file_data);
}

Keeper_data_set::Keeper_data_set(const Keeper_data_set &t):
s_name_file_data(t.s_name_file_data),allow_ram_volume(t.allow_ram_volume)
{
	/*stream_data.clear();
	stream_data.close();
	stream_data.open(name_file_data);*/
	re_open_stream();
	number_of_examples_in_files=t.get_number_of_examples_in_files();
	data_block=t.get_data_block();
}

Keeper_data_set & Keeper_data_set::operator=(const Keeper_data_set& t)
{
	s_name_file_data=t.get_name_file_data();
	allow_ram_volume=t.get_allow_ram_volume();
	//stream_data.clear();// �� ��������,  ��� ������ ����������� � ������ ������
	//stream_data.close();//
	//stream_data.open(name_file_data);// 
	number_of_examples_in_files=t.get_number_of_examples_in_files();
	data_block=t.get_data_block();
	return *this;
}

void Keeper_data_set::re_open_stream()
{
	stream_data.clear();
	stream_data.close();
	stream_data.open(s_name_file_data.c_str()); 
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
	stream_data_tmp.open(s_name_file_data.c_str());
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

void Keeper_data_set::get_example_in_random_order_whole_piece(std::valarray<double > &v, const int num_exam)
{
	v=data_block[num_exam];
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
		if (!ok)
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
		std::string s_name_file_data_tmp2(s_name_file_data.c_str());
		std::string s_name_file_data_t=s_name_file_data_tmp1+"_part_"+str.str()+"_"+s_name_file_data_tmp2;
		std::ofstream t_file_out(s_name_file_data_t.c_str());
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
			if (!ok)
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

bool Keeper_data_set::prepare_out_in_random_order(const char *name_number_cluster,std::vector<int> &v_random_list,const int t_dim) //to big for function
{
	if (number_of_examples_in_files.size()!=1) // 1 - ���������� ������ ���������, ������ �� ��������� � ������
	{
		std::vector<std::vector<int> > v_random_list_in_group;
		v_random_list_in_group.resize(number_of_examples_in_files.size());
		for (int x:v_random_list) // ��������� ������ ��������� ������� 1-������� ������� �� k ���� �� ���������� ������ ���������
		{
			std::pair<int,int> pair_tmp=get_num_of_file_and_num_of_line(double(x));
			v_random_list_in_group[pair_tmp.first].push_back(pair_tmp.second);
		}
		std::vector<std::shared_ptr<std::ifstream> > v_files_in;
		std::vector<std::shared_ptr<std::ofstream> > v_files_out;
		for (int k=0;k!=number_of_examples_in_files.size();k++) //�������� �������-�������� ������ � �������
		{
			std::stringstream str;
			str<<k+1;
			std::string s_name_file_data_tmp1(name_number_cluster);
			std::string s_name_file_data_tmp2(s_name_file_data.c_str());
			std::string s_name_file_data_out=s_name_file_data_tmp1+"_part_"+str.str()+"_random_"+s_name_file_data_tmp2;
			std::string s_name_file_data_in=s_name_file_data_tmp1+"_part_"+str.str()+"_"+s_name_file_data_tmp2;
			v_files_out.push_back(std::make_shared<ofstream>(s_name_file_data_out.c_str()));
			v_files_in.push_back(std::make_shared<ifstream>(s_name_file_data_in.c_str()));
		}
		for (int k=0;k!=number_of_examples_in_files.size();k++) // ���� �� ����������� ������, k - ������ �����
		{
			std::valarray<double > v_tmp(0.0,t_dim);
			std::deque<std::valarray<double > > t_data_block;
			while ((*v_files_in[k])) // ���������� ����� k � ������
			{
				bool ok=true;
				int i=0;
				double i_tmp;
				while (((*v_files_in[k]))&&(ok))
				{
					v_files_in[k]->operator>>(i_tmp);
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
				if (!ok)
					t_data_block.push_back(v_tmp);
			}
			for (int j=0;j!=v_random_list_in_group[k].size();j++)// ���� �� ��������� ������� � ������ k
			{
				for (double x :t_data_block[v_random_list_in_group[k][j]]) // �������� ������ �� ����� ������ �� ������� ���������� ������ 
				{
					*v_files_out[k]<<x;
					*v_files_out[k]<<"\t";
				}
				*v_files_out[k]<<"\n";
			}
			v_files_out[k]->clear();
			v_files_out[k]->close();
			v_files_in[k]->clear();
			v_files_in[k]->close();
			t_data_block.clear();
		}
	    return false;
	}
	else // ����� �� ����� ��������� � ������
	{
		std::deque<std::valarray<double > > t_data_block;
		/*stream_data.clear();
		stream_data.close();
		stream_data.open(name_file_data);*/
		re_open_stream();
		t_data_block=update_data_block(t_dim);
		data_block.clear();
		for (int num_rand : v_random_list)
		{
			data_block.push_back(t_data_block[num_rand]);
		}
		return true;
	}
}

/*std::deque<std::valarray<double > > Keeper_data_set::get_examples_from_file(std::string t_name_file,const int t_dim)
{
	std::deque<std::valarray<double > > t_data_block;
	Keeper_data_set t(t_name_file.c_str(),this->allow_ram_volume);
	return t_data_block;
}*/

bool Keeper_data_set::get_example_in_random_order(std::valarray<double > &v, int t_num_exam, const int t_dim)
/*����� �������������� �� ���� �� ������ ������� �� �����, � ���������� �� �� ������ ������, ��� �����������*/
{
	std::pair<int,int> pair_tmp=get_num_of_file_and_num_of_line(double(t_num_exam));
	int num_piece=pair_tmp.first;
	if ((*v_files_in_random[num_piece])) 
	{
		bool ok=true;
		int i=0;
		double i_tmp;
		while (((*v_files_in_random[num_piece]))&&(ok))
		{
			v_files_in_random[num_piece]->operator>>(i_tmp);
			v[i]=i_tmp;
			if (i!=t_dim-1)
			{
				i++;
			}
			else
			{
				ok=false;
			}
		}
		if (!ok)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

std::pair<int,int> Keeper_data_set::get_num_of_file_and_num_of_line(const double t_num_exam)
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
	std::pair<int,int> tmp_pair(k,tmp_num_exam_mem);
	return tmp_pair;
}

void Keeper_data_set::get_n_order_vector_in_file(const char *t_name_file_data,std::valarray<double > &v,const int t_num_exam,const int t_dim)
{
	std::ifstream t_stream_data;
	std::string s;
	std::valarray<double > v_tmp(0.0,t_dim);
	t_stream_data.open(t_name_file_data);
	for (int i=0;i!=t_num_exam;i++)
	{
		std::getline(t_stream_data,s);// ��������� �������� ������ ��� ���������� �� ����������� �����
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

void Keeper_data_set::clear(const char *name_number_cluster)
{
	int number_of_pieces=number_of_examples_in_files.size();
	for (int k=0;k!=number_of_pieces;k++)
	{
		std::stringstream str;
		str<<k+1;
		std::string s_name_file_data_tmp1(name_number_cluster);
		std::string s_name_file_data_tmp2(s_name_file_data.c_str());
		std::string s_name_file_data=s_name_file_data_tmp1+"_part_"+str.str()+"_"+s_name_file_data_tmp2;
		std::string s_name_file_data_random=s_name_file_data_tmp1+"_part_"+str.str()+"_random_"+s_name_file_data_tmp2;
		remove(s_name_file_data.c_str());
		remove(s_name_file_data_random.c_str()); //�� ����������� �������� �����, ������ � �����?
	}
	data_block.clear(); 
}

std::vector<int> Keeper_data_set::get_number_of_examples_in_files() const
{
	return  number_of_examples_in_files;
}

std::deque<std::valarray<double > >  Keeper_data_set::get_data_block() const
{
	return data_block;
}

const char * Keeper_data_set::get_name_file_data() const
{
	return s_name_file_data.c_str();
}

int Keeper_data_set::get_allow_ram_volume() const
{
	return allow_ram_volume;
}

bool Keeper_data_set::get_top_element_data_block(std::valarray<double > &v)
{
	if (!data_block.empty())
	{
		v=data_block.front();
		data_block.pop_front();
		return true;
	}
	else
	{
		return false;
	}
}

void Keeper_data_set::link_vector_stream_with_files(const char *name_number_cluster)
{
	for (int k=0;k!=number_of_examples_in_files.size();k++) //�������� �������-�������� ������ � �������
	{
		std::stringstream str;
		str<<k+1;
		std::string s_name_file_data_tmp1(name_number_cluster);
		std::string s_name_file_data_tmp2(s_name_file_data.c_str());
		std::string s_name_file_data_in_random=s_name_file_data_tmp1+"_part_"+str.str()+"_random_"+s_name_file_data_tmp2;
		v_files_in_random.push_back(std::make_shared<ifstream>(s_name_file_data_in_random.c_str()));
	}
}