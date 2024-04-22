#include <matplot/matplot.h>

int main()
{
  using namespace matplot;

  auto z = {1., 1.1, .1, 4.};
  auto x = {1, 2, 3, 4};
  auto y = {1, 2, 3, 4};

  scatter3(x, y, z, "~");
  view(-30, 10);

  show();
  return 0;
}
