#ifndef Fit_H
#define Fit_H

#include <math.h>
#include "FitClasses.h"

#define cnst static const Fvec_t 


cnst INF = .01, ZERO = 0.0, ONE = 1.;
cnst c_light = 0.000299792458, c_light_i = 1./c_light;


inline void ExtrapolateALight
( 
 Fvec_t T [], // input track parameters (x,y,tx,ty,Q/p)
 CovV &C,     // input covariance matrix
 Fvec_t       z_out  , // extrapolate to this z position
 Fvec_t       &qp0    , // use Q/p linearisation at this value
 FieldRegion &F
 )
{
  //
  //  Part of the analytic extrapolation formula with error (c_light*B*dz)^4/4!
  //  
  
  cnst  
    c1 = 1., c2 = 2., c3 = 3., c4 = 4., c6 = 6., c9 = 9., c15 = 15., c18 = 18., c45 = 45.,
    c2i = 1./2., c3i = 1./3., c6i = 1./6., c12i = 1./12.;

  Fvec_t qp = T[4];
  Fvec_t dz = (z_out - T[5]);
  Fvec_t dz2 = dz*dz;
  Fvec_t dz3 = dz2*dz;

  Fvec_t T0 = T[0];
  Fvec_t T1 = T[1];
  Fvec_t T2 = T[2];
  Fvec_t T3 = T[3];
  Fvec_t T4 = T[4];
  Fvec_t T5 = T[5];

  // construct coefficients 

  Fvec_t x   = T[2]; // tx !!
  Fvec_t y   = T[3]; // ty !!
  Fvec_t xx  = x*x;
  Fvec_t xy = x*y;
  Fvec_t yy = y*y;
  Fvec_t y2 = y*c2;
  Fvec_t x2 = x*c2;
  Fvec_t x4 = x*c4;
  Fvec_t xx31 = xx*c3+c1;
  Fvec_t xx159 = xx*c15+c9;

  Fvec_t Ay = -xx-c1;
  Fvec_t Ayy = x*(xx*c3+c3);
  Fvec_t Ayz = -c2*xy; 
  Fvec_t Ayyy = -(c15*xx*xx+c18*xx+c3);

  Fvec_t Ayy_x = c3*xx31;
  Fvec_t Ayyy_x = -x4*xx159;

  Fvec_t Bx = yy+c1; 
  Fvec_t Byy = y*xx31; 
  Fvec_t Byz = c2*xx+c1;
  Fvec_t Byyy = -xy*xx159;

  Fvec_t Byy_x = c6*xy;
  Fvec_t Byyy_x = -y*(c45*xx+c9);
  Fvec_t Byyy_y = -x*xx159;

  // end of coefficients calculation

  Fvec_t t2   = c1 + xx + yy;
  Fvec_t t    = sqrt( t2 );
  Fvec_t h    = qp0*c_light;
  Fvec_t ht   = h*t;

  // get field integrals
  Fvec_t ddz = T5-F.z;
  Fvec_t Fx0 = F.x0 + F.x1*ddz + F.x2*ddz*ddz;
  Fvec_t Fx1 = (F.x1 + c2*F.x2*ddz)*dz;
  Fvec_t Fx2 = F.x2*dz2;
  Fvec_t Fy0 = F.y0 + F.y1*ddz + F.y2*ddz*ddz;
  Fvec_t Fy1 = (F.y1 + c2*F.y2*ddz)*dz;
  Fvec_t Fy2 = F.y2*dz2;
  Fvec_t Fz0 = F.z0 + F.z1*ddz + F.z2*ddz*ddz;
  Fvec_t Fz1 = (F.z1 + c2*F.z2*ddz)*dz;
  Fvec_t Fz2 = F.z2*dz2;

  // 

  Fvec_t sx = ( Fx0 + Fx1*c2i + Fx2*c3i );
  Fvec_t sy = ( Fy0 + Fy1*c2i + Fy2*c3i );
  Fvec_t sz = ( Fz0 + Fz1*c2i + Fz2*c3i );

  Fvec_t Sx = ( Fx0*c2i + Fx1*c6i + Fx2*c12i );
  Fvec_t Sy = ( Fy0*c2i + Fy1*c6i + Fy2*c12i );
  Fvec_t Sz = ( Fz0*c2i + Fz1*c6i + Fz2*c12i );

  Fvec_t syz;
  { 
    cnst 
      d = 1./360., 
      c00 = 30.*6.*d, c01 = 30.*2.*d,   c02 = 30.*d,
      c10 = 3.*40.*d, c11 = 3.*15.*d,   c12 = 3.*8.*d, 
      c20 = 2.*45.*d, c21 = 2.*2.*9.*d, c22 = 2.*2.*5.*d;
    syz = Fy0*( c00*Fz0 + c01*Fz1 + c02*Fz2) 
      +   Fy1*( c10*Fz0 + c11*Fz1 + c12*Fz2) 
      +   Fy2*( c20*Fz0 + c21*Fz1 + c22*Fz2) ;
  }

  Fvec_t Syz;
  {
    cnst 
      d = 1./2520., 
      c00 = 21.*20.*d, c01 = 21.*5.*d, c02 = 21.*2.*d,
      c10 =  7.*30.*d, c11 =  7.*9.*d, c12 =  7.*4.*d, 
      c20 =  2.*63.*d, c21 = 2.*21.*d, c22 = 2.*10.*d;
    Syz = Fy0*( c00*Fz0 + c01*Fz1 + c02*Fz2 ) 
      +   Fy1*( c10*Fz0 + c11*Fz1 + c12*Fz2 ) 
      +   Fy2*( c20*Fz0 + c21*Fz1 + c22*Fz2 ) ;
  }

  Fvec_t syy  = sy*sy*c2i;
  Fvec_t syyy = syy*sy*c3i;

  Fvec_t Syy ;   
  {
    cnst  
    d= 1./2520., c00= 420.*d, c01= 21.*15.*d, c02= 21.*8.*d,
    c03= 63.*d, c04= 70.*d, c05= 20.*d;
    Syy =  Fy0*(c00*Fy0+c01*Fy1+c02*Fy2) + Fy1*(c03*Fy1+c04*Fy2) + c05*Fy2*Fy2 ;
  }
  
  Fvec_t Syyy;
  {
    cnst 
      d = 1./181440., 
      c000 =   7560*d, c001 = 9*1008*d, c002 = 5*1008*d, 
      c011 = 21*180*d, c012 = 24*180*d, c022 =  7*180*d, 
      c111 =    540*d, c112 =    945*d, c122 =    560*d, c222 = 112*d;
    Fvec_t Fy22 = Fy2*Fy2;
    Syyy = Fy0*( Fy0*(c000*Fy0+c001*Fy1+c002*Fy2)+ Fy1*(c011*Fy1+c012*Fy2)+c022*Fy22 )
      +    Fy1*( Fy1*(c111*Fy1+c112*Fy2)+c122*Fy22) + c222*Fy22*Fy2                  ;
  }
  
  
  Fvec_t sA1   = sx*xy   + sy*Ay   + sz*y ;
  Fvec_t sA1_x = sx*y - sy*x2 ;
  Fvec_t sA1_y = sx*x + sz ;

  Fvec_t sB1   = sx*Bx   - sy*xy   - sz*x ;
  Fvec_t sB1_x = -sy*y - sz ;
  Fvec_t sB1_y = sx*y2 - sy*x ;

  Fvec_t SA1   = Sx*xy   + Sy*Ay   + Sz*y ;
  Fvec_t SA1_x = Sx*y - Sy*x2 ;
  Fvec_t SA1_y = Sx*x + Sz;

  Fvec_t SB1   = Sx*Bx   - Sy*xy   - Sz*x ;
  Fvec_t SB1_x = -Sy*y - Sz;
  Fvec_t SB1_y = Sx*y2 - Sy*x;


  Fvec_t sA2   = syy*Ayy   + syz*Ayz ;
  Fvec_t sA2_x = syy*Ayy_x - syz*y2 ;
  Fvec_t sA2_y = -syz*x2 ;
  Fvec_t sB2   = syy*Byy   + syz*Byz  ;
  Fvec_t sB2_x = syy*Byy_x + syz*x4 ;
  Fvec_t sB2_y = syy*xx31 ;
  
  Fvec_t SA2   = Syy*Ayy   + Syz*Ayz ;
  Fvec_t SA2_x = Syy*Ayy_x - Syz*y2 ;
  Fvec_t SA2_y = -Syz*x2 ;
  Fvec_t SB2   = Syy*Byy   + Syz*Byz ;
  Fvec_t SB2_x = Syy*Byy_x + Syz*x4 ;
  Fvec_t SB2_y = Syy*xx31 ;

  Fvec_t sA3   = syyy*Ayyy  ;
  Fvec_t sA3_x = syyy*Ayyy_x;
  Fvec_t sB3   = syyy*Byyy  ;
  Fvec_t sB3_x = syyy*Byyy_x;
  Fvec_t sB3_y = syyy*Byyy_y;

 
  Fvec_t SA3   = Syyy*Ayyy  ;
  Fvec_t SA3_x = Syyy*Ayyy_x;
  Fvec_t SB3   = Syyy*Byyy  ;
  Fvec_t SB3_x = Syyy*Byyy_x;
  Fvec_t SB3_y = Syyy*Byyy_y;

  Fvec_t ht1 = ht*dz;
  Fvec_t ht2 = ht*ht*dz2;
  Fvec_t ht3 = ht*ht*ht*dz3;
  Fvec_t ht1sA1 = ht1*sA1;
  Fvec_t ht1sB1 = ht1*sB1;
  Fvec_t ht1SA1 = ht1*SA1;
  Fvec_t ht1SB1 = ht1*SB1;
  Fvec_t ht2sA2 = ht2*sA2;
  Fvec_t ht2SA2 = ht2*SA2;
  Fvec_t ht2sB2 = ht2*sB2;
  Fvec_t ht2SB2 = ht2*SB2;
  Fvec_t ht3sA3 = ht3*sA3;
  Fvec_t ht3sB3 = ht3*sB3; 
  Fvec_t ht3SA3 = ht3*SA3;
  Fvec_t ht3SB3 = ht3*SB3;

  T[0] = T0 + (x + ht1SA1 + ht2SA2 + ht3SA3)*dz ;
  T[1] = T1 + (y + ht1SB1 + ht2SB2 + ht3SB3)*dz ;
  T[2] = T2 + ht1sA1 + ht2sA2 + ht3sA3;
  T[3] = T3 + ht1sB1 + ht2sB2 + ht3sB3;
  T[5]+= dz;
  
  Fvec_t ctdz  = c_light*t*dz;
  Fvec_t ctdz2 = c_light*t*dz2;

  Fvec_t dqp = qp - qp0;
  Fvec_t t2i = c1*rcp(t2);// /t2;
  Fvec_t xt2i = x*t2i;
  Fvec_t yt2i = y*t2i;
  Fvec_t tmp0 = ht1SA1 + c2*ht2SA2 + c3*ht3SA3;
  Fvec_t tmp1 = ht1SB1 + c2*ht2SB2 + c3*ht3SB3;
  Fvec_t tmp2 = ht1sA1 + c2*ht2sA2 + c3*ht3sA3;
  Fvec_t tmp3 = ht1sB1 + c2*ht2sB2 + c3*ht3sB3;

  Fvec_t j02 = dz*(c1 + xt2i*tmp0 + ht1*SA1_x + ht2*SA2_x + ht3*SA3_x);
  Fvec_t j12 = dz*(     xt2i*tmp1 + ht1*SB1_x + ht2*SB2_x + ht3*SB3_x);
  Fvec_t j22 =     c1 + xt2i*tmp2 + ht1*sA1_x + ht2*sA2_x + ht3*sA3_x ;
  Fvec_t j32 =          xt2i*tmp3 + ht1*sB1_x + ht2*sB2_x + ht3*sB3_x ;
    
  Fvec_t j03 = dz*(     yt2i*tmp0 + ht1*SA1_y + ht2*SA2_y );
  Fvec_t j13 = dz*(c1 + yt2i*tmp1 + ht1*SB1_y + ht2*SB2_y + ht3*SB3_y );
  Fvec_t j23 =          yt2i*tmp2 + ht1*sA1_y + ht2*sA2_y  ;
  Fvec_t j33 =     c1 + yt2i*tmp3 + ht1*sB1_y + ht2*sB2_y + ht3*sB3_y ;
    
  Fvec_t j04 = ctdz2*( SA1 + c2*ht1*SA2 + c3*ht2*SA3 );
  Fvec_t j14 = ctdz2*( SB1 + c2*ht1*SB2 + c3*ht2*SB3 );
  Fvec_t j24 = ctdz *( sA1 + c2*ht1*sA2 + c3*ht2*sA3 );
  Fvec_t j34 = ctdz *( sB1 + c2*ht1*sB2 + c3*ht2*sB3 );
  
  // extrapolate inverse momentum
  
  T[0]+=j04*dqp;
  T[1]+=j14*dqp;
  T[2]+=j24*dqp;
  T[3]+=j34*dqp;

  //          covariance matrix transport 
 
  Fvec_t c42 = C.C42, c43 = C.C43;

  Fvec_t cj00 = C.C00 + C.C20*j02 + C.C30*j03 + C.C40*j04;
  Fvec_t cj10 = C.C10 + C.C21*j02 + C.C31*j03 + C.C41*j04;
  Fvec_t cj20 = C.C20 + C.C22*j02 + C.C32*j03 + c42*j04;
  Fvec_t cj30 = C.C30 + C.C32*j02 + C.C33*j03 + c43*j04;
 
  Fvec_t cj01 = C.C10 + C.C20*j12 + C.C30*j13 + C.C40*j14;
  Fvec_t cj11 = C.C11 + C.C21*j12 + C.C31*j13 + C.C41*j14;
  Fvec_t cj21 = C.C21 + C.C22*j12 + C.C32*j13 + c42*j14;
  Fvec_t cj31 = C.C31 + C.C32*j12 + C.C33*j13 + c43*j14;
 
  Fvec_t cj02 = C.C20*j22 + C.C30*j23 + C.C40*j24;
  Fvec_t cj12 = C.C21*j22 + C.C31*j23 + C.C41*j24;
  Fvec_t cj22 = C.C22*j22 + C.C32*j23 + c42*j24;
  Fvec_t cj32 = C.C32*j22 + C.C33*j23 + c43*j24;
 
  Fvec_t cj03 = C.C20*j32 + C.C30*j33 + C.C40*j34;
  Fvec_t cj13 = C.C21*j32 + C.C31*j33 + C.C41*j34;
  Fvec_t cj23 = C.C22*j32 + C.C32*j33 + c42*j34;
  Fvec_t cj33 = C.C32*j32 + C.C33*j33 + c43*j34;

  C.C40+= c42*j02 + c43*j03 + C.C44*j04; // cj40
  C.C41+= c42*j12 + c43*j13 + C.C44*j14; // cj41
  C.C42 = c42*j22 + c43*j23 + C.C44*j24; // cj42
  C.C43 = c42*j32 + c43*j33 + C.C44*j34; // cj43

  C.C00 = cj00 + j02*cj20 + j03*cj30 + j04*C.C40;
  C.C10 = cj01 + j02*cj21 + j03*cj31 + j04*C.C41;
  C.C11 = cj11 + j12*cj21 + j13*cj31 + j14*C.C41;

  C.C20 = j22*cj20 + j23*cj30 + j24*C.C40 ;
  C.C30 = j32*cj20 + j33*cj30 + j34*C.C40 ;
  C.C21 = j22*cj21 + j23*cj31 + j24*C.C41 ;
  C.C31 = j32*cj21 + j33*cj31 + j34*C.C41 ;
  C.C22 = j22*cj22 + j23*cj32 + j24*C.C42 ;
  C.C32 = j32*cj22 + j33*cj32 + j34*C.C42 ;
  C.C33 = j32*cj23 + j33*cj33 + j34*C.C43 ;

}

