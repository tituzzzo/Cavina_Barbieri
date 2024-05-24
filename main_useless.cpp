#include "flock.hpp"
#include <matplot/matplot.h>
#include <iostream>

namespace mp = matplot;

int main()
{
  int n_birds{100};
  Flock flock{n_birds, 15};
  //Flock flock2{50, 15};
  for (int i = 0; i <= 1800; ++i){
    //auto plot1 = mp::subplot({0.25f, 0.45f, 0.5f, 0.5f});
    auto x = flock.get_coordinates_of_axis('x');
    auto y = flock.get_coordinates_of_axis('y');
    
    //mp::scatter3(x, y, z, "filled");
    
    //mp::xlim({-10, 10});
    //mp::ylim({-10, 10});
    //mp::zlim({-10, 10});
    
    //auto x2 = flock2.get_coordinates_of_axis('x');
    //auto y2 = flock2.get_coordinates_of_axis('y');
    
    //for(double &pos:x2){x.push_back(pos);}
    //for(double &pos:y2){y.push_back(pos);}
    auto plot2 = mp::subplot({0.01f, 0.1f, 0.8f, 0.8f});
    mp::scatter(x, y);
    //mp::title("Subplot 1: sin(x)");
    mp::xlim({-10, 10});
    mp::ylim({-10, 10});

    //auto plot3 = mp::subplot({0.6f, 0.1f, 0.4f, 0.4f});
    //mp::scatter(x, z);
    //mp::title("Subplot 2: sin(2x)");
    //mp::save("img/barchart_gif", "pdf");
  
    mp::view(-10, 10); //-40 30
    flock.update_birds_position(0.02);
    //flock2.update_birds_position(0.02);

  }

  return 0;
}
