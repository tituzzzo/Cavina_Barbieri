#include "vector3d.hpp"
#include <cmath>

Vector3D& Vector3D::operator+=(Vector3D const& to_add)
{
  x += to_add.x;
  y += to_add.y;
  z += to_add.z;

  return *this;
}

double Vector3D::norm() const
{
  return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector3D Vector3D::operator-(Vector3D const& v) const
{
  return Vector3D{x - v.x, y - v.y, z - v.z};
}

Vector3D Vector3D::operator+(Vector3D const& v) const
{
  return Vector3D{x + v.x, y + v.y, z + v.z};
}

Vector3D Vector3D::operator*(Vector3D const& v) const
{
  return Vector3D{x * v.x, y * v.y, z * v.z};
}

Vector3D Vector3D::operator*(const double scalar) const
{
  return Vector3D{x * scalar, y * scalar, z * scalar};
}