struct HitInfo{
  Fvec_t cos_phi, sin_phi, sigma2, sigma216;
};

inline void Filter( TrackV &track, HitInfo &info, Fvec_t &u, Fvec_t &w )
{

  // convert input
  Fvec_t *T = track.T;
  CovV &C = track.C;

  register Fvec_t wi, zeta, zetawi, HCH;
  
  register Fvec_t F0, F1, F2, F3, F4;
  register Fvec_t  K1, K2, K3, K4;
  
  zeta = info.cos_phi*T[0] + info.sin_phi*T[1] - u;
  // F = CH'
  F0 = info.cos_phi*C.C00 + info.sin_phi*C.C10;
  F1 = info.cos_phi*C.C10 + info.sin_phi*C.C11;
  
  HCH = ( F0*info.cos_phi +F1*info.sin_phi );
  Fvec_t initialised = ( HCH<info.sigma216 );
  
  F2 = info.cos_phi*C.C20 + info.sin_phi*C.C21;
  F3 = info.cos_phi*C.C30 + info.sin_phi*C.C31;
  F4 = info.cos_phi*C.C40 + info.sin_phi*C.C41;
  
  wi = w*rcp(info.sigma2 +HCH);
#ifdef X87
  zetawi = w* zeta *rcp( (double)((long long)initialised & (long long)info.sigma2) + HCH);
  track.Chi2 += (double)((long long)initialised & (long long)(zeta * zetawi));    
#else
  zetawi = w* zeta *rcp( (initialised&info.sigma2) + HCH);
  track.Chi2 += initialised & (zeta * zetawi);    
#endif
  
  track.NDF  += w;

  K1 = F1*wi;
  K2 = F2*wi;
  K3 = F3*wi;
  K4 = F4*wi;

  T[0]-= F0*zetawi;
  T[1]-= F1*zetawi;
  T[2]-= F2*zetawi;
  T[3]-= F3*zetawi;
  T[4]-= F4*zetawi;

  C.C00-= F0*F0*wi;
  C.C10-= K1*F0;
  C.C11-= K1*F1;
  C.C20-= K2*F0;
  C.C21-= K2*F1;
  C.C22-= K2*F2;
  C.C30-= K3*F0;
  C.C31-= K3*F1;
  C.C32-= K3*F2;
  C.C33-= K3*F3;
  C.C40-= K4*F0;
  C.C41-= K4*F1;
  C.C42-= K4*F2;
  C.C43-= K4*F3;
  C.C44-= K4*F4;
}

