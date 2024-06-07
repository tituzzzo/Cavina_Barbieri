#ifndef FLOCK_HPP
#define FLOCK_HPP

#include "bird.hpp"
#include <vector>

#include "statistics.hpp"

struct FParametres
{
  const int n_birds; // number of birds to spawn 
  const double box_size; // edge size of the box area where the simulation will run
  const double d; // distance used with alignment and cohesion rule
  const double d_s; // distance used with separation rule
  const double s; // separation factor
  const double a; // aligment factor
  const double c; // cohesion factor
  const double w; // wall repulsion factor
  const double max_bird_velocity; //maximum speed of a bird

  // default parametres values have been set according to our suggestion
  // clang-format off
  FParametres(const int n_birds_ = 100,     
              const double box_size_ = 40., 
              const double d_ = 15.,        
              const double d_s_ = 5.,      
              const double s_ = 0.9,        
              const double a_ = 0.001,        
              const double c_ = 0.6,        
              const double w_ = 1,
              const double max_bird_velocity_ = 10000.)        
      :                              
              n_birds{n_birds_}, 
              box_size{box_size_}, 
              d{d_}, 
              d_s{d_s_}, 
              s{s_}, 
              a{a_}, 
              c{c_}, 
              w{w_},
              max_bird_velocity{max_bird_velocity_}
  // clang-format on
  {
    // class invariant sui parametri
  }
};

class Flock
{
 private:
  FParametres par;

  std::vector<Bird> birds;

  Statistics average_velocity;
  Statistics average_bird_to_bird_distance;
  
  void spawn_birds();
  void calc_bird_velocity(Bird& reference_bird);

 public:
  Flock(FParametres const& flock_parameters);
  int get_n_birds() const;
  void update_birds_position(const double delta_time);

  std::vector<double> get_coordinates_of_axis(const char axis) const; // da rifare se serve
};

Statistics calc_average_velocity_norm(std::vector<Bird> const& birds);

Statistics calc_average_bird_to_bird_distance(std::vector<Bird> const& birds);

void find_birds_within_distance(std::vector<int>& vector_to_fill, Bird const& reference_bird, const double radius_distance, std::vector<Bird> const& birds);

Vector2D separation_rule(Bird const& reference_bird, const double d_s, const double s_factor, std::vector<Bird> const& birds);

Vector2D alignment_rule(Bird const& reference_bird, const double d, const double a_factor, std::vector<Bird> const& birds);

Vector2D calc_mass_center(std::vector<int> const& birds_indexes, std::vector<Bird> const& birds);

Vector2D cohesion_rule(Bird const& reference_bird, const double d, const double c_factor, std::vector<Bird> const& birds);

Vector2D wall_rule(Bird const& reference_bird, const double wall_repulsion_factor, const double box_size);

double calc_bird_to_bird_distance(Bird const& bird1, Bird const& bird2);

#endif