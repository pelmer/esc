#include<cmath>
#include<cstdio>
#include<iostream>
#include <typeinfo>
\
// an "oriented" plane is defined by
// nx*(x-xp)+ny(y-yp)+nz*(z-zp)=0 (or in vector notation: n*(x-p) * here is the dot-product)
// for a generic point x,y,z this equation gives the distance of the point (with its sign) from the plane

// a line is represented by the "vector" equation x=xl+c*t (t is scalar and represent the path along the line from the "origin xl") 

struct Vect {
  Vect() : x(0),y(0),z(0){}
  Vect(float ix,float iy, float iz) : x(ix),y(iy),z(iz){}
  float x,y,z;
  void normalize() {
    float in = 1./sqrt(x*x+y*y+z*z);
    x*=in; y*=in; z*=in;
  }
};
typedef Vect Point;
inline
Vect operator+(Vect const & a,Vect const & b) { return Vect(a.x+b.x,a.y+b.y,a.z+b.z);}
inline
Vect operator-(Vect const & a,Vect const & b) { return Vect(a.x-b.x,a.y-b.y,a.z-b.z);}
inline
Vect operator*(float s,Vect const & a) { return Vect(s*a.x,s*a.y,s*a.z);}
inline
Vect operator*(Vect const & a,float s) { return Vect(s*a.x,s*a.y,s*a.z);}
inline
float dot(Vect const & a,Vect const & b) { return a.x*b.x+a.y*b.y+a.z*b.z;}

inline
Vect cross(Vect a,Vect const &b) {
  return Vect (a.y*b.z-a.z*b.y,
	       a.z*b.x-a.x*b.z,
	       a.x*b.y-a.y*b.x
	       );
}


struct Surface {
   virtual ~Surface(){}
};

struct Trajectory {
  // virtual ~Trajectory(){}
};


struct Plane : public Surface {
  Plane(): p(),n(){}
  Plane(Point ip, Vect in) : p(ip),n(in){}
  Point p;
  Vect n;
};

struct Line final : public Trajectory {
  Line(): p(),c(){}
  Line(Point const & ip, Vect const & ic) : p(ip),c(ic){}
  Point p;
  Vect c;

  Point go(float t) const { return p + c*t; }
};

// return t for the point of plane-line crossing
inline
float cross(Plane const & plane, Line const & line) {
  return dot(plane.n,plane.p-line.p)/dot(plane.n,line.c);
}

// return distance of a point from a plane
inline
float distance(Plane const & plane, Point const & point) {
  return dot(plane.n,point-plane.p);
}


struct Quadrangle : public Plane {
  Quadrangle() : Plane(), u(), v(), ul(0), vl(0){}
  Quadrangle(Point const & ip, Vect const & iu, Vect const & iv, float iul, float ivl) : 
    Plane(ip,cross(iu,iv)),u(iu),v(iv), ul(iul), vl(ivl){}
  Vect u, v;
  float ul, vl;
};

struct Rectangle final : public Quadrangle {
  template<typename ...Args>
  Rectangle(Args &&...args) :  Quadrangle(std::forward<Args>(args)...){}
};


struct BaseTrapezoid :  public Quadrangle {
  BaseTrapezoid() : Quadrangle(), alpha(0) {}
  BaseTrapezoid(Point const & ip, Vect const & iu, Vect const & iv, float ius,  float iul, float ivl) : 
    Quadrangle(ip,iu,iv, 0.5f*(ius+iul), ivl), alpha((iul-ius)/(ius+iul)) {}
  float alpha;
};

struct Trapezoid final : public BaseTrapezoid {
  template<typename ...Args>
  Trapezoid(Args &&...args) :  BaseTrapezoid(std::forward<Args>(args)...){}
};


// p is not the baricenter: is on the "straight" vertical side
struct LeftTrapezoid final : public BaseTrapezoid {
  template<typename ...Args>
  LeftTrapezoid(Args &&...args) :  BaseTrapezoid(std::forward<Args>(args)...){}
};

// p is not the baricenter: is on the "straight" vertical side
struct RightTrapezoid final : public BaseTrapezoid {
  template<typename ...Args>
  RightTrapezoid(Args &&...args) :  BaseTrapezoid(std::forward<Args>(args)...){}
};


