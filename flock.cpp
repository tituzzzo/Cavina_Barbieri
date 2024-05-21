#include "flock.hpp"
#include <cmath>
#include <random>

Flock::Flock(const int n_birds_, const double box_size_)
    : n_birds{n_birds_}
    , box_size{box_size_}
{
  // initialize birds

  // birds.reserve(n_birds); //ha senso?
  spawn_birds();
  // init vector birds
  // init tutto con roba presa da input
}

void Flock::spawn_birds()
{
  std::default_random_engine eng;
  std::uniform_int_distribution<> spawn(1, 9);
  double x, y, z;
  for (int i = 0; i < n_birds; ++i) {
    x = static_cast<double>(spawn(eng));
    y = static_cast<double>(spawn(eng));
    z = static_cast<double>(spawn(eng));

    Vector3D position{x, y, z};
    Bird bird = Bird(i, position);
    birds.push_back(bird);
  }
}

Bird& Flock::get_bird(const int index)
{
  return birds[static_cast<int>(index)];
}

void Flock::calc_average_velocity_norm()
{
  const int N{static_cast<int>(birds.size())}; // puoi usare diretto n_birds

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
}

void Flock::calc_average_bird_to_bird_distance()
{
  double sum_x{};
  double sum_x2{};
  for (int i = 0; i < n_birds; ++i) {
    for (int j = i + 1; j < n_birds; ++j) {
      double distance = calc_bird_to_bird_distance(birds[i], birds[j]);
      sum_x += distance;
      sum_x2 += pow(distance, 2);
    }
  }
  average_bird_to_bird_distance.mean_     = sum_x / n_birds; // static cast n double??????
  average_bird_to_bird_distance.sigma_    = std::sqrt((sum_x2 - n_birds * pow(average_bird_to_bird_distance.mean_, 2)) / (n_birds - 1));
  average_bird_to_bird_distance.mean_err_ = average_bird_to_bird_distance.sigma_ / std::sqrt(n_birds);
}

std::vector<int> Flock::find_birds_within_distance(const double radius_distance, Bird const& reference_bird) const
{
  std::vector<int> indexes_of_birds_within_distance;
  for (const Bird& bird : birds) {
    double distance{calc_bird_to_bird_distance(reference_bird, bird)};
    if (distance <= radius_distance && bird != reference_bird) {
      indexes_of_birds_within_distance.push_back(bird.get_index());
    }
  }
  return indexes_of_birds_within_distance;
}

Vector3D Flock::separation_rule(Bird const& reference_bird) const
{
  std::vector<int> near_birds_indexes = find_birds_within_distance(d_s, reference_bird);
  Vector3D birds_positions_sum;
  for (auto it = near_birds_indexes.begin(), last = near_birds_indexes.end(); it != last; ++it) {
    Vector3D second_bird_position = birds[*it].get_position();
    Vector3D distance             = second_bird_position - reference_bird.get_position();
    birds_positions_sum += distance;
  }
  return birds_positions_sum * (-s);
}

Vector3D Flock::alignment_rule(Bird const& reference_bird) const
{
  std::vector<int> near_birds_indexes = find_birds_within_distance(d, reference_bird);
  Vector3D birds_velocities_sum{};
  for (auto it = near_birds_indexes.begin(), last = near_birds_indexes.end(); it != last; ++it) {
    birds_velocities_sum += birds[*it].get_velocity();
  }
  const int N{static_cast<int>(near_birds_indexes.size())};
  if (N < 2) {
    return {0., 0., 0.};
  }
  Vector3D mean_bird_velocity{birds_velocities_sum * static_cast<double>(1 / (N - 1))};
  return (mean_bird_velocity - reference_bird.get_velocity()) * a;
}

Vector3D Flock::calc_mass_center(std::vector<int> const& birds_indexes) const
{
  Vector3D mass_center;
  for (auto it = birds_indexes.begin(), last = birds_indexes.end(); it != last; ++it) {
    mass_center += birds[*it].get_position();
  }
  const int N{static_cast<int>(birds.size())};
  if (N < 2) {
    return {0., 0., 0.};
  }
  mass_center = mass_center * static_cast<double>(1 / (N - 1));
  return mass_center;
}

Vector3D Flock::cohesion_rule(Bird const& reference_bird) const
{
  std::vector<int> near_birds_indexes = find_birds_within_distance(d, reference_bird);
  Vector3D mass_center{calc_mass_center(near_birds_indexes)};
  return (mass_center - reference_bird.get_position()) * c;
}

std::vector<double> Flock::get_coordinates_of_axis(const char axis) const
{
  std::vector<double> coordinates{};
  coordinates.reserve(n_birds);
  for (const Bird& bird : birds) {
    switch (axis) {
    case 'x':
      coordinates.push_back(bird.get_position().x);
      break;
    case 'y':
      coordinates.push_back(bird.get_position().y);
      break;
    case 'z':
      coordinates.push_back(bird.get_position().z);
      break;
    default:
      // nessun asse inserito o asse non esiste
      break;
    }
  }
  return coordinates;
}

void Flock::calc_bird_velocity(Bird& reference_bird)
{
  Vector3D new_velocity;
  new_velocity = reference_bird.get_velocity() + separation_rule(reference_bird) + alignment_rule(reference_bird) + cohesion_rule(reference_bird);
  reference_bird.set_velocity(new_velocity);
}

void Flock::update_birds_position(const double delta_time)
{
  for (Bird& bird : birds) {
    calc_bird_velocity(bird);
    Vector3D new_position{};
    Vector3D old_position = bird.get_position();
    Vector3D delta_space  = bird.get_velocity() * delta_time;
    new_position          = old_position + delta_space;
    bird.set_position(new_position);
  }
}

double calc_bird_to_bird_distance(Bird const& bird1, Bird const& bird2)
{
  Vector3D difference{bird1.get_position() - bird2.get_position()};
  double distance{};
  distance = difference.norm();
  return distance;
}
Vector3D Flock::wall_rule(Bird const& reference_bird, double distance_parameter) const
{
  Vector3D wall_distance;
  Vector3D reference_bird_position = reference_bird.get_position();
  wall_distance.x                  = (box_size - reference_bird_position.x);
  wall_distance.y                  = (box_size - reference_bird_position.y);
  wall_distance.z                  = (box_size - reference_bird_position.z);
  Vector3D velocity;
  velocity = wall_distance * distance_parameter;
  return velocity;
}
