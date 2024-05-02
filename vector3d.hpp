struct Vector3D
{
    double x;
    double y;
    double z;

    Vector3D& operator+=(const Vector3D&);
    double norm() const;
};
