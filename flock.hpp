#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "bird.hpp"
#include "statistics.hpp"

#include <stdexcept>
#include <vector>
#include <cmath>

namespace fl {

struct FParametres
{
  const int n_birds_;        // number of birds to spawn
  const int box_size_;       // size of the box area where the simulation will run
  double d_;                 // distance used with alignment and cohesion rule
  double d_s_;               // distance used with separation rule
  double s_;                 // separation factor
  double a_;                 // aligment factor
  double c_;                 // cohesion factor
  double max_bird_velocity_; // maximum speed of a bird

  // default parametres values have been set according to our suggestion
  // clang-format off
  FParametres(const int n_birds = 200,     
              const int box_size = 140,     
              const double s = 0.7,        
              const double a = 0.01,        
              const double c = 0.6,      
              const double max_bird_velocity = 300.)        
      :                              
              n_birds_{n_birds}, 
              box_size_{box_size}, 
              s_{s}, 
              a_{a}, 
              c_{c}, 
              max_bird_velocity_{max_bird_velocity}
  {
    const double max_d_s{sqrt(pow(box_size, 2) / static_cast<float>(n_birds))};
    d_s_ =  0.5 * max_d_s;
    d_ = max_d_s + box_size * 0.005;

    if(n_birds < 3 || n_birds > 1000) { throw std::runtime_error{"Flock parameter: wrong n_birds initialization."}; }
    if(box_size < 50 || box_size > 200) { throw std::runtime_error{"Flock parameter: wrong box_size initialization."}; }
    if(d_ < max_d_s) { throw std::runtime_error{"Flock parameter: wrong d initialization."}; }
    if(d_s_ < 0 || d_s_ > sqrt(pow(box_size, 2) / static_cast<float>(n_birds))) { throw std::runtime_error{"Flock parameter: wrong d_s initialization."}; }
    if(s < 0 || s > 1) { throw std::runtime_error{"Flock parameter: wrong s initialization."}; }
    if(a < 0 || a > 1) { throw std::runtime_error{"Flock parameter: wrong a initialization."}; }
    if(c < 0 || c > 1) { throw std::runtime_error{"Flock parameter: wrong c initialization."}; }
    if(max_bird_velocity < 10 || max_bird_velocity > 500) { throw std::runtime_error{"Flock parameter: wrong max_bird_velocity initialization."}; }
  }
  // clang-format on
};

class Flock
{
 private:
  // a vector containing all active birds
  std::vector<Bird> birds_;

  void spawn_birds();
  void set_random_velocities();

  // this method applies all flock rules to a bird and prevents it from exceeding the max_bird_velocity parameter
  void calc_bird_velocity(Bird& reference_bird) const;

 public:
  FParametres par_;
  Flock(FParametres const& flock_parameters);
  Bird const& get_bird(const int bird_index) const;
  std::vector<Bird> const& get_birds() const;
  void update_birds_position(const double delta_time);
};
 
Statistics calc_average_velocity_norm(std::vector<Bird> const& birds);

Statistics calc_average_bird_to_bird_distance(std::vector<Bird> const& birds);

void limit_bird_velocity_to_value(Vector2D& velocity, const double max_bird_velocity_norm);

Vector2D separation_rule(Bird const& reference_bird, const double d_s, const double s_factor, std::vector<Bird> const& birds);

Vector2D alignment_rule(Bird const& reference_bird, const double d, const double a_factor, std::vector<Bird> const& birds);

Vector2D calc_mass_center(Bird const& reference_bird, const double d, std::vector<Bird> const& birds);

Vector2D cohesion_rule(Bird const& reference_bird, const double d, const double c_factor, std::vector<Bird> const& birds);

double calc_bird_to_bird_distance(Bird const& bird1, Bird const& bird2);

double get_bird_direction(Bird const& reference_bird, const bool use_degrees);

} // namespace fl

#endif