#include "sig_complexd.hh"
std::ostream& operator << (std::ostream& os,const Cxd& cxval){

  cout<<"("<<c_blue<<cxval.Re()<<c_normal<<","<<c_green<<cxval.Im()<<c_normal<<")";
}
