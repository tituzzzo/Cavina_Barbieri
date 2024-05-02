#include "vector3d.hpp"
#include <vector>

class Bird
{
 private:
  Vector3D position_;
  Vector3D velocity_;
  std::vector<Bird*> birds_within_distance_d; //d value is defined inside flock class
  std::vector<Bird*> birds_within_distance_d_s; //d_s value is defined inside flock class

 public:
  Bird(Vector3D const& position);

  Vector3D const& get_position();
  void set_position(Vector3D&);
  Vector3D const& get_velocity();
  void set_velocity(Vector3D&);
  
};

































