inline void FilterFirst( TrackV &track, HitV &hit, Station &st )
{

  CovV &C = track.C;
  Fvec_t w1 = ONE-hit.w;
  Fvec_t sigma2 = hit.w*st.Sigma2 + w1*INF;
  // initialize covariance matrix 
  C.C00= sigma2; 
  C.C10= ZERO;      C.C11= sigma2; 
  C.C20= ZERO;      C.C21= ZERO;      C.C22= INF;
  C.C30= ZERO;      C.C31= ZERO;      C.C32= ZERO; C.C33= INF;
  C.C40= ZERO;      C.C41= ZERO;      C.C42= ZERO; C.C43= ZERO; C.C44= INF;

  track.T[0] = hit.w*hit.x + w1*track.T[0];
  track.T[1] = hit.w*hit.y + w1*track.T[1];
  track.NDF = -3.0;
  track.Chi2 = ZERO;
}

inline void AddMaterial( TrackV &track, Station &st, Fvec_t &qp0 )
{
  cnst mass2 = 0.1396*0.1396;

  Fvec_t tx = track.T[2];
  Fvec_t ty = track.T[3];
  Fvec_t txtx = tx*tx;
  Fvec_t tyty = ty*ty;
  Fvec_t txtx1 = txtx + ONE;
  Fvec_t h = txtx + tyty;
  Fvec_t t = sqrt(txtx1 + tyty);
  Fvec_t h2 = h*h;
  Fvec_t qp0t = qp0*t;
  
  cnst c1=0.0136, c2=c1*0.038, c3=c2*0.5, c4=-c3/2.0, c5=c3/3.0, c6=-c3/4.0;
    
  Fvec_t s0 = (c1+c2*st.logRadThick + c3*h + h2*(c4 + c5*h +c6*h2) )*qp0t;
    
  Fvec_t a = (ONE+mass2*qp0*qp0t)*st.RadThick*s0*s0;

  CovV &C = track.C;

  C.C22 += txtx1*a;
  C.C32 += tx*ty*a; C.C33 += (ONE+tyty)*a; 
}

