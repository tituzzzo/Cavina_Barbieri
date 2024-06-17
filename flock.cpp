#include "flock.hpp"

#include <time.h>
#include <cassert>
#include <random>

#include <iostream>

namespace fl {

using index_t = std::vector<Bird>::size_type;

Flock::Flock(FParametres const& flock_parameters)
    : par_{flock_parameters}
{
  spawn_birds();
}

// this function is used to initialize the birds at a random location inside the box area
void Flock::spawn_birds()
{
  std::default_random_engine eng{static_cast<long unsigned int>(time(0))};
  std::uniform_real_distribution<> spawn(1., static_cast<double>(par_.box_size_) - 1.);
  double x, y;
  for (int i{0}; i < par_.n_birds_; ++i) {
    bool is_different_from_previous{false};
    while (is_different_from_previous == false) {
      x = static_cast<double>(spawn(eng));
      y = static_cast<double>(spawn(eng));

      bool b{true};
      for (Bird const& bird : birds_) {
        if (x == bird.get_position().x_ && y == bird.get_position().y_) {
          b = false;
          break;
        }
      }
      if (b) {
        is_different_from_previous = true;
      }
    }

    Vector2D position{x, y};
    Bird bird = Bird(i, position);
    birds_.push_back(bird);
  }

  set_random_velocities();
}

void Flock::set_random_velocities()
{
  std::default_random_engine eng{static_cast<long unsigned int>(time(0))};
  std::uniform_real_distribution<> spawn(10., par_.max_bird_velocity_ / 3);
  double x, y;
  for (Bird& bird : birds_) {
    x = static_cast<double>(spawn(eng));
    y = static_cast<double>(spawn(eng));
    bird.set_velocity({x, y});
  }
}

// this method applies all flock rules to a bird and prevents it from exceeding the max_bird_velocity
void Flock::calc_bird_velocity(Bird& reference_bird) const
{
  Vector2D new_velocity;
  new_velocity = reference_bird.get_velocity() + separation_rule(reference_bird, par_.d_s_, par_.s_, birds_) + alignment_rule(reference_bird, par_.d_, par_.a_, birds_) + cohesion_rule(reference_bird, par_.d_, par_.c_, birds_);

  if (new_velocity.norm() > par_.max_bird_velocity_) {
    limit_bird_velocity_to_value(new_velocity, par_.max_bird_velocity_);
    reference_bird.set_velocity(new_velocity);
  } else {
    reference_bird.set_velocity(new_velocity);
  }
}

Bird const& Flock::get_bird(const int bird_index) const
{
  if (bird_index < 0 || bird_index > static_cast<int>(birds_.size() - 1)) {
    throw std::runtime_error{"In function get_bird: bird_index out of range."};
  }
  return birds_[static_cast<index_t>(bird_index)];
}

std::vector<Bird> const& Flock::get_birds() const
{
  return birds_;
}

void Flock::update_birds_position(const double delta_time)
{
  if (delta_time < 0) {
    throw std::runtime_error{"In function update_bird_position: negative value of delta_time is not allowed."};
  }

  // check parameters correctness before each flock update
  // significantly decreases preformance, used for debug
  assert(par_.n_birds_ >= 3 && par_.n_birds_ <= 1000);
  assert(par_.box_size_ >= 50 && par_.box_size_ <= 200);
  assert(par_.d_ >= par_.d_s_);
  assert(par_.d_s_ >= 0 && par_.d_s_ <= sqrt(pow(par_.box_size_, 2) / static_cast<float>(par_.n_birds_)));
  assert(par_.s_ >= 0 && par_.s_ <= 1);
  assert(par_.a_ >= 0 && par_.a_ <= 1);
  assert(par_.c_ >= 0 && par_.c_ <= 1);
  assert(par_.max_bird_velocity_ >= 10 && par_.max_bird_velocity_ <= 500);

  for (Bird& bird : birds_) {
    calc_bird_velocity(bird);
    Vector2D new_position{};
    Vector2D old_position = bird.get_position();
    Vector2D delta_space  = bird.get_velocity() * delta_time;
    new_position          = old_position + delta_space;

    // if a bird leaves the box area it will be moved to the opposite side of the box area
    if (new_position.x_ > par_.box_size_) {
      new_position = {new_position.x_ - par_.box_size_, new_position.y_};
    }
    if (new_position.x_ < 0.) {
      new_position = {new_position.x_ + par_.box_size_, new_position.y_};
    }
    if (new_position.y_ > par_.box_size_) {
      new_position = {new_position.x_, new_position.y_ - par_.box_size_};
    }
    if (new_position.y_ < 0.) {
      new_position = {new_position.x_, new_position.y_ + par_.box_size_};
    }
    bird.set_position(new_position);

    assert(bird.get_position().x_ <= par_.box_size_ + 0.01 && bird.get_position().x_ > -0.01);
    assert(bird.get_position().y_ <= par_.box_size_ + 0.01 && bird.get_position().y_ > -0.01);
  }
}

Statistics calc_average_velocity_norm(std::vector<Bird> const& birds)
{
  Statistics average_velocity;
  const int N{static_cast<int>(birds.size())};

  if (N < 2) {
    throw std::runtime_error{"In funtion calc_average_velocity_norm: not enough entries to run statistics"};
  }

  double sum_x{};
  double sum_x2{};

  for (auto& bird : birds) {
    double bird_velocity = (bird.get_velocity().norm());
    sum_x += bird_velocity;
    sum_x2 += pow(bird_velocity, 2);
  }

  average_velocity.mean_     = sum_x / N;
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

  if (N < 3) {
    throw std::runtime_error{"In funtion calc_average_bird_to_bird_distance: not enough entries to run statistics"};
  }

  int sum_cont{0};
  for (index_t i{0}; i <= static_cast<index_t>(N - 1); ++i) {
    for (index_t j{i + 1}; j <= static_cast<index_t>(N - 1); ++j) {
      double distance = calc_bird_to_bird_distance(birds[i], birds[j]);
      sum_x += distance;
      sum_x2 += pow(distance, 2);
      ++sum_cont;
    }
  }
  average_bird_to_bird_distance.mean_     = sum_x / sum_cont;
  average_bird_to_bird_distance.sigma_    = std::sqrt((sum_x2 - sum_cont * pow(average_bird_to_bird_distance.mean_, 2)) / (sum_cont - 1));
  average_bird_to_bird_distance.mean_err_ = average_bird_to_bird_distance.sigma_ / std::sqrt(sum_cont);

  return average_bird_to_bird_distance;
}

void limit_bird_velocity_to_value(Vector2D& velocity, const double max_bird_velocity)
{
  if (velocity.norm() > max_bird_velocity) {
    const double scaling_factor = sqrt(static_cast<double>(pow(max_bird_velocity, 2)) / static_cast<double>(pow(velocity.x_, 2) + pow(velocity.y_, 2)));
    velocity                    = velocity * scaling_factor;
  }
  assert(velocity.norm() <= max_bird_velocity + 0.01);
}

Vector2D separation_rule(Bird const& reference_bird, const double d_s, const double s_factor, std::vector<Bird> const& birds)
{
  Vector2D steer{};
  for (Bird const& bird : birds) {
    const double d{calc_bird_to_bird_distance(reference_bird, bird)};
    if (bird != reference_bird && d <= d_s) {
      Vector2D diff{reference_bird.get_position() - bird.get_position()};
      diff = diff * (1. / diff.norm());
      diff = diff * (1. / d);
      steer += diff;
    }
  }
  return steer * (s_factor * 10.);
}

Vector2D alignment_rule(Bird const& reference_bird, const double d, const double a_factor, std::vector<Bird> const& birds)
{
  Vector2D birds_velocities_sum{};
  int n_near_birds{0};
  for (Bird const& bird : birds) {
    const double d_{calc_bird_to_bird_distance(reference_bird, bird)};
    if (bird != reference_bird && d_ <= d) {
      birds_velocities_sum += birds[static_cast<index_t>(bird.get_index())].get_velocity();
      ++n_near_birds;
    }
  }

  if (n_near_birds < 2) {
    return {0., 0.};
  } else {
    const double f = 1. / (static_cast<double>(n_near_birds));
    Vector2D mean_bird_velocity{birds_velocities_sum * f};
    return (mean_bird_velocity - reference_bird.get_velocity()) * a_factor;
  }
}

Vector2D calc_mass_center(Bird const& reference_bird, const double d, std::vector<Bird> const& birds)
{
  int n_near_birds{0};
  Vector2D mass_center{};
  for (Bird const& bird : birds) {
    const double d_{calc_bird_to_bird_distance(reference_bird, bird)};
    if (bird != reference_bird && d_ <= d) {
      mass_center += birds[static_cast<index_t>(bird.get_index())].get_position();
      ++n_near_birds;
    }
  }
  if (n_near_birds == 0) {
    return {0., 0.};
  } else {
    const double f = 1. / static_cast<double>(n_near_birds);
    return mass_center * f;
  }
}

Vector2D cohesion_rule(Bird const& reference_bird, const double d, const double c_factor, std::vector<Bird> const& birds)
{
  Vector2D mass_center{calc_mass_center(reference_bird, d, birds)};
  return (mass_center - reference_bird.get_position()) * c_factor;
}

double calc_bird_to_bird_distance(Bird const& bird1, Bird const& bird2)
{
  Vector2D difference{bird1.get_position() - bird2.get_position()};
  double distance{};
  distance = difference.norm();
  return distance;
}

double get_bird_direction(Bird const& reference_bird, const bool use_degrees)
{
  double angle{atan2(reference_bird.get_velocity().y_, reference_bird.get_velocity().x_)};

  if (use_degrees) {
    return (angle * 180.) / M_PI;
  } else {
    return angle;
  }
}

} // namespace fl