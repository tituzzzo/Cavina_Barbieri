#include "bird.hpp"
#include <random>

Bird::Bird(Vector3D const& position, int index)
    : position_{position}, index_{index}
{
  // class invariant posizione nella box
}

Vector3D const& Bird::get_position() const
{
  return position_;
}

bool Bird::operator!=(Bird& second_bird) const
{
  return index_ != second_bird.index_;
}

void Bird::set_position(Vector3D& new_position)
{
  position_ = new_position;
}

Vector3D const& Bird::get_velocity() const
{
  return velocity_;
}

void Bird::set_velocity(Vector3D& new_velocity)
{
  velocity_ = new_velocity;
}

