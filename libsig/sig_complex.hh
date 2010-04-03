#ifndef sig_complex_hh
#define sig_complex_hh

#include "cpp_monit.hh"
#include "sig_scalar.hh" //for rsqrt

// Define class Cxs for vector of two doubles.
// The low element represents the real part and the high element
// represents the imaginary part of a complex number:
template<typename TX>
class Cx {
   protected:
   TX vec[2];              // Vector of two TX's
   public:
   // Default constructor:
   Cx() {};
   // Constructor to make from real and imaginary part:
   Cx(TX const& re, TX const& im) {
     vec[0] = re; vec[1] = im;}
   // Constructor to convert from real number. Set imag. part to 0:
   Cx(TX const& x) {
     vec[0] = x; vec[1] = 0;}
   TX Re() const {
      return vec[0];}
   // Member function to extract Imaginary part:
   TX Im() const {
     return vec[1];}
   // void operator = (const Cx& val){
   //   vec[0] = val.vec[0];  vec[1] = val.vec[1];
   // }
};

// Define operators for class Cx:
// complex + real: add only to real part:
template<typename TX>
static inline Cx<TX> operator + (Cx<TX> const &a, TX const& b){
  return Cx<TX>(a.Re()+b,a.Im());}
// complex - real: subtract only from real part:
template<typename TX>
static inline Cx<TX> operator - (Cx<TX> const &a, TX const& b){
   return Cx<TX>(a.Re()-b,a.Im());}
// complex * real: multiply both real and imaginary part:
template<typename TX>
static inline Cx<TX> operator * (Cx<TX> const &a, TX const& b){
   return Cx<TX>(a.Re()*b,a.Im()*b);}
template<typename TX>
static inline Cx<TX> operator * (TX const& b,Cx<TX> const &a){
   return Cx<TX>(a.Re()*b,a.Im()*b);}
// complex / real: multiply both real and imag. part by reciprocal b:
template<typename TX>
static inline Cx<TX> operator / (Cx<TX> const &a, TX const& b){
  return a*(1/b);}
// complex + complex: add both parts:
template<typename TX>
static inline Cx<TX> operator + (Cx<TX> const & a,
Cx<TX> const & b) {
  return Cx<TX>(a.Re()+b.Re(),a.Im()+b.Im());}
// complex - complex: subtract both parts:
template<typename TX>
static inline Cx<TX> operator - (Cx<TX> const & a,
Cx<TX> const & b) {
  return Cx<TX>(a.Re()-b.Re(),a.Im()-b.Im());}
// complex * complex: (a.re*b.re-a.im*b.im, a.re*b.im+b.re*a.im).
template<typename TX>
static inline Cx<TX> operator * (Cx<TX> const & a,
Cx<TX> const & b) {
  return Cx<TX>(a.Re()*b.Re()-a.Im()*b.Im(), a.Re()*b.Im()+b.Re()*a.Im());
}
// complex / complex:
// (a.re*b.re+a.im*b.im, b.re*a.im-a.re*b.im)/(b.re*b.re+b.im*b.im).
template<typename TX>
static inline Cx<TX> operator / (Cx<TX> const & a,
Cx<TX> const & b) {
  return Cx<TX>((a.Re()*b.Re()+a.Im()*b.Im(), b.Re()*a.Im()-a.Re()*b.Im()))/(b.Re()*b.Re()+b.Im()*b.Im()) ;
}
// - complex: (-a.re, -a.im):
template<typename TX>
static inline Cx<TX> operator - (Cx<TX> const & a) {
  return Cx<TX>(-a.Re(), -a.Im()); // Change sign of both elements
}
// complex conjugate: (a.re, -a.im)
template<typename TX>
static inline Cx<TX> conj(Cx<TX> const & a) {
  return Cx<TX>(a.Re(),-a.Im()); // Change sign of imag. part
}
// abs2: (a.re*a.re + a.im*a.im)
template<typename TX>
static inline Cx<TX> abs2(Cx<TX> const & a) {
  return Cx<TX>(a.Re()*a.Re()+a.Im()*a.Im(),0.f); 
}
// normalisation complex
template<typename TX>
static inline Cx<TX> normcx(Cx<TX> const & a){
  return a*rsqrt(a.Re()*a.Re()+a.Im()*a.Im());
}

template<typename TX>
std::ostream& operator << (std::ostream& os,const Cx<TX>& cxval){
  cout<<"("<<c_blue<<(double)cxval.Re()<<c_normal<<","<<c_green<<(double)cxval.Im()<<c_normal<<")";
}

#endif