inline void GuessVec( TrackV &t, Station *vStations, int NStations )
{
  Fvec_t *T = t.T;

  Int_t NHits = NStations;

  Fvec_t A0, A1=ZERO, A2=ZERO, A3=ZERO, A4=ZERO, A5=ZERO, a0, a1=ZERO, a2=ZERO,
    b0, b1=ZERO, b2=ZERO;
  Fvec_t z0, x, y, z, S, w, wz, wS;

  Int_t i=NHits-1;
  z0 = vStations[i].zhit;
  HitV *hlst = &(t.vHits[i]);
  w = hlst->w;
  A0 = w;
  a0 = w*hlst->x;
  b0 = w*hlst->y;
  HitV *h = t.vHits;
  Station *st = vStations;
  for( ; h!=hlst; h++, st++ ){
    x = h->x;
    y = h->y;
    w = h->w;
    z = st->zhit - z0;
    S = st->Sy;
    wz = w*z;
    wS = w*S;
    A0+=w; 
    A1+=wz;  A2+=wz*z;
    A3+=wS;  A4+=wS*z; A5+=wS*S;
    a0+=w*x; a1+=wz*x; a2+=wS*x;
    b0+=w*y; b1+=wz*y; b2+=wS*y;
  }

  Fvec_t A3A3 = A3*A3;
  Fvec_t A3A4 = A3*A4;
  Fvec_t A1A5 = A1*A5;
  Fvec_t A2A5 = A2*A5;
  Fvec_t A4A4 = A4*A4;
  
  Fvec_t det = rcp(-A2*A3A3 + A1*( A3A4+A3A4 - A1A5) + A0*(A2A5-A4A4));
  Fvec_t Ai0 = ( -A4A4 + A2A5 );
  Fvec_t Ai1 = (  A3A4 - A1A5 );
  Fvec_t Ai2 = ( -A3A3 + A0*A5 );
  Fvec_t Ai3 = ( -A2*A3 + A1*A4 );
  Fvec_t Ai4 = (  A1*A3 - A0*A4 );
  Fvec_t Ai5 = ( -A1*A1 + A0*A2 );

  Fvec_t L, L1;
  T[0] = (Ai0*a0 + Ai1*a1 + Ai3*a2)*det;
  T[2] = (Ai1*a0 + Ai2*a1 + Ai4*a2)*det;
  Fvec_t txtx1 = 1.+T[2]*T[2];
  L    = (Ai3*a0 + Ai4*a1 + Ai5*a2)*det *rcp(txtx1);
  L1 = L*T[2];
  A1 = A1 + A3*L1;
  A2 = A2 + ( A4 + A4 + A5*L1 )*L1;
  b1+= b2 * L1;
  det = rcp(-A1*A1+A0*A2);

  T[1] = (  A2*b0 - A1*b1 )*det;
  T[3] = ( -A1*b0 + A0*b1 )*det;
  T[4] = -L*c_light_i*rsqrt(txtx1 +T[3]*T[3]);
  T[5] = z0;
}   

