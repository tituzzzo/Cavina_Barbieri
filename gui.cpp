#include "flock.hpp"
#include "statistics.hpp"

#include <SFML/Graphics.hpp>
#include "Gui/Gui.hpp"
#include "Gui/Theme.hpp"

#include <cassert>
#include <string>
#include <vector>

namespace gp { // graphics

class FlockSimulationGUI
{
  const int w;
  const int h;
  const double delta_time;
  double zoom;
  sf::Texture bird_texture;
  std::vector<sf::Sprite> bird_shapes;

  sf::Texture empty_circle;
  std::vector<sf::Sprite> d_radius_shapes;
  sf::Texture empty_circle_2;
  std::vector<sf::Sprite> d_s_radius_shapes;

  bool show_d;
  bool show_d_s;

  const sf::Color sky_color{220, 254, 255};
  sf::Font font;

  fl::Flock& flock;

  fl::Statistics average_bird_to_bird_distance;
  fl::Statistics average_velocity_norm;

 public:
  FlockSimulationGUI(const int w_, const int h_, const double delta_time_ ,fl::Flock& flock_)
      : w{w_}
      , h{h_}
      , delta_time{delta_time_}
      , flock{flock_}
  {
    show_d   = false;
    show_d_s = false;
    zoom = static_cast<double>(h / flock.par_.box_size_);

    if (!bird_texture.loadFromFile("graphics/bird_texture.png")) {
      std::cout << "Could not load bird texture" << std::endl;
      // return 0; errori
    }

    if (!empty_circle.loadFromFile("graphics/empty_circle.png")) {
      std::cout << "Could not load empty_circle texture" << std::endl;
      // return 0; errori
    }

    if (!empty_circle_2.loadFromFile("graphics/empty_circle_2.png")) {
      std::cout << "Could not load empty_circle texture" << std::endl;
      // return 0; errori
    }

    if (!font.loadFromFile("graphics/font.ttf")) {
      // error...
    }

    for (int i{0}; i < flock.par_.n_birds_; ++i) {
      sf::Sprite bird_shape;
      bird_shape.setTexture(bird_texture);
      bird_shape.setOrigin(static_cast<float>(bird_texture.getSize().x / 2), static_cast<float>(bird_texture.getSize().y / 2));
      const float scale{(1.f / static_cast<float>(flock.par_.box_size_)) * 6.f};
      bird_shape.setScale(scale, scale);
      bird_shapes.push_back(bird_shape);

      sf::Sprite d_radius_shape;
      d_radius_shape.setTexture(empty_circle);
      d_radius_shape.setOrigin(static_cast<float>(empty_circle.getSize().x / 2), static_cast<float>(empty_circle.getSize().y / 2));
      d_radius_shapes.push_back(d_radius_shape);

      sf::Sprite d_s_radius_shape;
      d_s_radius_shape.setTexture(empty_circle_2);
      d_s_radius_shape.setOrigin(static_cast<float>(empty_circle_2.getSize().x / 2), static_cast<float>(empty_circle_2.getSize().y / 2));
      d_s_radius_shapes.push_back(d_s_radius_shape);
    }
  }
  void show();
};

class FlockInputGUI
{
  const int w;
  const int h;
  sf::Font font;

 public:
  std::vector<int> input;
  FlockInputGUI(int w_, int h_)
      : w{w_}
      , h{h_}
  {
    if (!font.loadFromFile("graphics/font.ttf")) {
      // error...
    }
  }

