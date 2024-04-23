#include <matplot/matplot.h>
#include<chrono>
#include<thread>


int main()
{
  using namespace matplot;

  double delta = 0.1;
  for (int i = 0; i <= 300; ++i) {
    auto z = {1. + i * delta, 1.1 + i * delta, .1, 4. + i * delta};
    auto x = {1, 2, 3, 4};
    auto y = {1, 2, 3, 4};

    scatter3(x, y, z, "filled");
    view(-40 + i, 30);
    //std::this_thread::sleep_for(std::chrono::nanoseconds(200000000));
  }

  auto z = {1., 1.1, .1, 4.};
  auto x = {1, 2, 3, 4};
  auto y = {1, 2, 3, 4};

  scatter3(x, y, z, "~");
  view(-30, 10);
  aaaaaaaaaaaaaaaaaaaaaaaa

  show();
  return 0;
}
