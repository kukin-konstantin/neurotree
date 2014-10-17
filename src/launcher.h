#ifndef LAUNC_H
#define LAUNC_H
#include "Tree.h"
#include <boost/python.hpp>

class Launcher
{
public:
  Launcher(std::string t_name_file_data); //init
  void get_vec_cluster(boost::python::list& l_in,boost::python::list& l_result);
  ~Launcher(); //destroy
private:
  void copy_list_to_valarray(boost::python::list& l_list,std::valarray<double> &v,const int dim);
  void copy_vec_to_list(std::vector<int> &response,boost::python::list& l_result);
  Tree *tree;
};

#endif