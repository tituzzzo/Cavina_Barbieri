#include "vector2d.hpp"
#include <cmath>

Vector2D& Vector2D::operator+=(Vector2D const& to_add)
{
  x += to_add.x;
  y += to_add.y;

  return *this;
}

double Vector2D::norm() const
{
  return sqrt(pow(x, 2) + pow(y, 2));
}

Vector2D Vector2D::operator-(Vector2D const& v) const
{
  return Vector2D{x - v.x, y - v.y};
}

Vector2D Vector2D::operator+(Vector2D const& v) const
{
  return Vector2D{x + v.x, y + v.y};
}

Vector2D Vector2D::operator*(Vector2D const& v) const
{
  return Vector2D{x * v.x, y * v.y};
}

Vector2D Vector2D::operator*(const double scalar) const
{
  return Vector2D{x * scalar, y * scalar};
}
