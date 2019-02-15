#include <stdio.h>
#include <Eigen/Dense>


using namespace Eigen ;


extern "C"
 {

  double f( double* x , double* y );

  double f( double* x , double* y )
   {

    double r ;
    int i ;

    VectorXf vx(6) , vy(6) ;

    for( i=0; i<6; ++i)
     {

      vx[i] = x[i] ;

      vy[i] = y[i] ;

     }

    r = vx.dot( vy ) ;

    return( r ) ;

   }

 }

