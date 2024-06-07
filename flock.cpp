#include "flock.hpp"
#include <time.h>
#include <cmath>
#include <random>
// non servono tutti
#include <iostream>

using index_t = std::vector<Bird>::size_type;

Flock::Flock(FParametres const& flock_parameters)
    : par{flock_parameters}
{
  // initialize birds

  // birds.reserve(n_birds); //ha senso?
  spawn_birds();
  // init vector birds
  // init tutto con roba presa da input
}

void Flock::spawn_birds()
{
  //non si devono sovrappore
  std::default_random_engine eng{static_cast<long unsigned int>(time(0))};
  std::uniform_int_distribution<> spawn(5, 50);
  double x, y;
  for (int i = 0; i < par.n_birds; ++i) {
    x = static_cast<double>(spawn(eng));
    y = static_cast<double>(spawn(eng));

    Vector2D position{x, y};
    Bird bird = Bird(i, position);
    birds.push_back(bird);
  }
}

void Flock::calc_bird_velocity(Bird& reference_bird)
{
  Vector2D new_velocity;
  new_velocity = reference_bird.get_velocity() + separation_rule(reference_bird, par.d_s, par.s, birds) + alignment_rule(reference_bird, par.d, par.a, birds) + cohesion_rule(reference_bird, par.d, par.c, birds) + wall_rule(reference_bird, par.w, par.box_size);
  //if (new_velocity.norm() > par.max_bird_velocity) {
  //const double scaling_factor = sqrt(static_cast<double>(pow(par.max_bird_velocity, 2)) / static_cast<double>(pow(new_velocity.norm(),2)));
  //reference_bird.set_velocity(new_velocity * scaling_factor);
  // } else {
  //reference_bird.set_velocity(new_velocity);
  //}
  reference_bird.set_velocity(new_velocity);

}

int Flock::get_n_birds() const
{
  return par.n_birds;
}

void Flock::update_birds_position(const double delta_time)
{
  for (Bird& bird : birds) {
    calc_bird_velocity(bird);
    Vector2D new_position{};
    Vector2D old_position = bird.get_position();
    Vector2D delta_space  = bird.get_velocity() * delta_time;
    new_position          = old_position + delta_space;
    bird.set_position(new_position);
  }
  std::cout << "updated\n";
}

std::vector<double> Flock::get_coordinates_of_axis(const char axis) const // forse non serve con sflm.... se serve ancora metti passaggio per reference
{
  std::vector<double> coordinates{};
  coordinates.reserve(static_cast<index_t>(par.n_birds));
  for (const Bird& bird : birds) {
    switch (axis) {
    case 'x':
      coordinates.push_back(bird.get_position().x);
      break;
    case 'y':
      coordinates.push_back(bird.get_position().y);
      break;
    default:
      // nessun asse inserito o asse non esiste
      break;
    }
  }
  return coordinates;
}

Statistics calc_average_velocity_norm(std::vector<Bird> const& birds)
{
  Statistics average_velocity;
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

  average_velocity.mean_     = sum_x / N; // static cast n double??????
  average_velocity.sigma_    = std::sqrt((sum_x2 - N * pow(average_velocity.mean_, 2)) / (N - 1));
  average_velocity.mean_err_ = average_velocity.sigma_ / std::sqrt(N);

  return average_velocity;
}

Statistics calc_average_bird_to_bird_distance(std::vector<Bird> const& birds)
{
  Statistics average_bird_to_bird_distance;
  double sum_x{};
  double sum_x2{};

  const int N{static_cast<int>(birds.size())};

  int sum_cont{0};
  for (index_t i{0}; i <= static_cast<index_t>(N - 1); ++i) {
    for (index_t j{i + 1}; j <= static_cast<index_t>(N - 1); ++j) {
      double distance = calc_bird_to_bird_distance(birds[i], birds[j]);
      sum_x += distance;
      sum_x2 += pow(distance, 2);
      ++sum_cont;
    }
  }
  average_bird_to_bird_distance.mean_     = sum_x / sum_cont; // static cast n double??????
  average_bird_to_bird_distance.sigma_    = std::sqrt((sum_x2 - sum_cont * pow(average_bird_to_bird_distance.mean_, 2)) / (sum_cont - 1));
  average_bird_to_bird_distance.mean_err_ = average_bird_to_bird_distance.sigma_ / std::sqrt(sum_cont);

  return average_bird_to_bird_distance;
}