inline 
void locCor(Quadrangle const & r, Point const & p, float & u, float & v) {
  Vect d = p-r.p;
  // printf("d= %e,%e,%e\n",d.x,d.y,d.z);
  u = dot(d,r.u);
  v = dot(d,r.v);
  // printf("u,v= %e,%e\n",u,v);
}

inline
int inside(Rectangle const & r, Point const & p) {
  float u,v;
  locCor(r,p,u,v);
  return  (r.ul>std::abs(u) ) &  ( r.vl>std::abs(v) ) ;
}


inline
int inside(Trapezoid const & r, Point const & p) {
  float u,v;
  locCor(r,p,u,v);
  float ul = r.ul + v * r.alpha;
  // printf("u,v= %e,%e\n",u,v);
  return  (ul>u) &  ( r.vl>std::abs(v) ) ;
}

inline
int inside(LeftTrapezoid const & r, Point const & p) {
  float u,v;
  locCor(r,p,u,v);
  float ul = r.ul + v * r.alpha;
  // printf("u,v= %e,%e\n",u,v);
  return  (v>0.f ) & (ul>u  ) &  ( (-r.vl)<v ) ;
}

inline
int inside(RightTrapezoid const & r, Point const & p) {
  float u,v;
  locCor(r,p,u,v);
  float ul = r.ul + v * r.alpha;
  // printf("u,v= %e,%e\n",u,v);
  return  (u<0.f) & (ul>u ) &  ( r.vl>v ) ;
}



/*
struct Vects {
  static constexpr int nValues=3;
  Vects() : x(0),y(0),z(0),m_size(0){}
  Vects(int s, float * mem) : x(mem),y(x+s),z(y+s),m_size(s){}
  float *x, *y, *z;
  int m_size;
  
  Vect operator[](int i) const {
    return Vect(x[i],y[i],z[i]);
  } 
  int size() const { return m_size;}
  
  void set(Vect v,int i) {
    x[i]=v.x; y[i]=v.y; z[i]=v.z;
  }
  
};
*/


// less aliasing??
struct Vects {
  static constexpr int nValues=3;
  Vects() : mem(nullptr),m_size(0){}
  Vects(int s, float * imem) : mem(imem),m_size(s){}
  float * __restrict__ mem;
  int m_size;
  
  float & x(int i) { return mem[i];}
  float & y(int i) { return mem[i+size()];}
  float & z(int i) { return mem[i+size()+size()];}

  float x(int i) const { return mem[i];}
  float y(int i) const { return mem[i+size()];}
  float z(int i) const { return mem[i+size()+size()];}


  Vect operator[](int i) const {
    return Vect(x(i),y(i),z(i));
  } 
  int size() const { return m_size;}
  
  void set(Vect const & v,int i) {
    x(i)=v.x; y(i)=v.y; z(i)=v.z;
  }
  
};



typedef Vects Points;

struct Lines {
  static constexpr int nValues=2*Vects::nValues;
  Lines(){}
  Lines(int s, float * mem):p(s,mem),c(s,mem){}
  Vects p, c;

  Line operator[](int i) const { return Line(p[i],c[i]);}
  int size() const { return p.size();  }
  void set(Line const & v,int i) { p.set(v.p,i);c.set(v.c,i);}
  void setPoint(Point const & ip, int i) { p.set(ip,i);}

};

inline
  Plane makePlane(float offr=0) {
  // a generic plane
  double nx = -7./13., ny=3./7., nz=17./23.; 
  double nor = 1./sqrt(nx*nx+ny*ny+nz*nz); 
  return Plane({float(3./7.+offr),float(4./7.+offr),13./17.},{float(nx*nor),float(ny*nor),float(nz*nor)});
}

inline
Rectangle makeRectangle(float offr=0) {
  // a generic Rectangle
  double nx = -7./13., ny=3./7., nz=17./23.; 
  double nor = 1./sqrt(nx*nx+ny*ny+nz*nz);
  Vect n = {float(nx*nor),float(ny*nor),float(nz*nor)};
  nor = 1./sqrt(nx*nx+ny*ny);
  Vect u = {float(ny*nor),float(-nx*nor),0};
  Vect v = cross(n,u);
  Point p = {float(3./7.+offr),float(4./7.+offr),13./17.};
  return Rectangle(p,u,v,27./17.,15./17.);
}

