#include "flock.hpp"
#include <matplot/matplot.h>
#include <iostream>

namespace mp = matplot;

int main()
{
  int n_birds{100};
  Flock flock{n_birds};
  for (int i = 0; i <= 1800; ++i){
    //auto plot1 = mp::subplot({0.25f, 0.45f, 0.5f, 0.5f});
    auto x = flock.get_coordinates_of_axis('x');
    auto y = flock.get_coordinates_of_axis('y');
    auto z = flock.get_coordinates_of_axis('z');
    mp::scatter3(x, y, z, "filled");
    
    /*
    auto plot2 = mp::subplot({0.1f, 0.1f, 0.25f, 0.25f});
    mp::scatter(x, y);
    //mp::title("Subplot 1: sin(x)");

    auto plot3 = mp::subplot({0.65f, 0.1f, 0.25f, 0.25f});
    mp::scatter(x, z);
    //mp::title("Subplot 2: sin(2x)");
    mp::save("img/barchart_gif", "pdf");
    */
    mp::view(-40, 30);
    flock.update_birds_position(0.001);
    mp::show();

  }

  return 0;
}
