#include "bird.hpp"
#include <vector>

#include "statistics.hpp"

class Flock
{
 private:
  const int n_birds;
  const double box_size;
  double d{10.}; //const
  double d_s{2.}; //const
  double a{0.001};//const
  double c{0.4}; //const
  double s{0.6}; //const
  Statistics average_velocity;
  Statistics average_bird_to_bird_distance;
  std::vector<Bird> birds; // problema: può essere modificato l'ordine compromettendo il programma; accedere ad index da bird
  void spawn_birds();
  Bird& get_bird(const int index);
  // statistics

  void calc_average_velocity_norm();
  void calc_average_bird_to_bird_distance();
  // functions
  Vector3D separation_rule(Bird const& reference_bird) const;
  Vector3D alignment_rule(Bird const& reference_bird) const;
  Vector3D calc_mass_center(std::vector<int> const& birds_indexes) const;
  Vector3D cohesion_rule(Bird const& reference_bird) const;
  Vector3D cohesion_rule(Bird const& reference_bird) const;
  void calc_bird_velocity(Bird& reference_bird);
  std::vector<int> find_birds_within_distance(const double distance, Bird const& reference_bird) const;

 public:
  Flock(const int n_birds_, const double box_size);
  void update_birds_position(const double delta_time);

  std::vector<double> get_coordinates_of_axis(const char axis) const;
};

double calc_bird_to_bird_distance(Bird const& bird1, Bird const& bird2);