inline
LeftTrapezoid makeLeftTrapezoid(float offr=0) {
  // a generic Rectangle
  double nx = -7./13., ny=3./7., nz=17./23.; 
  double nor = 1./sqrt(nx*nx+ny*ny+nz*nz);
  Vect n = {float(nx*nor),float(ny*nor),float(nz*nor)};
  nor = 1./sqrt(nx*nx+ny*ny);
  Vect u = {float(ny*nor),float(-nx*nor),0};
  Vect v = cross(n,u);
  Point p = {float(3./7.+offr),float(4./7.+offr),13./17.};
  return LeftTrapezoid(p,u,v,21./17.,27./17., 15./17.);
}


inline
Line makeLine(float r, float z) {
  // a generic line (almost parallel to the plane...)
  double cy = -7./13.+r, cx=3./7.-r, cz=17./23.+z; 
  double nor = 1./sqrt(cx*cx+cy*cy+cz*cz); 
  return Line({3.3/7.,4.4/7.,13./17.},{float(cx*nor),float(cy*nor),float(cz*nor)});
}

inline
void makeLines(Lines & lines) {
  float epsr = 0.5/13., epsz=1./23.;
  float r=0, z=0;
  int n = lines.size();
  for (int i=0; i!=n; ++i) {
    r+=epsr;
    z += (i<n/2) ? epsz : -epsz;
    lines.set(makeLine(r,z),i);
  }
}



void testCross() {
  Plane pl = makePlane();
  Line  ln = makeLine(0,0);

  float d0 = distance(pl,ln.p);

  printf("d0 = %e, %a\n",d0,d0);
  float t = cross(pl,ln);
  printf("t  = %e, %a\n",t,t);
  Point x1 = ln.go(t);
  float d1 = distance(pl,x1);
  printf("d1 = %e, %a\n",d1,d1);
  t = nextafterf(t,copysignf(999999.f,t));
  printf("t+ = %e, %a\n",t,t);
  Point x2 = ln.go(t);
  float d2 = distance(pl,x2);
  printf("d2 = %e, %a\n",d2,d2);
  printf("\n");

}

void testInside() {
  Rectangle r = makeRectangle();
  bool ic = inside(r,r.p+0.9*r.ul*r.u-0.5*r.vl*r.v);
  std::cout << (ic ? "inside" : "outside") << std::endl;
  ic = inside(r,r.p+1.2*r.ul*r.u-0.5*r.vl*r.v);
  std::cout << (ic ? "inside" : "outside") << std::endl;
  printf("\n");
}


void loopCross() {
  int N=256;
  float arena[N*Lines::nValues];
  Lines lines(N,arena);
  makeLines(lines);

  Plane pl = makePlane();
  int n = lines.size();
  float d2[n];
  for (int i=0; i!=n; ++i) {
    float t = cross(pl,lines[i]);
    Point x2 = lines[i].go(t);
    d2[i] = distance(pl,x2);
  }

  printf("\n");
  for (int i=0; i!=n; ++i)
    printf("%e,%a ",d2[i],d2[i]);
  printf("\n");
}



template<typename S>
inline
void loopInsideKernel(S const & r) {
  int N=256;
  float arena[N*Lines::nValues];
  Lines lines(N,arena);
  makeLines(lines);

  int n = lines.size();
  int in[n];
  for (int i=0; i!=n; ++i) {
    float t = cross(r,lines[i]);
    Point x2 = lines[i].go(t);
    in[i] = inside(r,x2);
    x2 = in[i] ? x2 : lines[i].p;
    // if (in[i]) 
    lines.setPoint(x2,i);
  }

  printf("\n");
  std::cout << typeid(S).name() << std::endl;
  for (int i=0; i!=n; ++i)
    std::cout << ( in[i] ? 'i' : 'o');
  std::cout << std::endl;
  for (int i=0; i!=n; ++i)
    std::cout <<  lines[i].p.x <<",";
  std::cout << std::endl;
}


void loopInsideR() {
  Rectangle r = makeRectangle(0.01f);
  loopInsideKernel(r);
}

void loopInsideLT() {
  LeftTrapezoid r = makeLeftTrapezoid(0.01f);
  loopInsideKernel(r);
}

int main() {
  testCross();
  testInside();

  loopCross();
  loopInsideR();

  loopInsideLT();
}