  void get_input();
};

//sfml-widgets library code needed to create a menu widget (provided by the creator)
inline sf::Color hex2color(const std::string& hexcolor)
{
  sf::Color color = sf::Color::Black;
  if (hexcolor.size() == 7) // #ffffff
  {
    color.r = static_cast<sf::Uint8>(strtoul(hexcolor.substr(1, 2).c_str(), NULL, 16));
    color.g = static_cast<sf::Uint8>(strtoul(hexcolor.substr(3, 2).c_str(), NULL, 16));
    color.b = static_cast<sf::Uint8>(strtoul(hexcolor.substr(5, 2).c_str(), NULL, 16));
  } else if (hexcolor.size() == 4) // #fff
  {
    color.r = static_cast<sf::Uint8>(strtoul(hexcolor.substr(1, 1).c_str(), NULL, 16) * 17);
    color.g = static_cast<sf::Uint8>(strtoul(hexcolor.substr(2, 1).c_str(), NULL, 16) * 17);
    color.b = static_cast<sf::Uint8>(strtoul(hexcolor.substr(3, 1).c_str(), NULL, 16) * 17);
  }
  return color;
}
struct Theme
{
  sf::Color backgroundColor;
  std::string texturePath;
};
// end sfml-widgets creator code


inline void set_text_settings(sf::Text& text, std::string const& string, unsigned int const char_size, sf::Font& font, sf::Color const& color, fl::Vector2D const& position)
{
  text.setFont(font);
  text.setString(string);
  text.setCharacterSize(char_size);
  text.setFillColor(color);
  text.setPosition(static_cast<float>(position.x_), static_cast<float>(position.y_));
}

inline bool check_given_input_correctness(std::string const& input, const int a, const int b)
{
  int i_input;
  try {
    i_input = std::stoi(input);

  } catch (std::exception& err) {
    return false;
  }
  if (i_input < a || i_input > b) {
    return false;
  } else {
    return true;
  }
}

// create a window to obtain user input (n_birds and box_size)
inline void FlockInputGUI::get_input()
{
  sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(w), static_cast<unsigned int>(h)), "Flock Simulator Input", sf::Style::Close);
  window.setFramerateLimit(60);

  //sfml-widgets library code needed to create a menu widget (provided by the creator)
  Theme defaultTheme = {hex2color("#dddbde"), "third_party/sfml-widgets/demo/texture-default.png"};
  gui::Menu menu(window);
  gui::Theme::loadFont("graphics/font.ttf");
  gui::Theme::loadTexture(defaultTheme.texturePath);
  gui::Theme::textSize                   = 11;
  gui::Theme::click.textColor            = hex2color("#191B18");
  gui::Theme::click.textColorHover       = hex2color("#191B18");
  gui::Theme::click.textColorFocus       = hex2color("#000");
  gui::Theme::input.textColor            = hex2color("#000");
  gui::Theme::input.textColorHover       = hex2color("#000");
  gui::Theme::input.textColorFocus       = hex2color("#000");
  gui::Theme::input.textSelectionColor   = hex2color("#8791AD");
  gui::Theme::input.textPlaceholderColor = hex2color("#8791AD");
  gui::Theme::PADDING                    = 2.f;
  gui::Theme::windowBgColor              = defaultTheme.backgroundColor;
  gui::HBoxLayout* hbox = menu.addHBoxLayout();
  gui::FormLayout* form = hbox->addFormLayout();
  // end sfml-widgets creator code

  menu.setPosition(100, 100);

  sf::Text title;
  set_text_settings(title, "FLOCK SIMULATOR", 70, font, sf::Color::Black, {10, 10});

  // n_birds parameter textbox
  gui::TextBox* nb_textbox = new gui::TextBox();
  nb_textbox->setText(std::to_string(200));
  form->addRow("num of birds (2, 1000):", nb_textbox);

  // box_size parameter textbox
  gui::TextBox* bs_textbox = new gui::TextBox();
  bs_textbox->setText(std::to_string(140));
  form->addRow("box area size (50-200): ", bs_textbox);

  // incorrect input text warning
  sf::Text input_error_text("", font);
  input_error_text.setPosition(100, 200);
  input_error_text.setScale(1, 1);

  // start simulation button
  menu.addButton("Start Simulation", [&]() {
    if (check_given_input_correctness(nb_textbox->getText(), 3, 1000) && check_given_input_correctness(bs_textbox->getText(), 50, 200)) // max bird dipende da box_size
    {
      int n_birds{std::stoi(static_cast<std::string>(nb_textbox->getText()))};
      int box_size{std::stoi(static_cast<std::string>(bs_textbox->getText()))};
      input = {n_birds, box_size};
      window.close();
    } else {
      input_error_text.setString("Wrong inputs. Retry.");
    }
  });

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      // Send events to menu
      menu.onEvent(event);
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sf::Color::White);

    window.draw(title);
    window.draw(input_error_text);
    window.draw(menu);

    window.display();
  }
}

