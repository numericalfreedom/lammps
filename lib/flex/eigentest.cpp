#include <iostream>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;


int eigentest( Matrix3d& , Vector3d& );

int eigentest( Matrix3d& s , Vector3d& v )
{
  MatrixXd m(2,2);
  m(0,0) = 3;
  m(1,0) = 2.5;
  m(0,1) = -1;
  m(1,1) = m(1,0) + m(0,1);
}
