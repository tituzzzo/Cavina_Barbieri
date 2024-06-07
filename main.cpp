#include "flock.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <iostream>
#include <vector>

Flock flock{FParametres{160, 80.}};

class Image
{
  sf::Image data_;

 public:
  Image(int w, int h)
  {
    assert(w > 0 && h > 0);
    data_.create(w, h, sf::Color::White);
  }
  void render_bird(Vector2D position)
  {
    //?
  }
  void show() const;
};

void Image::show() const
{
  sf::RenderWindow window(sf::VideoMode(data_.getSize().x, data_.getSize().y), "Boids");
  window.setFramerateLimit(60);

  std::vector<sf::CircleShape> birds;
  for (size_t i = 0; i < flock.get_n_birds(); i++) {
    sf::CircleShape shape(2);
    birds.push_back(shape);
  }

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();

    flock.update_birds_position(0.002);
    std::vector<double> birds_x{flock.get_coordinates_of_axis('x')};
    std::vector<double> birds_y{flock.get_coordinates_of_axis('y')};

    for (size_t i = 0; i < flock.get_n_birds(); i++) {
      birds[i].setPosition((birds_x[i] * 10) + 100, (birds_y[i] * 10) + 100);
      window.draw(birds[i]);
    }

    window.display();
  }
}

int main()
{
  auto const x_min  = -2.;
  auto const x_max  = 1.;
  auto const y_min  = -1.;
  auto const y_max  = 1.;
  auto const width  = 1000;
  auto const height = 1000;
  auto const dx     = (x_max - x_min) / width;
  auto const dy     = (y_max - y_min) / height;

  Image image{width, height};

  image.show();
}
