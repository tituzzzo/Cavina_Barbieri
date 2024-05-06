#include "bird.hpp"
#include <random>

Bird::Bird(Vector3D const& position)
    : position_{position}
{
  // class invariant posizione nella box
}

Vector3D const& Bird::get_position()
{
  return position_;
}

void Bird::set_position(Vector3D& new_position)
{
  position_ = new_position;
}

Vector3D const& Bird::get_velocity()
{
  return velocity_;
}

void Bird::set_velocity(Vector3D& new_velocity)
{
  velocity_ = new_velocity;
}