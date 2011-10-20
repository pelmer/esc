#include <oxstd.h>
#include <oxfloat.h>

FZero(const Fcn,x,dx,...){
//  x0=FZERO(F,X) finds a zero of f(x).  F is a scalar function of
//       a scalar x, and the value supplied for x is a starting guess.
//
//	An optional third argument sets the relative tolerance for convergence.
//      The algorithm is the widely used Dekker-Brent algorithm.
//      Programmed by N. Shamsundar, University of Houston, 1999.

decl a,b,c,d,e,fa,fb,fc,i,m,args=va_arglist(),tol,toler,s,p,q,r;

if(sizeof(args)==0)tol=1.2e-13; else tol=args[0];
if (sizer(x) > 1 || sizec(x) > 1 || any(isdotinf(x))){
   println("Second argument must be a finite scalar.");
   return 0;
   }

a = x - dx;  fa = Fcn(a);
b = x + dx;  fb = Fcn(b);

// Find point where F(x) changes sign.

while (fa > 0 == fb > 0){
   dx = 2*dx;
   a = x - dx;  fa = Fcn(a);
   if (fa > 0 != fb > 0)break;
   b = x + dx;  fb = Fcn(b);
   }

fc = fb;
while (fb != 0){
   // Arrange results such that b is the best roott so far,
   // a is the previous value of b, and F(c)F(b) < 0.
   if (fb > 0 == fc > 0){
      c = a;  fc = fa;
      d = b - a;  e = d;
   }
   if (fabs(fc) < fabs(fb)){
      a = b;    b = c;    c = a;
      fa = fb;  fb = fc;  fc = fa;
      }

   // Convergence test
   m = 0.5*(c - b);
   toler = 2.0*tol*max(fabs(b),1.0);
   if (fabs(m) <= toler || fb == 0.0)break;

   // Choose bisection or interpolation
   if (fabs(e) < toler || fabs(fa) <= fabs(fb)){
   // Bisection
      d = m;  e = m;
      }else{
   // Interpolation
      s = fb/fa;
      if (a == c){
      // Linear interpolation
         p = 2.0*m*s;
         q = 1.0 - s;
	}else{
      // Inverse quadratic interpolation
         q = fa/fc;
         r = fb/fc;
         p = s*(2.0*m*q*(q - r) - (b - a)*(r - 1.0));
         q = (q - 1.0)*(r - 1.0)*(s - 1.0);
	}
      if (p > 0) q = -q; else p = -p;
      // Is interpolated point acceptable
      if (2.0*p < 3.0*m*q - fabs(toler*q) * p < fabs(0.5*e*q)){
         e = d;  d = p/q;
         } else {d = m;  e = m;}
     }
   // End Interpolation

   // Select next point
   a = b;
   fa = fb;
   if (fabs(d) > toler) b = b + d;
   else {if (b > c) b-= toler; 
        else b += toler;
        }
   fb = Fcn(b);
	}
return b;
}

func_v(const r)
{
	return (r * densn(r) + tailn(r)) * sqrt(M_2PI);
}
func_z(const r, const c, const iPrint)
{
	decl v = func_v(r), xi, xi1, fi, i, sum = 0, sumr = 0;

	for (i = c - 1, xi = xi1 = r, fi = exp(-.5 * xi * xi); i >= 2; --i)
	{
		if (iPrint == 2)
			println("xi=", xi, " fi=", fi, " ratio=", xi / xi1);
		xi1 = xi;
		xi = sqrt(-2.0 * log(v / xi + fi));
		fi = exp(-.5 * xi * xi);
		sum += (xi1 - xi) * fi;
		sumr += xi / xi1;
	}
	if (iPrint)
	{
		if (iPrint == 2)
			println("xi=", xi, " fi=", fi, " ratio=", xi / xi1);
		println("total rectangular area:             ", v * c);
		println("total area below curve:             ", 0.5 * sqrt(M_2PI));
		println("total rectangular area below curve: ", sum);
		println("efficiency step 1:  ", sumr / (c - 2) );
		println("efficiency overall: ", (0.5 * sqrt(M_2PI)) / (v * c));
	}
	return v - xi + xi * fi;
}
static decl s_iC = 128;
func_z_sol(const x)
{
	return func_z(x, s_iC, FALSE);
}

main()
{
	decl r, eps = 1e-16;

	format("%21.15e");
//	println("z(r): ", func_z(2.3382, 8, 0));
//	println("z(r): ", func_z(2.3383, 8, 0));
	
	s_iC = 3;
	r = FZero(func_z_sol, 1.6986, 0.01, eps);
	println("\nC=", s_iC, " R=", r);
	println("z(r): ", func_z(r, s_iC, 1));

	s_iC = 4;
	r = FZero(func_z_sol, 1.91492, 0.001, eps);
	println("\nC=", s_iC, " R=", r);
	println("z(r): ", func_z(r, s_iC, 1));

	s_iC = 8;
	r = FZero(func_z_sol, 2.3383, 0.01, eps);
	println("\nC=", s_iC, " R=", r);
	println("z(r): ", func_z(r, s_iC, 1));

	s_iC = 16;
	r = FZero(func_z_sol, 2.6755, 0.001, eps);
	println("\nC=", s_iC, " R=", r);
	println("z(r): ", func_z(r, s_iC, 1));

	s_iC = 32;
	r = FZero(func_z_sol, 2.9613, 0.001, eps);
	println("\nC=", s_iC, " R=", r);
	println("z(r): ", func_z(r, s_iC, 1));

	s_iC = 64;
	r = FZero(func_z_sol, 3.2136576, 0.0001, eps);
	println("\nC=", s_iC, " R=", r);
	println("z(r): ", func_z(r, s_iC, 1));

	s_iC = 128;
	r = FZero(func_z_sol, 3.44261, 0.0001, eps);
	println("\nC=", s_iC, " R=", r);
	println("z(r): ", func_z(r, s_iC, 1));

	s_iC = 256;
	r = FZero(func_z_sol, 3.6541528, 0.00001, eps);
	println("\nC=", s_iC, " R=", r);
	println("z(r): ", func_z(r, s_iC, 1));

}