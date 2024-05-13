#include "bird.hpp"
#include <random>

Bird::Bird(int index, Vector3D const& position)
    : index_{index}, position_{position}
{
  // class invariant posizione nella box
}

bool Bird::operator!=(Bird& second_bird) const
{
  return index_ != second_bird.index_;
}

Vector3D const& Bird::get_position() const
{
  return position_;
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

