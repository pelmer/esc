typedef float Scalar;
// typedef double Scalar;
constexpr int SIZE=10;

Scalar overabS;
Scalar offset;
Scalar alphaS;
void compChi2(Scalar const * ampl, Scalar const * err2, Scalar t, 
	      Scalar sumAA, Scalar& chi2, Scalar& amp) {
  Scalar sumAf = 0;
  Scalar sumff = 0;
  constexpr Scalar eps = Scalar(1e-6);
  constexpr Scalar denom =  Scalar(1)/Scalar(SIZE);
  
  for(unsigned int it = 0; it < SIZE; it++){
    Scalar offset = (Scalar(it) - t)*overabS;
    Scalar term1 = Scalar(1) + offset;
    if(term1>eps){
      Scalar f = std::exp( alphaS*(std::log(term1) - offset) );
      sumAf += ampl[it]*(f*err2[it]);
      sumff += f*(f*err2[it]);
    }
  }
  
  chi2 = sumAA;
  amp = 0;
  if( sumff > 0 ){
    amp = sumAf/sumff;
    chi2 = sumAA - sumAf*amp;
  }
  chi2 *=denom;
}
