#include "bird.hpp"
#include <vector>

class Flock
{
  Vector3D mass_center;
  std::vector<Bird*> birds;
  Flock();

  auto calculate_mass_center();
  auto spawn_birds(int);
};
