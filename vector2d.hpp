#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

struct Vector2D
{
  double x;
  double y;

  Vector2D(double x_ = 0., double y_ = 0.)
      : x{x_}
      , y{y_}
  {}

  
  double norm() const;
  Vector2D& operator+=(Vector2D const&);
  Vector2D operator-(Vector2D const&) const;
  Vector2D operator+(Vector2D const&) const;
  Vector2D operator*(Vector2D const&) const;
  Vector2D operator*(const double scalar) const;
  bool operator==(Vector2D const&) const;
};

#endif