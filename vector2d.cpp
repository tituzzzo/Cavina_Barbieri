#include "vector2d.hpp"
#include <cmath>

namespace fl {

double Vector2D::norm() const
{
  return sqrt(pow(x_, 2) + pow(y_, 2));
}

Vector2D& Vector2D::operator+=(Vector2D const& to_add)
{
  x_ += to_add.x_;
  y_ += to_add.y_;

  return *this;
}

Vector2D Vector2D::operator-(Vector2D const& v) const
{
  return Vector2D{x_ - v.x_, y_ - v.y_};
}

Vector2D Vector2D::operator+(Vector2D const& v) const
{
  return Vector2D{x_ + v.x_, y_ + v.y_};
}

Vector2D Vector2D::operator*(Vector2D const& v) const
{
  return Vector2D{x_ * v.x_, y_ * v.y_};
}

Vector2D Vector2D::operator*(const double scalar) const
{
  return Vector2D{x_ * scalar, y_ * scalar};
}

bool Vector2D::operator==(Vector2D const& v) const
{
  return x_ == v.x_ && y_ == v.y_;
}

} // namespace fl