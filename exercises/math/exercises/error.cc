#include<cmath>
  float stripErrorSquared(const float uProj) {
      const float P1=-0.339f;
      const float P2=0.90f;
      const float P3=0.279f;
      //      const float uerr = P1*uProj*vdt::fast_expf(-uProj*P2)+P3;
      const float uerr = P1*uProj*std::exp(-uProj*P2)+P3;
      return uerr*uerr;
  }
