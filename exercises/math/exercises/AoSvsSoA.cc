// typedef __complex__ double Value;
// typedef double Value;

// typedef __complex__ float Value;
typedef float Value;

struct LorentzVector {

  LorentzVector(Value x=0, Value  y=0, Value  z=0, Value  t=0) : theX(x),theY(y),theZ(z),theT(t){} 
  LorentzVector & operator+=(const LorentzVector & a) {
    theX += a.theX;
    theY += a.theY;
    theZ += a.theZ;
    theT += a.theT;
    return *this;
  }
  
  Value theX;
  Value theY;
  Value theZ;
  Value theT;
}  __attribute__ ((aligned(16)));

inline
int operator==(LorentzVector const & a, LorentzVector const & b) {
  int ret[4];
  ret[0] = a.theX==b.theX;ret[1] = a.theY==b.theY;ret[2] = a.theZ==b.theZ;ret[3] = a.theT==b.theT;
  return ret[0]&ret[1]&ret[2]&ret[3];
  //  return a.theX==b.theX & a.theY==b.theY & a.theZ==b.theZ & a.theT==b.theT;
}

inline LorentzVector
operator+(LorentzVector const & a, LorentzVector const & b) {
  return LorentzVector(a.theX+b.theX,a.theY+b.theY,a.theZ+b.theZ,a.theT+b.theT);
}

inline LorentzVector
operator-(LorentzVector const & a, LorentzVector const & b) {
  return LorentzVector(a.theX-b.theX,a.theY-b.theY,a.theZ-b.theZ,a.theT-b.theT);
}

inline LorentzVector
operator*(LorentzVector const & a, Value s) {
    return LorentzVector(a.theX*s,a.theY*s,a.theZ*s,a.theT*s);
}

inline LorentzVector
operator*(Value s, LorentzVector const & a) {
  return a*s;
}

inline
Value dot(LorentzVector const & a, LorentzVector const & b) {
  //  LorentzVector p(a.theX*b.theX,a.theY*b.theY,a.theZ*b.theZ,a.theT*b.theT);
  //return p.theX+p.theY+p.theZ-p.theT;
  return a.theX*b.theX+a.theY*b.theY+a.theZ*b.theZ-a.theT*b.theT;
}

Value hsum(LorentzVector const &p) {
  return p.theX+p.theY+p.theZ+p.theT;
}

//-----

void sum1(LorentzVector & res, Value s, LorentzVector const & v1, LorentzVector const & v2) {
  res += s*(v1+v2);
}


inline
LorentzVector __attribute__((always_inline)) ksum(Value s, LorentzVector const & v1, LorentzVector const & v2) {
  return s*(v1+v2) + s*(v1-2.f*v2) ;
}

void msum(LorentzVector & res, Value s, LorentzVector const & v1, LorentzVector const & v2) {
  res = s*(v1+v2) + s*(v1-2.f*v2) ;
}


struct Particle {
  Particle(){}
  Particle(LorentzVector const & iv, LorentzVector const & ip, float ic, float it) : v(iv),p(ip), charge(ic),type(it){}
  LorentzVector v;
  LorentzVector p;
  float charge;
  float type;

};

namespace aos {
  LorentzVector a[1024], b[1014], c[1024];
  void fill(LorentzVector * v, float * f, int N) {
    for (int i=0; i!=N;++i)
      v[i] = LorentzVector(f[i],f[i+N],f[i+2*N],f[i+3*N]);
  }
  Value s;
  Value m[1024];
  void lsum(bool p) {
    for (int i=0; i!=1024;++i)
      a[i] = ksum(s,b[i],c[i]);
    if (p) for (int i=0; i!=1024;++i)
      m[i] =dot(a[i],b[i]);
  }
}

namespace aosP {
  Particle a[1024], b[1014], c[1024];
  void fill(Particle * v, float * f, int N) {
    for (int i=0; i!=N;++i){
      v[i].p = LorentzVector(f[i],f[i+N],f[i+2*N],f[i+3*N]);
      v[i].charge = (0==i%10) ? 1.f : 0.f;
    }
  }
  Value s;
  Value m[1024];
  void lsum(bool p) {
    for (int i=0; i!=1024;++i)
       a[i].p = (0==a[i].charge) ?
	 b[i].p : ksum(s,b[i].p,c[i].p);
    if (p) for (int i=0; i!=1024;++i)
	     m[i] =dot(a[i].p,b[i].p);
  }
}



//------------  SOA -----------

template<typename SOA>
struct Setter {
  SOA & soa;
  int i;
  Setter( SOA const & isoa, int ii) : soa(const_cast<SOA&>(isoa)), i(ii){}

  template<typename S>
  void operator=(S const & s) {
    soa.set(s,i);
  }
  template<typename S>
  operator S() const { return soa.get(i);}
};