void find_birds_within_distance(std::vector<int>& vector_to_fill, Bird const& reference_bird, const double radius_distance, std::vector<Bird> const& birds)
{
  for (const Bird& bird : birds) {
    double distance{calc_bird_to_bird_distance(reference_bird, bird)};
    if (distance <= radius_distance && bird != reference_bird) {
      vector_to_fill.push_back(bird.get_index());
    }
  }
}

Vector2D separation_rule(Bird const& reference_bird, const double d_s, const double s_factor, std::vector<Bird> const& birds)
{
  std::vector<int> near_birds_indexes{};
  find_birds_within_distance(near_birds_indexes, reference_bird, d_s, birds);
  Vector2D birds_positions_sum{};
  for (auto it = near_birds_indexes.begin(), last = near_birds_indexes.end(); it != last; ++it) {
    Vector2D second_bird_position = birds[static_cast<index_t>(*it)].get_position();
    Vector2D distance             = second_bird_position - reference_bird.get_position();
    birds_positions_sum += distance;
  }
  return birds_positions_sum * (-s_factor);
}

Vector2D alignment_rule(Bird const& reference_bird, const double d, const double a_factor, std::vector<Bird> const& birds)
{
  std::vector<int> near_birds_indexes;
  find_birds_within_distance(near_birds_indexes, reference_bird, d, birds);
  const int N{static_cast<int>(near_birds_indexes.size())};
  if (N < 2) {
    return {0., 0.};
  }
  Vector2D birds_velocities_sum{};
  for (auto it = near_birds_indexes.begin(), last = near_birds_indexes.end(); it != last; ++it) {
    birds_velocities_sum += birds[static_cast<index_t>(*it)].get_velocity();
  }
  const double f = 1. / (static_cast<double>(N));
  Vector2D mean_bird_velocity{birds_velocities_sum * f};

  return (mean_bird_velocity - reference_bird.get_velocity()) * a_factor;
}

Vector2D calc_mass_center(std::vector<int> const& near_birds_indexes, std::vector<Bird> const& birds)
{
  Vector2D mass_center{};
  const int N{static_cast<int>(near_birds_indexes.size())};

  for (auto it = near_birds_indexes.begin(), last = near_birds_indexes.end(); it != last; ++it) {
    mass_center += birds[static_cast<index_t>(*it)].get_position();
  }
  const double f = 1. / static_cast<double>(N);
  return mass_center * f;
}

Vector2D cohesion_rule(Bird const& reference_bird, const double d, const double c_factor, std::vector<Bird> const& birds)
{
  std::vector<int> near_birds_indexes;
  find_birds_within_distance(near_birds_indexes, reference_bird, d, birds);
  const int N{static_cast<int>(near_birds_indexes.size())};
  if (N == 0) {
    return {0., 0.};
  }
  Vector2D mass_center{calc_mass_center(near_birds_indexes, birds)};
  return (mass_center - reference_bird.get_position()) * c_factor;
}

Vector2D wall_rule(Bird const& reference_bird, const double w_factor, const double box_size)
{
  Vector2D wall_distance;
  Vector2D half_wall_distance;
  Vector2D reference_bird_position = reference_bird.get_position();

  wall_distance.x      = (box_size / 2 - reference_bird_position.x);
  wall_distance.y      = (box_size / 2 - reference_bird_position.y);
  half_wall_distance.x = (box_size / 2 - std::abs(wall_distance.x));
  half_wall_distance.y = (box_size / 2 - std::abs(wall_distance.y));
  Vector2D velocity;
  velocity.x = wall_distance.x * (1. / half_wall_distance.x);
  velocity.y = wall_distance.y * (1. / half_wall_distance.y);
  return velocity;
}

double calc_bird_to_bird_distance(Bird const& bird1, Bird const& bird2)
{
  Vector2D difference{bird1.get_position() - bird2.get_position()};
  double distance{};
  distance = difference.norm();
  return distance;
}