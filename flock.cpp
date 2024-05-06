#include "flock.hpp"
#include <cmath>

Flock::Flock()
{
  // initialize birds
  spawn_birds(10);
  // init vector birds
  // init center_mass
}

void Flock::spawn_birds(int n_birds)
{
  for (int i = 0; i < n_birds; ++i) {
    Vector3D position{i * 0.1, i * 0.1, i * 0.1};
    Bird bird = Bird(position);
    birds.push_back(bird);
  }
}

void Flock::calc_bird_velocity(Vector3D& bird_velocity)
{}

void Flock::calc_average_velocity()
{
  const int N{static_cast<int>(birds.size())};

  /*
  if (N < 2) {
    throw std::runtime_error{"Not enough entries to run a statistics"};
  }
  */

  double sum_x{};
  double sum_x2{};

  for (auto& bird : birds) {
    double bird_velocity = (bird.get_velocity().norm());
    sum_x += bird_velocity;
    sum_x2 += pow(bird_velocity, 2);
  }

  average_velocity.mean_ = sum_x / N;
  average_velocity.sigma_ =
      std::sqrt((sum_x2 - N * pow(average_velocity.mean_, 2)) / (N - 1));
  average_velocity.mean_err_ = average_velocity.sigma_ / std::sqrt(N);
}

void Flock::calc_average_bird_to_bird_distance()
{}
