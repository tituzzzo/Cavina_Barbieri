struct Vector3D
{
  double x;
  double y;
  double z;

  Vector3D(double x_ = 0., double y_ = 0., double z_ = 0.)
      : x{x_}
      , y{y_}
      , z{z_}
  {}

  Vector3D& operator+=(const Vector3D&);
  double norm() const;
  Vector3D operator-(const Vector3D&) const;
  Vector3D operator*(const Vector3D&) const;
  Vector3D operator*(double scalar) const;
};
