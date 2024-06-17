#include "bird.hpp"

namespace fl {

bool Bird::operator!=(Bird const& second_bird) const
{
  return index_ != second_bird.index_;
}

int Bird::get_index() const
{
  return index_;
}

Vector2D const& Bird::get_position() const
{
  return position_;
}

void Bird::set_position(Vector2D const& new_position)
{
  position_ = new_position;
}

Vector2D const& Bird::get_velocity() const
{
  return velocity_;
}

void Bird::set_velocity(Vector2D const& new_velocity)
{
  velocity_ = new_velocity; 
}

} // namespace fl
