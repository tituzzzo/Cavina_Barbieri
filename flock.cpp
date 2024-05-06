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
    Bird bird = Bird(position, i);
    birds.push_back(bird);
  }
}

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

double calc_bird_to_bird_distance(const Bird& bird1, const Bird& bird2)
{
  Vector3D difference{bird1.get_position() - bird2.get_position()};
  double distance{};
  distance = difference.norm();
  return distance;
};

void Flock::find_birds_within_distance(double distance, Bird& reference_bird) const
{
  std::vector<Bird> provvisory_birds_within_distance{};
  for (const Bird& bird : birds){
    if (calc_bird_to_bird_distance(reference_bird, bird) <= distance && bird != reference_bird)
    {
      //-------------------------------------------------------
    }
    
  }
}


