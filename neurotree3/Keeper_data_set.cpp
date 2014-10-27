#include "Keeper_data_set.h"

/*Keeper_data_set*/
Keeper_data_set::Keeper_data_set(const char *t_name_file_data,const int t_allow_ram_volume):
s_name_file_data(t_name_file_data),allow_ram_volume(t_allow_ram_volume)
{
	assert(allow_ram_volume>0); // если выделяемая память меньше нуля, то прекратить выполнение программы
}

Keeper_data_set::Keeper_data_set(const Keeper_data_set &t):
s_name_file_data(t.s_name_file_data),allow_ram_volume(t.allow_ram_volume)
{
	
}

Keeper_data_set & Keeper_data_set::operator=(const Keeper_data_set& t)
{
	s_name_file_data=t.get_name_file_data();
	allow_ram_volume=t.get_allow_ram_volume();
	return *this;
}

Keeper_data_set::~Keeper_data_set()
{
	stream_data.clear();
	stream_data.close();
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

void Keeper_data_set::clear_vector_stream_with_files(const char *name_number_cluster)
{
	for (int k=0;k!=number_of_examples_in_files.size();k++) 
	{
		v_files_in_random[k]->clear();
		v_files_in_random[k]->close();
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
		remove(s_name_file_data_random.c_str()); //не происхоидит удаления файла, ошибка в имени?
	}
	data_block.clear(); 
}

/*Keeper_data_set*/
/*Keeper_data_set_text*/
Keeper_data_set_text::Keeper_data_set_text(const char *t_name_file_data,const int t_allow_ram_volume):
	Keeper_data_set(t_name_file_data,t_allow_ram_volume)
{
	stream_data.open(t_name_file_data);
}

Keeper_data_set_text::Keeper_data_set_text(const Keeper_data_set_text &t):
Keeper_data_set(t)
{
	re_open_stream();
	number_of_examples_in_files=t.get_number_of_examples_in_files();
	data_block=t.get_data_block();
}

Keeper_data_set_text & Keeper_data_set_text::operator=(const Keeper_data_set_text& t)
{
	Keeper_data_set::operator=(t);
	number_of_examples_in_files=t.get_number_of_examples_in_files();
	data_block=t.get_data_block();
	return *this;
}

Keeper_data_set_text::~Keeper_data_set_text()
{
	
}

void Keeper_data_set_text::re_open_stream()
{
	stream_data.clear();
	stream_data.close();
	stream_data.open(s_name_file_data.c_str()); 
}

double Keeper_data_set_text::get_number_of_examples(const int t_dim)
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

bool Keeper_data_set_text::get_example_in_order(std::valarray<double > &v, const int t_dim)
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

void Keeper_data_set_text::get_example_in_random_order_whole_piece(std::valarray<double > &v, const int num_exam)
{
	v=data_block[num_exam];
}

std::deque<std::valarray<double > > Keeper_data_set_text::update_data_block(const int t_dim)
{
	std::deque<std::valarray<double > > t_data_block;
	std::valarray<double > v_tmp(0.0,t_dim);
	const int mb_in_byte=1048576; // 1 mb = 1 048 575 bytes
	int number_of_example=allow_ram_volume*((mb_in_byte)/(t_dim*sizeof(double)));
	std::cout<<"number_of_example="<<number_of_example<<"\n";
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
	std::cout<<"the end update_data_block"<<"\n";
	return t_data_block;
}

void  Keeper_data_set_text::split_file_in_pieces(const char *name_number_cluster,const int t_dim)
{
	std::valarray<double > v_tmp(0.0,t_dim);
	const int mb_in_byte=1048576; // 1 mb = 1 048 576 bytes
	const int number_of_example=allow_ram_volume*((mb_in_byte)/(t_dim*sizeof(double)));
	int sh_part_of_files=0;
	number_of_examples_in_files.clear();
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

bool Keeper_data_set_text::prepare_out_in_random_order(const char *name_number_cluster,std::vector<int> &v_random_list,const int t_dim) //to big for function
{
	if (number_of_examples_in_files.size()!=1) // 1 - количество кусков разбиения, нельзя всё поместить в память
	{
		std::vector<std::vector<int> > v_random_list_in_group;
		v_random_list_in_group.resize(number_of_examples_in_files.size());
		for (int x:v_random_list) // разбиение набора случайных номеров 1-мерного вектора на k штук по количеству файлов разбиения
		{
			std::pair<int,int> pair_tmp=get_num_of_file_and_num_of_line(double(x));
			v_random_list_in_group[pair_tmp.first].push_back(pair_tmp.second);
		}
		std::vector<std::shared_ptr<std::ifstream> > v_files_in;
		std::vector<std::shared_ptr<std::ofstream> > v_files_out;
		for (int k=0;k!=number_of_examples_in_files.size();k++) //привязка входных-выходных файлов к вектору
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
		for (int k=0;k!=number_of_examples_in_files.size();k++) // цикл по разделенным файлам, k - индекс файла
		{
			std::valarray<double > v_tmp(0.0,t_dim);
			std::deque<std::valarray<double > > t_data_block;
			while ((*v_files_in[k])) // считывание блока k в память
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
			std::cout<<"v_random_list_in_group.size()="<<v_random_list_in_group.size()<<"\n";
			std::cout<<"k="<<k<<"\n";
			std::cout<<"v_random_list_in_group[k].size()="<<v_random_list_in_group[k].size()<<"\n";
			std::cout<<"t_data_block.size()="<<t_data_block.size()<<"\n";
			for (int j=0;j!=v_random_list_in_group[k].size();j++)// цикл по случайным номерам в группе k
			{
				//std::cout<<"!*"<<v_random_list_in_group[k][j]<<"\t";
				for (double x :t_data_block[v_random_list_in_group[k][j]]) // выбирает пример из блока данных по данному случайному номеру 
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
	else // можно всё сразу поместить в память
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


bool Keeper_data_set_text::get_example_in_random_order(std::valarray<double > &v, int t_num_exam, const int t_dim)
/*Можно оптимизировать не беря по одному примеру из файла, а подкачивая их из блоков памяти, уже заполненных*/
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


/*void Keeper_data_set::get_n_order_vector_in_file(const char *t_name_file_data,std::valarray<double > &v,const int t_num_exam,const int t_dim)
{
	std::ifstream t_stream_data;
	std::string s;
	std::valarray<double > v_tmp(0.0,t_dim);
	t_stream_data.open(t_name_file_data);
	for (int i=0;i!=t_num_exam;i++)
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
}*/


std::vector<int> Keeper_data_set_text::get_number_of_examples_in_files() const
{
	return  number_of_examples_in_files;
}

std::deque<std::valarray<double > >  Keeper_data_set_text::get_data_block() const
{
	return data_block;
}

void Keeper_data_set_text::link_vector_stream_with_files(const char *name_number_cluster)
{
	v_files_in_random.clear();
	for (int k=0;k!=number_of_examples_in_files.size();k++) //привязка входных-выходных файлов к вектору
	{
		std::stringstream str;
		str<<k+1;
		std::string s_name_file_data_tmp1(name_number_cluster);
		std::string s_name_file_data_tmp2(s_name_file_data.c_str());
		std::string s_name_file_data_in_random=s_name_file_data_tmp1+"_part_"+str.str()+"_random_"+s_name_file_data_tmp2;
		v_files_in_random.push_back(std::make_shared<ifstream>(s_name_file_data_in_random.c_str()));
	}
}

void Keeper_data_set_text::reboot_vector_stream_with_files(const char *name_number_cluster)
{
	for (int k=0;k!=number_of_examples_in_files.size();k++) 
	{
		std::stringstream str;
		str<<k+1;
		std::string s_name_file_data_tmp1(name_number_cluster);
		std::string s_name_file_data_tmp2(s_name_file_data.c_str());
		std::string s_name_file_data_in_random=s_name_file_data_tmp1+"_part_"+str.str()+"_random_"+s_name_file_data_tmp2;
		v_files_in_random[k]->open(s_name_file_data_in_random.c_str());
	}
}


/*Keeper_data_set_text*/

/*Keeper_data_set_bin*/
Keeper_data_set_bin::Keeper_data_set_bin(const char *t_name_file_data,const int t_allow_ram_volume):
	Keeper_data_set(t_name_file_data,t_allow_ram_volume)
{
	stream_data.open(t_name_file_data,std::ios::binary);
}

Keeper_data_set_bin::Keeper_data_set_bin(const Keeper_data_set_bin &t):
Keeper_data_set(t)
{
	stream_data.open(t.get_name_file_data(),std::ios::binary);
}

Keeper_data_set_bin & Keeper_data_set_bin::operator=(const Keeper_data_set_bin& t)
{
	Keeper_data_set::operator=(t);
	stream_data.open(t.get_name_file_data(),std::ios::binary);
	return *this;
}

Keeper_data_set_bin::~Keeper_data_set_bin()
{
	
}

double Keeper_data_set_bin::get_number_of_examples(const int t_dim)
{
	stream_data.seekg(0,std::ios::end);
	double fSize;
	fSize=stream_data.tellg();
	stream_data.seekg(0,std::ios::beg);
	return (fSize/(sizeof(double)*t_dim));
}

bool Keeper_data_set_bin::get_example_in_order(std::valarray<double > &v, const int dim)
{
	bool result=false;
	if (stream_data)
	{
		double tmp;
		int sh=0;
		while (stream_data)
		{
			stream_data.read( reinterpret_cast<char*>( &tmp ), sizeof tmp );
			v[sh]=tmp;
			sh++;
			if (sh==dim)
			{
				result=true;
				break;
			}
		}
		
	}
	return result;
}

void Keeper_data_set_bin::get_example_in_random_order_whole_piece(std::valarray<double > &v, const int num_exam,const int t_dim)
{
	stream_data.seekg(sizeof(double)*t_dim*num_exam);
	double tmp;
	for (int i=0;i!=t_dim;i++)
	{
		stream_data.read( reinterpret_cast<char*>( &tmp ), sizeof tmp );
		v[i]=tmp;
	}
}

void Keeper_data_set_bin::re_open_stream()
{
	stream_data.clear();
	stream_data.close();
	stream_data.open(s_name_file_data.c_str(),std::ios::binary); 
}

bool Keeper_data_set_bin::prepare_out_in_random_order(const char *name_number_cluster,std::vector<int> &v_random_list,const int t_dim)
{
	if (number_of_examples_in_files.size()!=1) // 1 - количество кусков разбиения, нельзя всё поместить в память
	{
		std::cout<<"number_of_examples_in_files.size()!=1"<<"\n";
		std::vector<std::vector<int> > v_random_list_in_group;
		v_random_list_in_group.resize(number_of_examples_in_files.size());
		for (int x:v_random_list) // разбиение набора случайных номеров 1-мерного вектора на k штук по количеству файлов разбиения
		{
			std::pair<int,int> pair_tmp=get_num_of_file_and_num_of_line(double(x));
			v_random_list_in_group[pair_tmp.first].push_back(pair_tmp.second);
		}
		std::vector<std::shared_ptr<std::ifstream> > v_files_in;
		std::vector<std::shared_ptr<std::ofstream> > v_files_out;
		for (int k=0;k!=number_of_examples_in_files.size();k++) //привязка входных-выходных файлов к вектору
		{
			std::stringstream str;
			str<<k+1;
			std::string s_name_file_data_tmp1(name_number_cluster);
			std::string s_name_file_data_tmp2(s_name_file_data.c_str());
			std::string s_name_file_data_out=s_name_file_data_tmp1+"_part_"+str.str()+"_random_"+s_name_file_data_tmp2;
			std::string s_name_file_data_in=s_name_file_data_tmp1+"_part_"+str.str()+"_"+s_name_file_data_tmp2;
			v_files_out.push_back(std::make_shared<ofstream>(s_name_file_data_out.c_str(),std::ios::out|std::ios::binary));
			v_files_in.push_back(std::make_shared<ifstream>(s_name_file_data_in.c_str(),std::ios::binary));
		}
		for (int k=0;k!=number_of_examples_in_files.size();k++) // цикл по разделенным файлам, k - индекс файла
		{
			std::valarray<double > v_tmp(0.0,t_dim);
			std::deque<std::valarray<double > > t_data_block;
			bool b_out_file=true;
			while ((*v_files_in[k])&&(b_out_file)) // считывание блока k в память
			{
				bool ok=true;
				int i=0;
				double i_tmp;
				while (((*v_files_in[k]))&&(ok))
				{
					v_files_in[k]->read( reinterpret_cast<char*>( &i_tmp ), sizeof i_tmp );
					if ((v_files_in[k]->eof())||(!v_files_in[k]->good()))
					{
						b_out_file=false;
					}
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
			std::cout<<"v_random_list_in_group.size()="<<v_random_list_in_group.size()<<"\n";
			std::cout<<"k="<<k<<"\n";
			std::cout<<"v_random_list_in_group[k].size()="<<v_random_list_in_group[k].size()<<"\n";
			std::cout<<"t_data_block.size()="<<t_data_block.size()<<"\n";
			for (int j=0;j!=v_random_list_in_group[k].size();j++)// цикл по случайным номерам в группе k
			{
				//std::cout<<"!*"<<v_random_list_in_group[k][j]<<"\t";
				for (double x :t_data_block[v_random_list_in_group[k][j]]) // выбирает пример из блока данных по данному случайному номеру 
				{
					v_files_out[k]->write(reinterpret_cast<char*>(&x),sizeof(double));
				}
			}
			v_files_out[k]->clear();
			v_files_out[k]->close();
			v_files_in[k]->clear();
			v_files_in[k]->close();
			t_data_block.clear();
		}
	    return false;
	}
	else // можно всё сразу поместить в память
	{
		std::cout<<"good"<<"\n";
		std::deque<std::valarray<double > > t_data_block;
		//std::map<int,std::valarray<double > > t_data_block;
		re_open_stream();
		t_data_block=update_data_block(t_dim);
		//t_data_block=update_data_block_map(t_dim);
		data_block.clear();
		std::cout<<"v_random_list.size()="<<v_random_list.size()<<"\n";
		for (int num_rand : v_random_list)
		{
			data_block.push_back(t_data_block[num_rand]);
			//t_data_block.erase(num_rand);
		}
		t_data_block.clear();
		return true;
	}
}

void  Keeper_data_set_bin::split_file_in_pieces(const char *name_number_cluster,const int t_dim)//не проверено
{
	//number_of_examples_in_files.push_back(this->get_number_of_examples(t_dim));
	re_open_stream();
	double num_exam_real=this->get_number_of_examples(t_dim);
	std::valarray<double > v_tmp(0.0,t_dim);
	const int mb_in_byte=1048576; // 1 mb = 1 048 576 bytes
	const int number_of_example=allow_ram_volume*((mb_in_byte)/(t_dim*sizeof(double)));
	int num_piece=(num_exam_real/number_of_example);
	number_of_examples_in_files.clear();
	if (num_piece==0)
	{
		number_of_examples_in_files.push_back(int(num_exam_real));
	}
	else
	{
		for (int i=0;i!=num_piece;i++)
		{
			number_of_examples_in_files.push_back(number_of_example);
		}
		number_of_examples_in_files.push_back(int(num_exam_real-num_piece*number_of_example));
		for (int i=0;i!=number_of_examples_in_files.size();i++)
		{
			int num_p=number_of_examples_in_files[i];
			std::stringstream str;
			str<<i+1;
			std::string s_name_file_data_tmp1(name_number_cluster);
			std::string s_name_file_data_tmp2(s_name_file_data.c_str());
			std::string s_name_file_data_t=s_name_file_data_tmp1+"_part_"+str.str()+"_"+s_name_file_data_tmp2;
			std::ofstream t_file_out(s_name_file_data_t.c_str(),std::ios::out|std::ios::binary);
			for (int k=0;k!=num_p;k++)
			{
				double i_tmp;
				for (int d=0;d!=t_dim;d++)
				{
					stream_data.read( reinterpret_cast<char*>( &i_tmp ), sizeof i_tmp );
					t_file_out.write(reinterpret_cast<char*>(&i_tmp),sizeof(double));
				}
			}
			t_file_out.clear();
			t_file_out.close();
		}
	}
}

std::deque<std::valarray<double > > Keeper_data_set_bin::update_data_block(const int t_dim)
{
	std::deque<std::valarray<double > > t_data_block;
	std::valarray<double > v_tmp(0.0,t_dim);
	const int mb_in_byte=1048576; // 1 mb = 1 048 576 bytes
	int number_of_example=allow_ram_volume*((mb_in_byte)/(t_dim*sizeof(double)));
	std::cout<<"number of allowed example="<<number_of_example<<"\n";
	bool b_out_file=true;
	while ((!stream_data.eof())&&(number_of_example!=0)&&(b_out_file))
	{
		bool ok=true;
		int i=0;
		double i_tmp;
		while ((!stream_data.eof())&&(ok))
		{
			stream_data.read( reinterpret_cast<char*>( &i_tmp ), sizeof i_tmp );
			if ((stream_data.eof())||(!stream_data.good()))
			{
				b_out_file=false;
			}
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
	std::cout<<"the end update_data_block"<<"\n";
	std::cout<<"t_data_block.size() "<<t_data_block.size()<<"\n";
	return t_data_block;
}

std::map<int,std::valarray<double > > Keeper_data_set_bin::update_data_block_map(const int t_dim)
{
	std::map<int,std::valarray<double > > t_data_block;
	int sh=0;
	std::valarray<double > v_tmp(0.0,t_dim);
	const int mb_in_byte=1048576; // 1 mb = 1 048 576 bytes
	int number_of_example=allow_ram_volume*((mb_in_byte)/(t_dim*sizeof(double)));
	std::cout<<"number of allowed example="<<number_of_example<<"\n";
	bool b_out_file=true;
	while ((!stream_data.eof())&&(number_of_example!=0)&&(b_out_file))
	{
		bool ok=true;
		int i=0;
		double i_tmp;
		while ((!stream_data.eof())&&(ok))
		{
			stream_data.read( reinterpret_cast<char*>( &i_tmp ), sizeof i_tmp );
			if ((stream_data.eof())||(!stream_data.good()))
			{
				b_out_file=false;
			}
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
			t_data_block.insert(make_pair(sh,v_tmp));
			sh++;
		}
		number_of_example--;
	}
	std::cout<<"the end update_data_block"<<"\n";
	std::cout<<"t_data_block.size() "<<t_data_block.size()<<"\n";
	return t_data_block;
}

void Keeper_data_set_bin::link_vector_stream_with_files(const char *name_number_cluster)
{
	v_files_in_random.clear();
	for (int k=0;k!=number_of_examples_in_files.size();k++) //привязка входных-выходных файлов к вектору
	{
		std::stringstream str;
		str<<k+1;
		std::string s_name_file_data_tmp1(name_number_cluster);
		std::string s_name_file_data_tmp2(s_name_file_data.c_str());
		std::string s_name_file_data_in_random=s_name_file_data_tmp1+"_part_"+str.str()+"_random_"+s_name_file_data_tmp2;
		v_files_in_random.push_back(std::make_shared<ifstream>(s_name_file_data_in_random.c_str(),std::ios::binary));
	}
}

void Keeper_data_set_bin::reboot_vector_stream_with_files(const char *name_number_cluster)
{
	for (int k=0;k!=number_of_examples_in_files.size();k++) 
	{
		std::stringstream str;
		str<<k+1;
		std::string s_name_file_data_tmp1(name_number_cluster);
		std::string s_name_file_data_tmp2(s_name_file_data.c_str());
		std::string s_name_file_data_in_random=s_name_file_data_tmp1+"_part_"+str.str()+"_random_"+s_name_file_data_tmp2;
		v_files_in_random[k]->open(s_name_file_data_in_random.c_str(),std::ios::binary);
	}
}

bool Keeper_data_set_bin::get_example_in_random_order(std::valarray<double > &v, int t_num_exam, const int t_dim)
/*Можно оптимизировать не беря по одному примеру из файла, а подкачивая их из блоков памяти, уже заполненных*/
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
			v_files_in_random[num_piece]->read( reinterpret_cast<char*>( &i_tmp ), sizeof i_tmp );
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

void Keeper_data_set_bin::close_stream()
{
	stream_data.clear();
	stream_data.close();
}

/*Keeper_data_set_bin*/
