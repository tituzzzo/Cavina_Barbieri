#include <matplot/matplot.h>
#include <time.h>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
  using namespace matplot;

  double delta = 0.1;
  for (int i = 0; i <= 300; ++i) {
    //clock_t inizio = clock();
    auto x         = {1, 2, 3, 4};
    auto z         = {1. + i * delta, 1.1 + i * delta, .1, 4. + i * delta};
    auto y         = {1, 2, 3, 4};
    scatter3(x, y, z, "filled");
    view(-40 + i, 30);

    //clock_t fine = clock();
    // std::cout << (double)(fine-inizio)/CLOCKS_PER_SEC << "\n";
    std::string file = "img/fps_";
    for (int j = 0; j < i; j++) {
      /* code */ file += "1";
    }

    //save(file, "png"); //png pesa meno,meglio usarlo
    // std::this_thread::sleep_for(std::chrono::nanoseconds(200000000));
  }

  auto z = {1., 1.1, .1, 4.};
  auto x = {1, 2, 3, 4};
  auto y = {1, 2, 3, 4};

  scatter3(x, y, z, "~");
  view(-30, 10);

  show();
  return 0;
}
