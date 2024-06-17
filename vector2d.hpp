#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

namespace fl {

struct Vector2D 
{
  double x_;
  double y_;

  Vector2D(double x = 0., double y = 0.)
      : x_{x}
      , y_{y}
  {}

  double norm() const;
  Vector2D& operator+=(Vector2D const&);
  Vector2D operator-(Vector2D const&) const;
  Vector2D operator+(Vector2D const&) const;
  Vector2D operator*(Vector2D const&) const;
  Vector2D operator*(const double scalar) const;
  bool operator==(Vector2D const&) const;
};

} // namespace fl

#endif