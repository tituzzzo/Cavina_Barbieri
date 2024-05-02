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
  std::vector<Bird*> birds;
  void spawn_birds(int);
 
  void calc_average_velocity();
  void calc_average_bird_to_bird_distance();
  void find_birds_at_distance(double dinstance, std::vector<Bird>& birds_within_distance);
  void calc_bird_velocity(Vector3D& bird_velocity);

 public:
  Flock();
  


};
