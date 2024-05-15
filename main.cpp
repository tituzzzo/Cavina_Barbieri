#include "flock.hpp"
#include <matplot/matplot.h>
#include <iostream>

namespace mp = matplot;

int main()
{
  Flock flock{100};
  for (int i = 0; i <= 10; ++i) {
    auto plot1 = mp::subplot({0.25f, 0.45f, 0.5f, 0.5f});
    auto x = flock.get_coordinates_of_axis('x');
    auto y = flock.get_coordinates_of_axis('y');
    auto z = flock.get_coordinates_of_axis('z');
    mp::scatter3(x, y, z, "filled");

    auto plot2 = mp::subplot({0.1f, 0.1f, 0.25f, 0.25f});
    mp::fplot("sin(x)");
    //mp::title("Subplot 1: sin(x)");

    auto plot3 = mp::subplot({0.65f, 0.1f, 0.25f, 0.25f});
    mp::fplot("sin(2*x)");
    //mp::title("Subplot 2: sin(2x)");
    
    //mp::view(-40, 30);

    
  }

  return 0;
}
