#include "launcher.h"

Launcher::Launcher(std::string t_name_file_data)
{
  tree = new Tree(t_name_file_data.c_str());
}

Launcher::~Launcher()
{
  std::cout<<"destruct"<<"\n";
  delete tree;
}

void Launcher::get_vec_cluster(boost::python::list& l_in,boost::python::list& l_result)
{
  const int dim=len(l_in);
  valarray<double> v(0.0,dim);
  copy_list_to_valarray(l_in,v,dim);
  vector<int> response;
  tree->get_response_online(v,response);
  copy_vec_to_list(response,l_result);
}

void Launcher::copy_list_to_valarray(boost::python::list& l_list,std::valarray<double> &v,const int dim)
{
  for (int i=0;i<dim;++i)
  {
    v[i]=boost::python::extract<double>(l_list[i]);
  }
}

void Launcher::copy_vec_to_list(std::vector<int> &response,boost::python::list& l_result)
{
  for (int i=0;i!=response.size();i++)
  {
    l_result.append(response[i]);
  }
}

using namespace boost::python;
BOOST_PYTHON_MODULE(launcher)
{
  class_<Launcher>("Launcher",init<std::string>())
	.def("get_response",&Launcher::get_vec_cluster)    
  ;  
}