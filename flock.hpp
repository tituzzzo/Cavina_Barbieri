#include "bird.hpp"
#include <vector>

#include "statistics.hpp"

class Flock
{
 private:
  double d;
  double d_s;
  float a;
  float c;
  float s;
  Statistics average_velocity;
  Statistics average_bird_to_bird_distance;
  std::vector<Bird> birds;
  void spawn_birds(int);

  void calc_average_velocity();
  void calc_average_bird_to_bird_distance();
  void find_birds_within_distance(double distance, Bird& reference_bird) const;
  

 public:
  Flock();
};
double calc_bird_to_bird_distance(const Bird& bird1, const Bird& bird2);
