#include "bird.hpp"
#include <random>

Bird::Bird(int index, Vector2D const& position)
    : index_{index}
    , position_{position}
{
  // class invariant posizione nella box
}
// usare template
bool Bird::operator!=(Bird& second_bird) const
{
  return index_ != second_bird.index_;
}

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

void Bird::set_position(Vector2D& new_position)
{
  position_ = new_position;
}

Vector2D const& Bird::get_velocity() const
{
  return velocity_;
}

void Bird::set_velocity(Vector2D& new_velocity)
{
  velocity_ = new_velocity;
}
