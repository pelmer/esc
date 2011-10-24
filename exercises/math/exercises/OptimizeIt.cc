#include<cmath>
struct VD {
  double x() const {return m_x;}
  double y() const {return m_y;}
  double z() const {return m_z;}
  double mag() const { return sqrt(mag2());}
  double mag2() const { return  perp2() + z()*z();}
  double perp() const { return sqrt(perp2());} 
  double perp2() const { return x()*x()+y()*y();}

  double m_x, m_y, m_z;
};

void eloss(VD mom, double radLen, double & z, double & varz ) {
  // Energy loss and variance according to Bethe and Heitler, see also
  // Comp. Phys. Comm. 79 (1994) 157. 
  //
  double p = mom.mag();
  double normalisedPath = fabs(p/mom.z())*radLen;
  z = exp(-normalisedPath);
  varz = (exp(-normalisedPath*log(3.)/log(2.))- exp(-2*normalisedPath));

}


double r_x, r_y, s;
void sniplet(VD mom, double a_i, double d_x, double d_y) {
  double pt = mom.perp();  
  double j = a_i*(d_x * mom.x() + d_y * mom.y())/(pt*pt);      
  r_x = d_x - 2* mom.x()*(d_x*mom.x()+d_y*mom.y())/(pt*pt);         
  r_y = d_y - 2* mom.y()*(d_x*mom.x()+d_y*mom.y())/(pt*pt);         
  s = 1/(pt*pt*sqrt(1 - j*j));
}



