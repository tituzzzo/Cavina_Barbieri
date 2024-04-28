#include "flock.hpp"

Flock::Flock()
{
  // initialize birds
  spawn_birds(10);
  // init vector birds
  // init center_mass
}

auto Flock::calculate_mass_center()
{
  // calc
}

auto Flock::spawn_birds(int n_birds)
{
  for (int i = 0; i < n_birds; ++i) {
    Vector3D position{i * 0.1, i * 0.1, i * 0.1};
    Bird* bird_ptr = new Bird(position);
    birds.push_back(bird_ptr);
    //finisce lo scope ma birds rimangono sullo heap
  }
}
