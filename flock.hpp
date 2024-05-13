#include "bird.hpp"
#include <vector>

#include "statistics.hpp"

class Flock
{
 private:
  const double d;
  const double d_s;
  const double a;
  const double c;
  const double s;
  Statistics average_velocity;
  Statistics average_bird_to_bird_distance;
  std::vector<Bird> birds; //potrebbe essere const???????????????????????????????????????????? poi lo spawn lo fai nel costruttore
  void spawn_birds(int);
  Bird& get_bird(int index);
  //statistics
  
  void calc_average_velocity_norm();
  void calc_average_bird_to_bird_distance();
  //functions
  Vector3D separation_rule(Bird const& reference_bird) const;
  Vector3D alignment_rule(Bird const& reference_bird) const;
  Vector3D calc_mass_center(std::vector<int> const& birds_indexes) const;
  Vector3D cohesion_rule(Bird const& reference_bird) const;
  void calc_bird_velocity();
  std::vector<int> find_birds_within_distance(double distance, const Bird& reference_bird) const;
  

 public:
  Flock();
  //metodo aggiorna flock?
};

double calc_bird_to_bird_distance(const Bird& bird1, const Bird& bird2);