inline void FlockSimulationGUI::show()
{
  sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(w), static_cast<unsigned int>(h)), "Flock Simulation", sf::Style::Close);
  window.setFramerateLimit(60);

  //sfml-widgets library code needed to create a menu widget (provided by the creator)
  Theme defaultTheme = {hex2color("#dddbde"), "third_party/sfml-widgets/demo/texture-default.png"};
  gui::Menu menu(window);
  gui::Theme::loadFont("graphics/font.ttf");
  gui::Theme::loadTexture(defaultTheme.texturePath);
  gui::Theme::textSize                   = 11;
  gui::Theme::click.textColor            = hex2color("#191B18");
  gui::Theme::click.textColorHover       = hex2color("#191B18");
  gui::Theme::click.textColorFocus       = hex2color("#000");
  gui::Theme::input.textColor            = hex2color("#000");
  gui::Theme::input.textColorHover       = hex2color("#000");
  gui::Theme::input.textColorFocus       = hex2color("#000");
  gui::Theme::input.textSelectionColor   = hex2color("#8791AD");
  gui::Theme::input.textPlaceholderColor = hex2color("#8791AD");
  gui::Theme::PADDING                    = 2.f;
  gui::Theme::windowBgColor              = defaultTheme.backgroundColor;
  gui::HBoxLayout* hbox = menu.addHBoxLayout();
  gui::FormLayout* form = hbox->addFormLayout();
  // end sfml-widgets creator code

  menu.setPosition(1015, 10);

  // d parameter slider
  gui::Slider* d_slider = new gui::Slider();
  d_slider->setStep(2);
  d_slider->setValue(static_cast<int>(flock.par_.d_));
  d_slider->setCallback([&]() { flock.par_.d_ = d_slider->getValue(); });
  form->addRow("d distance:", d_slider);

  // d_s parameter slider
  gui::Slider* d_s_slider = new gui::Slider();
  d_s_slider->setStep(2);
  d_s_slider->setValue(static_cast<int>(flock.par_.d_s_));
  d_s_slider->setCallback([&]() { flock.par_.d_s_ = d_s_slider->getValue(); });
  form->addRow("d_s distance:", d_s_slider);

  // s parameter slider
  gui::Slider* s_slider = new gui::Slider();
  s_slider->setStep(2);
  s_slider->setValue(static_cast<int>(flock.par_.s_ * 100));
  s_slider->setCallback([&]() { flock.par_.s_ = 0.01 * s_slider->getValue(); });
  form->addRow("s par:", s_slider);

  // a parameter slider
  gui::Slider* a_slider = new gui::Slider();
  a_slider->setStep(2);
  a_slider->setValue(static_cast<int>(flock.par_.a_ * 100));
  a_slider->setCallback([&]() { flock.par_.a_ = 0.001 * a_slider->getValue(); });
  form->addRow("a par:", a_slider);

  // c parameter slider
  gui::Slider* c_slider = new gui::Slider();
  c_slider->setStep(2);
  c_slider->setValue(static_cast<int>(flock.par_.c_ * 100));
  c_slider->setCallback([&]() { flock.par_.c_ = 0.01 * c_slider->getValue(); });
  form->addRow("c par:", c_slider);

  // max_bird_velocity parameter slider
  gui::Slider* mv_slider = new gui::Slider();
  mv_slider->setStep(5);
  mv_slider->setValue(static_cast<int>((flock.par_.max_bird_velocity_ - 40) * (1. / 5.)));
  mv_slider->setCallback([&]() { flock.par_.max_bird_velocity_ = mv_slider->getValue() * 4 + 100.; });
  form->addRow("max_bird_velocity:", mv_slider);

  // d checkbox
  gui::CheckBox* d_checkbox = new gui::CheckBox();
  d_checkbox->setCallback([&]() {
    if (d_checkbox->isChecked()) {
      show_d = true;
    } else {
      show_d = false;
    }
  });
  form->addRow("show d radius", d_checkbox);

  // d_s checkbox
  gui::CheckBox* d_s_checkbox = new gui::CheckBox();
  d_s_checkbox->setCallback([&]() {
    if (d_s_checkbox->isChecked()) {
      show_d_s = true;
    } else {
      show_d_s = false;
    }
  });
  form->addRow("show d_s radius", d_s_checkbox);

  sf::Text statistics_text;
  sf::Text d_slider_label;
  sf::Text d_s_slider_label;
  sf::Text s_slider_label;
  sf::Text a_slider_label;
  sf::Text c_slider_label;
  sf::Text max_vel_slider_label;

  // creates a line to divide simulation area from the parametres menu
  sf::RectangleShape wall{{5, 1000}};
  wall.setPosition(1000, 0);
  wall.setFillColor(sf::Color::Black);

  using index_t   = std::vector<fl::Bird>::size_type;
  using index_t_2 = std::vector<sf::Sprite>::size_type;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      // Send events to menu
      menu.onEvent(event);
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear(sky_color);
    window.draw(wall);
    window.draw(menu);

    for (fl::Bird const& bird : flock.get_birds()) {
      const int i = bird.get_index();
      bird_shapes[static_cast<index_t>(i)].setPosition(static_cast<float>(bird.get_position().x_ * zoom), static_cast<float>(bird.get_position().y_ * zoom));
      bird_shapes[static_cast<index_t>(i)].setRotation(static_cast<float>(fl::get_bird_direction(flock.get_bird(i), true) + 90.));
      window.draw(bird_shapes[static_cast<index_t>(i)]);

      if (show_d) {
        d_radius_shapes[static_cast<index_t_2>(i)].setPosition(static_cast<float>(bird.get_position().x_ * zoom), static_cast<float>(bird.get_position().y_ * zoom));
        const float scale{static_cast<float>(flock.par_.d_ * zoom / empty_circle.getSize().x)};
        d_radius_shapes[static_cast<index_t_2>(i)].setScale(scale, scale);
        window.draw(d_radius_shapes[static_cast<index_t_2>(i)]);
      }

      if (show_d_s) {
        d_s_radius_shapes[static_cast<index_t_2>(i)].setPosition(static_cast<float>(bird.get_position().x_ * zoom), static_cast<float>(bird.get_position().y_ * zoom));
        const float scale{static_cast<float>(flock.par_.d_s_ * zoom / empty_circle_2.getSize().x)};
        d_s_radius_shapes[static_cast<index_t_2>(i)].setScale(scale, scale);
        window.draw(d_s_radius_shapes[static_cast<index_t_2>(i)]);
      }
    }

    // realtime statistics 
    average_bird_to_bird_distance = fl::calc_average_bird_to_bird_distance(flock.get_birds());
    std::string s;
    s.append("Average bird-bird distance:     ");
    s.append(std::to_string(average_bird_to_bird_distance.mean_));
    s.append(" +/- ");
    s.append(std::to_string(average_bird_to_bird_distance.mean_err_));
    s.append("\n");
    s.append("Average bird velocity:          ");
    average_velocity_norm = fl::calc_average_velocity_norm(flock.get_birds());
    s.append(std::to_string(average_velocity_norm.mean_));
    s.append(" +/- ");
    s.append(std::to_string(average_velocity_norm.mean_err_));

    //update all labels
    set_text_settings(statistics_text, s, 14, font, sf::Color::Black, {1020, 400});
    window.draw(statistics_text);
    set_text_settings(d_slider_label, std::to_string(flock.par_.d_), 11, font, sf::Color::Black, {1315, 15});
    window.draw(d_slider_label);
    set_text_settings(d_s_slider_label, std::to_string(flock.par_.d_s_), 11, font, sf::Color::Black, {1315, 47});
    window.draw(d_s_slider_label);
    set_text_settings(s_slider_label, std::to_string(flock.par_.s_), 11, font, sf::Color::Black, {1315, 79});
    window.draw(s_slider_label);
    set_text_settings(a_slider_label, std::to_string(flock.par_.a_), 11, font, sf::Color::Black, {1315, 109});
    window.draw(a_slider_label);
    set_text_settings(c_slider_label, std::to_string(flock.par_.c_), 11, font, sf::Color::Black, {1315, 141});
    window.draw(c_slider_label);
    set_text_settings(max_vel_slider_label, std::to_string(flock.par_.max_bird_velocity_), 11, font, sf::Color::Black, {1310, 173});
    window.draw(max_vel_slider_label);

    window.display();

    // updates flock in order to render next frame
    flock.update_birds_position(delta_time);
  }
}

} // namespace gp