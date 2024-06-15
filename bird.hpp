#ifndef BIRD_HPP
#define BIRD_HPP

#include "vector2d.hpp"
#include <vector>

namespace fl {

class Bird
{
 private:
  const int index_;
  Vector2D position_;
  Vector2D velocity_;

 public:
  Bird(int index, Vector2D const& position)
      : index_{index}
      , position_{position}
  {}

  bool operator!=(Bird const&) const;

  int get_index() const;
  Vector2D const& get_position() const;
  void set_position(Vector2D const&);
  Vector2D const& get_velocity() const;
  void set_velocity(Vector2D const&);
};

} // namespace fl

#endif
