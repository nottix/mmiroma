#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <csim.h>


double mu = 3.86;
double lambda = 9.46;

double calc_gauss_inverse() 
{
 double x; 
 double v = normal(0,1);
 double y = pow(v,2);
 double x1 = 0;
 x1 = mu + ((pow(mu,2)*y)/(2*lambda)) - ((mu/(2*lambda)) * sqrt(4*mu*lambda*y+pow(mu,2)*pow(y,2)));
 double u = uniform(0,1);
 double temp = mu/(mu+x1);
 if(u <= temp) {
	x = x1;
 } 
 else
	x = pow(mu,2)/x1;

 return x;
}

double calc_p(double x) 
{
  return sqrt(lambda/(2*M_PI))*pow(x,-3/2)*pow(M_E,(-lambda*pow((x-mu),2))/(2*pow(mu,2)*x));
  
}

void sim(int argc, char **argv) 
{
  int i = 0;
  for(;i<1000;i++) {
    double res = calc_gauss_inverse(); 
    printf("%lf\n",p);
    //double p = calc_p(res); 


  }
}