inline void Fit( TrackV &t, Station vStations[], int NStations )
{

  HitInfo Xinfo, Yinfo;
  cnst c16 = 16.;
  Xinfo.cos_phi = ONE; 
  Xinfo.sin_phi = ZERO;
  Xinfo.sigma2  = vStations[0].Sigma2;
  Xinfo.sigma216 = Xinfo.sigma2*c16;
  Yinfo.cos_phi = ZERO;
  Yinfo.sin_phi = ONE; 
  Yinfo.sigma2  = Xinfo.sigma2;
  Yinfo.sigma216 = Xinfo.sigma216;

  // upstream 

  GuessVec( t, vStations,NStations );
  
  // downstream

  FieldRegion f;
  Fvec_t z0,z1,z2, dz;
  FieldVector H0, H1, H2;

  Fvec_t qp0 = t.T[4];
  Int_t i= NStations-1;
  HitV *h = &t.vHits[i];

  FilterFirst( t, *h, vStations[i] );
  AddMaterial( t, vStations[ i ], qp0 );

  z1 = vStations[ i ].z;
  vStations[i].Map.GetField(t.T[0],t.T[1], H1);
  H1.Combine( h->H, h->w );

  z2 = vStations[ i-2 ].z;
  dz = z2-z1;
  vStations[ i-2 ].Map.GetField(t.T[0]+t.T[2]*dz,t.T[1]+t.T[3]*dz,H2);
  h = &t.vHits[i-2];
  H2.Combine( h->H, h->w );

  for( --i; i>=0; i-- ){
    h = &t.vHits[i];
    Station &st = vStations[i];
    z0 = st.z;    
    dz = (z1-z0);
    st.Map.GetField(t.T[0]-t.T[2]*dz,t.T[1]-t.T[3]*dz,H0);
    H0.Combine( h->H, h->w );
    f.Set( H0, z0, H1, z1, H2, z2);

    //CalcRegion(f,t.T,vStations[i+1].Map,vStations[i+1].MapLeft, st.Map ); 

    ExtrapolateALight( t.T, t.C, st.zhit, qp0, f );
    //ExtrapolateALight( t.T, t.C, st.z+st.thick/4, qp0);//, f );
    AddMaterial( t, st, qp0 );
    //ExtrapolateALight( t.T, t.C, st.zhit, qp0);//, f );
    Filter( t, Xinfo, h->x, h->w );
    Filter( t, Yinfo, h->y, h->w );
    //ExtrapolateALight( t.T, t.C, st.z-st.thick/4, qp0);//, f );
    //AddMaterial( t, st, qp0 );
    //ExtrapolateALight( t.T, t.C, st.zhit, qp0);//, f );
    H2 = H1; 
    z2 = z1;
    H1 = H0; 
    z1 = z0;
  }
}

#undef cnst

#endif
