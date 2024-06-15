#include "flock.hpp"
#include "gui.cpp"

int main()
{
  try {
    gp::FlockInputGUI input{500, 260};
    input.get_input();
    fl::Flock flock{fl::FParametres{input.input[0], input.input[1]}};
    const double delta_time{0.001};
    gp::FlockSimulationGUI window{1380, 1000, delta_time,flock};
    window.show();

    return EXIT_SUCCESS;

  } catch (std::runtime_error const& e) {
    std::cerr << e.what() << '\n';

    return EXIT_FAILURE;
  }
}
