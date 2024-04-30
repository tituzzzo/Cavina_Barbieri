#include "bird.hpp"
#include <vector>

class Flock
{
  Vector3D mass_center;
  std::vector<Bird*> birds;
  Flock();

  auto calculate_mass_center(); //quale tipo di ritorno? non si può usare auto in una dichiarazione
  void spawn_birds(int);
  Vector3D& calc_bird_velocity(int bird_index, Vector3D& bird_velocity);
};
