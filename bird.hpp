#include "vector3d.hpp"
#include <vector>

class Bird
{
 private:
 int index_;
  Vector3D position_;
  Vector3D velocity_;
  std::vector<Bird*> birds_within_distance_d; //d value is defined inside flock class
  std::vector<Bird*> birds_within_distance_d_s; //d_s value is defined inside flock class

 public:
  Bird(Vector3D const& position, int index);

  void calc_bird_velocity();

  bool operator!=(Bird&) const;

  //FORSE NON SERVE
  Vector3D const& get_position() const;
  void set_position(Vector3D&);
  Vector3D const& get_velocity() const;
  void set_velocity(Vector3D&);
  std::vector<Bird*> const& get_birds_within_distance_d() const;
  void set_birds_within_distance_d(std::vector<Bird*>& );
  std::vector<Bird*> const& get_birds_within_distance_d_s() const;
  void set_birds_within_distance_d_s(std::vector<Bird*>&);
};
































