struct SoA4 {
  SoA4(Value * __restrict__ im=0, int in=0) : mem(im),n(in){}
  typedef Setter<SoA4> Set;
  Value * __restrict__  mem;
  int n;
  Value x(int i) const  { return mem[i];} 
  Value y(int i) const { return  mem[i+n];} 
  Value z(int i) const { return  mem[i+2*n];} 
  Value t(int i) const { return  mem[i+3*n];} 
  Value & x(int i) { return mem[i];} 
  Value & y(int i) { return  mem[i+n];} 
  Value & z(int i) { return  mem[i+2*n];} 
  Value & t(int i) { return  mem[i+3*n];} 
  /*
  LorentzVector operator[](int i) const {
    return LorentzVector(x(i),y(i),z(i),t(i));
  }
  */
  Set operator[](int i) {
    return Set(*this,i);
  }
  Set operator[](int i) const {
    return Set(*this,i);
  }

  LorentzVector get(int i) const {
    return LorentzVector(x(i),y(i),z(i),t(i));
  }

  void set(LorentzVector const & v, int i) {
    x(i)=v.theX;y(i)=v.theY;z(i)=v.theZ;t(i)=v.theT;
  }
};

struct Particles {
  Particles(Value * __restrict__ im, int in) : v(im,in),p(im+4*in,in){}
  typedef Setter<Particles> Set;
  SoA4 v;
  SoA4 p;
  float charge(int i) const { return p.mem[i+4*p.n]; }
  float type(int i) const { return p.mem[i+5*p.n]; }
  float & charge(int i) { return p.mem[i+4*p.n]; }
  float & type(int i)  { return p.mem[i+5*p.n]; }
 
  int size() const { return v.n;}

  /*
  Set operator[](int i) {
    return Set(*this,i);
  }
  */
  Particle operator[](int i) const {
    return Set(*this,i);
  }

  Particle get(int i) const {
    return Particle(v[i],p[i],charge(i),type(i));
  }
  void set(Particle const & ip, int i) {
    v.set(ip.v,i); p.set(ip.p,i); charge(i)=ip.charge; type(i)=ip.type;
  }
  void setP(LorentzVector const & ip, int i) {
    p.set(ip,i);
  }
  LorentzVector getP(int i) const {
    return p[i];
  }
};

struct SoA3 {
  Value * __restrict__ mem;
  int n;
  Value x(int i) const  { return mem[i];} 
  Value y(int i) const { return  mem[i+n];} 
  Value z(int i) const { return  mem[i+2*n];} 

  Value & x(int i) { return mem[i];} 
  Value & y(int i) { return  mem[i+n];} 
  Value & z(int i) { return  mem[i+2*n];} 

  LorentzVector operator[](int i) const {
    return LorentzVector(x(i),y(i),z(i));
  }
  void set(LorentzVector const & v, int i) {
    x(i)=v.theX;y(i)=v.theY;z(i)=v.theZ;
  }
};

namespace soa4 {

  int N=1024;
  Value arena[3*4*1024];
  Value m1[4*1024],m2[4*1024],m3[4*1024];
  SoA4 a,b,c; 
  Value s;
  Value m[1024];
  void soAsum(bool p) {
    //a.mem=arena; b.mem=arena+4*1024;c.mem=b.mem+4*1024;
    a.mem=m1; b.mem=m2;c.mem=m3;
    a.n=b.n=c.n=1024;
    for (int i=0; i!=1024;++i)
      a[i] = ksum(s,b[i],c[i]);
    if (p) for (int i=0; i!=1024;++i)
      m[i] =dot(a[i],b[i]);
  }
}

namespace soaP {

  int N=1024;
  Value arena[3*4*1024];
  Value m1[2*4*1024],m2[2*4*1024],m3[2*4*1024];
  Particles a(m1,1024), b(m2,1024),c(m3,1024); 
  Value s;
  Value m[1024];
  void soAsum(bool p) {
    //a.mem=arena; b.mem=arena+4*1024;c.mem=b.mem+4*1024;
    for (int i=0; i!=1024;++i) {
      LorentzVector v  =  b[i].p;
      if (0!=a.charge(i)) 
	v= ksum(s,b[i].p,c[i].p);
      a.setP(v,i);
    }
    if (p) for (int i=0; i!=1024;++i)
	     m[i] =dot(a[i].p,b[i].p);
  }
}

namespace soa3 {

  int N=1024;
  Value arena[3*3*1024];
  Value m1[3*1024],m2[3*1024],m3[3*1024];
  SoA3 a,b,c; 
  Value s;
  Value m[1024];
  void soAsum(bool p) {
    // a.mem=arena; b.mem=arena+4*1024;c.mem=b.mem+4*1024;
    a.mem=m1; b.mem=m2;c.mem=m3;
    a.n=b.n=c.n=1024;
    for (int i=0; i!=1024;++i)
      a.set(ksum(s,b[i],c[i]),i);
    if (p) for (int i=0; i!=1024;++i)
      m[i] =dot(a[i],b[i]);
  }
}
