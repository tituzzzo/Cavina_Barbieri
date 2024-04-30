#include "vector3d.hpp"

class Bird
{
 private:
  Vector3D position_;
  Vector3D velocity_;
  int index_;

 public:
  Bird(int index, Vector3D const& position);
  
};