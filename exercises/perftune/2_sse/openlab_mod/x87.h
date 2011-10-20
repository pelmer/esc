double rcp(double a){return 1./a;}

double rsqrt( double a ){ return 1. / sqrt(a); }

//class X87_double{
//
// public:
//  double value;
//
//  X87_double( ){}
//  X87_double( const double &a ) { value = a; }
//  X87_double(long a){value = (double)a;}
//
//  /* Conversion function */
//  operator  double() const { return value; }		/* Convert to double */
//  operator  long() const { return (long)value; }		/* Convert to double */
//
//  /* Logical */
// 
//  friend X87_double operator&( const X87_double &a, const X87_double &b ){ // mask returned
//    return (X87_double)((long)a & (long)b);
//  }
//
//}
//
