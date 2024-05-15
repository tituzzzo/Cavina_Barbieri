#include "vector3d.hpp"
#include <vector>

class Bird
{
 private:
 const int index_;
  Vector3D position_;
  Vector3D velocity_;

 public:
  Bird(int index, Vector3D const& position);

  bool operator!=(Bird&) const; //serve????????? o basta il caso const?
  bool operator!=(Bird const&) const;

  int get_index() const;
  Vector3D const& get_position() const;
  void set_position(Vector3D&);
  Vector3D const& get_velocity() const;
  void set_velocity(Vector3D&);
};
































































