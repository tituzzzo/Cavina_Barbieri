#include "vector3d.hpp"
#include <cmath>

Vector3D& Vector3D::operator+=(const Vector3D& to_add)
{
  this->x += to_add.x;
  this->y += to_add.y;
  this->z += to_add.z;
  return *this;
}
double Vector3D::norm() const
{
  return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}