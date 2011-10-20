#ifndef FitClasses_H
#define FitClasses_H 1

typedef float Fvar_t;

#ifdef DOUBLE_PRECISION
#include "openlab_mod/P4_F64vec2.h"
typedef F64vec2 Fvec_t;
const int vecN = 2;
typedef double  Single_t;
#else
#ifdef X87
#include "openlab_mod/x87.h"
typedef double Fvec_t;
const int vecN = 1;
typedef double  Single_t;
#else
#include "openlab_mod/P4_F32vec4.h"
typedef F32vec4 Fvec_t;
const int vecN = 4;
typedef float  Single_t;
#endif /* x87 */
#endif /* DOUBLE_PRECISION */

//typedef float  Double_t;
//typedef double  Single_t;
typedef int    Int_t;

struct FieldVector{
  Fvec_t X, Y, Z;
  void Combine( FieldVector &H, Fvec_t w ){
    X+= w*(H.X-X);
    Y+= w*(H.Y-Y);
    Z+= w*(H.Z-Z);
  }
} __attribute__ ((aligned(16)));

//#include "CbmKF.h"

struct FieldSlice{
  Fvec_t X[10], Y[10], Z[10]; // polinom coeff.
  //Fvec_t z;
  FieldSlice(){ for( int i=0; i<10; i++ ) X[i]=Y[i]=Z[i]=0; }

  void GetField( const Fvec_t &x, const Fvec_t &y, Fvec_t &Hx, Fvec_t &Hy, Fvec_t &Hz ){

    Fvec_t x2 = x*x;
    Fvec_t y2 = y*y;
    Fvec_t xy = x*y;
    Fvec_t x3 = x2*x;
    Fvec_t y3 = y2*y;
    Fvec_t xy2 = x*y2;
    Fvec_t x2y = x2*y;
    //Fvec_t x4 = x3*x;
    //Fvec_t xy3 = x*y3;
    //Fvec_t x2y2 = x*xy2;
    //Fvec_t x3y = x*x2y;
    //Fvec_t y4 = y*y3;

    Hx = X[0] +X[1]*x +X[2]*y +X[3]*x2 +X[4]*xy +X[5]*y2 +X[6]*x3 +X[7]*x2y +X[8]*xy2 +X[9]*y3;
    //+ X[10]*x4 + X[11]*x3y +X[12]*x2y2 +X[13]*xy3 + X[14]*y4;
    Hy = Y[0] +Y[1]*x +Y[2]*y +Y[3]*x2 +Y[4]*xy +Y[5]*y2 +Y[6]*x3 +Y[7]*x2y +Y[8]*xy2 +Y[9]*y3;
    //+ Y[10]*x4 + Y[11]*x3y +Y[12]*x2y2 +Y[13]*xy3 + Y[14]*y4;
    Hz = Z[0] +Z[1]*x +Z[2]*y +Z[3]*x2 +Z[4]*xy +Z[5]*y2 +Z[6]*x3 +Z[7]*x2y +Z[8]*xy2 +Z[9]*y3;
    //+ Z[10]*x4 + Z[11]*x3y +Z[12]*x2y2 +Z[13]*xy3 + Z[14]*y4;
  }

  void GetField( const Fvec_t &x, const Fvec_t &y, FieldVector &H ){
    GetField( x, y, H.X, H.Y, H.Z );
  }
} __attribute__ ((aligned(16)));


struct FieldRegion{
  Fvec_t x0, x1, x2 ; // Hx(Z) = x0 + x1*(Z-z) + x2*(Z-z)^2
  Fvec_t y0, y1, y2 ; // Hy(Z) = y0 + y1*(Z-z) + y2*(Z-z)^2
  Fvec_t z0, z1, z2 ; // Hz(Z) = z0 + z1*(Z-z) + z2*(Z-z)^2
  Fvec_t z;

  FieldRegion(){
    x0 = x1 = x2 = y0 = y1 = y2 = z0 = z1 = z2 = z = 0.;
  }

  void Set( const FieldVector &H0, const Fvec_t &H0z, 
	    const FieldVector &H1, const Fvec_t &H1z, 
	    const FieldVector &H2, const Fvec_t &H2z){
    z = H0z;
    Fvec_t dz1 = H1z-H0z, dz2 = H2z-H0z;
    Fvec_t det = rcp(dz1*dz2*(dz2-dz1));
    Fvec_t w21 = -dz2*det;
    Fvec_t w22 = dz1*det;
    Fvec_t w11 = -dz2*w21;
    Fvec_t w12 = -dz1*w22;
    
    Fvec_t dH1 = H1.X - H0.X;
    Fvec_t dH2 = H2.X - H0.X;
    x0 = H0.X;
    x1 = dH1*w11 + dH2*w12 ;
    x2 = dH1*w21 + dH2*w22 ;
      
    dH1 = H1.Y - H0.Y;
    dH2 = H2.Y - H0.Y;
    y0 = H0.Y;
    y1 = dH1*w11 + dH2*w12 ;
    y2 = dH1*w21 + dH2*w22  ;

    dH1 = H1.Z - H0.Z;
    dH2 = H2.Z - H0.Z;
    z0 = H0.Z;
    z1 = dH1*w11 + dH2*w12 ;
    z2 = dH1*w21 + dH2*w22 ;         
  }
  
  void Shift( Fvec_t Z0){
    Fvec_t dz = Z0-z;
    Fvec_t x2dz = x2*dz;
    Fvec_t y2dz = y2*dz;
    Fvec_t z2dz = z2*dz;
    z = Z0;
    x0+= (x1 + x2dz)*dz;
    x1+= x2dz+x2dz; 
    y0+= (y1 + y2dz)*dz;
    y1+= y2dz+y2dz; 
    z0+= (z1 + z2dz)*dz;
    z1+= z2dz+z2dz; 
  }

} __attribute__ ((aligned(16)));


struct Station{
  Fvec_t z, thick, zhit, RL,  RadThick, logRadThick, 
    Sigma, Sigma2, Sy;
  FieldSlice Map;
  Station(){ 
    Sigma = 20.E-4; Sigma2 = Sigma*Sigma; 
  }
} __attribute__ ((aligned(16)));


struct Hit{
  Single_t x, y;
  Int_t ista;
  Single_t tmp1;
} __attribute__ ((aligned(16)));

struct MCTrack{
  Single_t MC_x, MC_y, MC_z, MC_px, MC_py, MC_pz, MC_q;
} __attribute__ ((aligned(16)));

struct Track{
  Int_t NHits;
  Hit vHits[12];
  Single_t T[6]; // x, y, tx, ty, qp, z  
  Single_t C[15]; // cov matr.
  Single_t Chi2;
  Int_t NDF;
} __attribute__ ((aligned(16)));

struct HitV{
  Fvec_t x, y, w;
  FieldVector H;
} __attribute__ ((aligned(16)));


struct CovV{

#ifdef SQRT_FILTER
  Fvec_t C00, C01, C02, C03, C04,
    C10, C11, C12, C13, C14,
    C20, C21, C22, C23, C24,
    C30, C31, C32, C33, C34,
    C40, C41, C42, C43, C44;
#else
  Fvec_t C00, 
    C10, C11, 
    C20, C21, C22, 
    C30, C31, C32, C33, 
    C40, C41, C42, C43, C44;
#endif
} __attribute__ ((aligned(16)));

struct TrackV{
  HitV vHits[12];
  Fvec_t T[6]; // x, y, tx, ty, qp, z  
  CovV   C;    // cov matr.
  Fvec_t Chi2;
  Fvec_t NDF;
} __attribute__ ((aligned(16)));

#endif
