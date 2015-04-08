#include <iostream>
#include <fstream>
#include <string>
#include <valarray>
int main()
{
	int t_dim;
	std::string s_name_data_file;
	std::string s_name_data_file_out;
	std::cout<<"input data file"<<"\n";
	std::cin>>s_name_data_file;
	std::cout<<"input dim"<<"\n";
	std::cin>>t_dim;
	std::cout<<"output data file"<<"\n";
	std::cin>>s_name_data_file_out;
	std::ifstream stream_data(s_name_data_file.c_str());
	std::ofstream stream_data_out(s_name_data_file_out.c_str(),std::ios::out|std::ios::binary);
	double i_tmp;
	std::valarray<double > v_tmp(0.0,t_dim);
	while (stream_data)
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
			for (double x:v_tmp)
			{
				stream_data_out.write(reinterpret_cast<char*>(&x),sizeof(double));
			}
		}
	}
	stream_data.close();
	stream_data_out.close(); 
	/*int t_dim=978;
	std::string s_name_data_file_out="learn_nystroem_inp.b";
	std::ifstream stream_prover(s_name_data_file_out.c_str(),std::ios::binary);
	double read;
	stream_prover.seekg(0,std::ios::end);
	double fSize;
	fSize = stream_prover.tellg();
	std::cout<<(fSize/(sizeof(double)*t_dim))<<"\n";
	/*while (stream_prover)
	{
		stream_prover.read( reinterpret_cast<char*>( &read ), sizeof read );
        std::cout << read << '\n';
	}*/
	/*stream_prover.close();*/
	system("pause");
	return 0;
}