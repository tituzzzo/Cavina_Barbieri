#include "flock.hpp"
#include "statistics.hpp"
#include "text_table.hpp"

#include <Gui/Gui.hpp>
#include <Gui/Theme.hpp>
#include <SFML/Graphics.hpp>

#include <ctype.h>
#include <array>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>

#include <iostream>
#include <stdexcept>

namespace gp { // graphics

class FlockSimulationGUI
{
  const int w_;
  const int h_;
  const double delta_time_;
  double zoom_;
  sf::Texture bird_texture_;
  std::vector<sf::Sprite> bird_shapes_;

  sf::Texture empty_circle;
  std::vector<sf::Sprite> d_radius_shapes_;
  sf::Texture empty_circle_2;
  std::vector<sf::Sprite> d_s_radius_shapes_;

  bool show_d_;
  bool show_d_s_;

  const sf::Color sky_color{220, 254, 255};
  sf::Font font_;

  fl::Flock& flock_;

  fl::Statistics average_bird_to_bird_distance_;
  fl::Statistics average_velocity_norm_;

 public:
  FlockSimulationGUI(const int w, const int h, const double delta_time, fl::Flock& flock)
      : w_{w}
      , h_{h}
      , delta_time_{delta_time}
      , flock_{flock}
  {
    show_d_   = false;
    show_d_s_ = false;
    zoom_     = static_cast<double>(h_ / flock_.par_.box_size_);

    if (!bird_texture_.loadFromFile("graphics/bird_texture.png")) {
      throw std::runtime_error{"Could not load bird_texture texture."};
    }

    if (!empty_circle.loadFromFile("graphics/empty_circle.png")) {
      throw std::runtime_error{"Could not load empty_circle texture."};
    }

    if (!empty_circle_2.loadFromFile("graphics/empty_circle_2.png")) {
      throw std::runtime_error{"Could not load empty_circle_2 texture."};
    }

    if (!font_.loadFromFile("graphics/font.ttf")) {
      throw std::runtime_error{"Could not load font."};
    }

    for (int i{0}; i < flock_.par_.n_birds_; ++i) {
      sf::Sprite bird_shape;
      bird_shape.setTexture(bird_texture_);
      bird_shape.setOrigin(static_cast<float>(bird_texture_.getSize().x / 2), static_cast<float>(bird_texture_.getSize().y / 2));
      const float scale{static_cast<float>(0.01 * zoom_)};
      bird_shape.setScale(scale, scale);
      bird_shapes_.push_back(bird_shape);

      sf::Sprite d_radius_shape;
      d_radius_shape.setTexture(empty_circle);
      d_radius_shape.setOrigin(static_cast<float>(empty_circle.getSize().x / 2), static_cast<float>(empty_circle.getSize().y / 2));
      d_radius_shapes_.push_back(d_radius_shape);

      sf::Sprite d_s_radius_shape;
      d_s_radius_shape.setTexture(empty_circle_2);
      d_s_radius_shape.setOrigin(static_cast<float>(empty_circle_2.getSize().x / 2), static_cast<float>(empty_circle_2.getSize().y / 2));
      d_s_radius_shapes_.push_back(d_s_radius_shape);
    }
  }
  void update_shapes_positions(sf::RenderWindow& window);
  void show();
};

class FlockInputGUI
{
  const int w_;
  const int h_;
  sf::Font font_;

 public:
  std::vector<int> input_;
  FlockInputGUI(int w, int h)
      : w_{w}
      , h_{h}
  {
    if (!font_.loadFromFile("graphics/font.ttf")) {
      throw std::runtime_error{"Could not load font."};
    }
  }

  void get_input();
};

// sfml-widgets library code needed to create a menu widget (provided by the creator)
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

// use precision = 100 to print two decimal digits
inline void round_two_dec_digits(std::string& input_string, double value)
{
  value = std::round(value * 100) / 100;
  std::string s{std::to_string(value)};
  for (size_t i{0}; i < s.size(); ++i) {
    if (s[i] == '.') {
      input_string.push_back(s[i]);
      input_string.push_back(s[i + 1]);
      input_string.push_back(s[i + 2]);
      break;
    } else {
      input_string.push_back(s[i]);
    }
  }
}

inline void set_text_settings(sf::Text& text, std::string const& string, unsigned int const char_size, sf::Font const& font, sf::Color const& color, fl::Vector2D const& position)
{
  text.setFont(font);
  text.setString(string);
  text.setCharacterSize(char_size);
  text.setFillColor(color);
  text.setPosition(static_cast<float>(position.x_), static_cast<float>(position.y_));
}

inline bool check_given_input_correctness(std::string const& input, const int a, const int b)
{
  for (size_t i{0}; i < input.size(); ++i) {
    if (isdigit(input[i]) == 0) {
      return false;
    }
  }

  int i_input{std::stoi(input)};
  if (i_input < a || i_input > b) {
    return false;
  } else {
    return true;
  }
}

// create a window to obtain user input (n_birds and box_size)
inline void FlockInputGUI::get_input()
{
  // create window
  sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(w_), static_cast<unsigned int>(h_)), "Flock Simulator Input", sf::Style::Close);
  window.setFramerateLimit(60);

  // sfml-widgets library code needed to create a menu widget (provided by the creator)
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
  gui::HBoxLayout* hbox                  = menu.addHBoxLayout();
  gui::FormLayout* form                  = hbox->addFormLayout();
  // end sfml-widgets creator code

  menu.setPosition(100, 100);

  sf::Text title;
  set_text_settings(title, "FLOCK SIMULATOR", 70, font_, sf::Color::Black, {10, 10});

  // n_birds parameter textbox
  gui::TextBox* nb_textbox = new gui::TextBox();
  nb_textbox->setText(std::to_string(200));
  form->addRow("num of birds (3-1000):", nb_textbox);

  // box_size parameter textbox
  gui::TextBox* bs_textbox = new gui::TextBox();
  bs_textbox->setText(std::to_string(140));
  form->addRow("box area size (50-200): ", bs_textbox);

  // incorrect input text warning
  sf::Text input_error_text("", font_);
  input_error_text.setFillColor(sf::Color::Red);
  input_error_text.setPosition(100, 200);
  input_error_text.setScale(1, 1);

  // start simulation button
  menu.addButton("Start Simulation", [&]() {
    std::string nb_text = nb_textbox->getText().toAnsiString();
    std::string bs_text = bs_textbox->getText().toAnsiString();
    if (check_given_input_correctness(nb_text, 3, 1000) && check_given_input_correctness(bs_text, 50, 200)) {
      int n_birds{std::stoi(nb_text)};
      int box_size{std::stoi(bs_text)};
      input_ = {n_birds, box_size};
      window.close();
    } else {
      input_error_text.setString("Wrong input. Retry.");
    }
  });

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      // Send events to menu
      menu.onEvent(event);
      if (event.type == sf::Event::Closed) {
        window.close();
        delete nb_textbox;
        delete bs_textbox;
      }
    }

    window.clear(sf::Color::White);

    window.draw(title);
    window.draw(input_error_text);
    window.draw(menu);

    window.display();
  }
}

inline void FlockSimulationGUI::update_shapes_positions(sf::RenderWindow& window)
{
  using index_t = std::vector<sf::Sprite>::size_type;

  for (fl::Bird const& bird : flock_.get_birds()) {
    // update bird_shapes positions
    const int i = bird.get_index();
    bird_shapes_[static_cast<index_t>(i)].setPosition(static_cast<float>(bird.get_position().x_ * zoom_), static_cast<float>(bird.get_position().y_ * zoom_));
    bird_shapes_[static_cast<index_t>(i)].setRotation(static_cast<float>(fl::get_bird_direction(flock_.get_bird(i), true) + 90.));
    window.draw(bird_shapes_[static_cast<index_t>(i)]);

    if (show_d_) {
      // update d_radius_shapes positions
      d_radius_shapes_[static_cast<index_t>(i)].setPosition(static_cast<float>(bird.get_position().x_ * zoom_), static_cast<float>(bird.get_position().y_ * zoom_));
      const float scale{static_cast<float>(flock_.par_.d_ * zoom_ / empty_circle.getSize().x)};
      d_radius_shapes_[static_cast<index_t>(i)].setScale(scale, scale);
      window.draw(d_radius_shapes_[static_cast<index_t>(i)]);
    }

    if (show_d_s_) {
      // update d_radius_shapes positions
      d_s_radius_shapes_[static_cast<index_t>(i)].setPosition(static_cast<float>(bird.get_position().x_ * zoom_), static_cast<float>(bird.get_position().y_ * zoom_));
      const float scale{static_cast<float>(flock_.par_.d_s_ * zoom_ / empty_circle_2.getSize().x)};
      d_s_radius_shapes_[static_cast<index_t>(i)].setScale(scale, scale);
      window.draw(d_s_radius_shapes_[static_cast<index_t>(i)]);
    }
  }
}

inline void FlockSimulationGUI::show()
{
  // create window
  sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(w_), static_cast<unsigned int>(h_)), "Flock Simulation", sf::Style::Close);
  window.setFramerateLimit(60);

  // sfml-widgets library code needed to create a menu widget (provided by the creator)
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
  gui::HBoxLayout* hbox                  = menu.addHBoxLayout();
  gui::FormLayout* form                  = hbox->addFormLayout();
  // end sfml-widgets creator code

  menu.setPosition(1015, 10);

  // d parameter slider
  gui::Slider* d_slider = new gui::Slider();
  d_slider->setStep(2);
  d_slider->setValue(static_cast<int>(flock_.par_.d_));
  // set maximum d value to half the box_size
  const double max_d_s = sqrt(pow(flock_.par_.box_size_, 2) / static_cast<float>(flock_.par_.n_birds_));
  d_slider->setCallback([&]() { flock_.par_.d_ = max_d_s + d_slider->getValue() * (flock_.par_.box_size_ * 0.005); });
  form->addRow("d distance:", d_slider);

  // d_s parameter slider
  gui::Slider* d_s_slider = new gui::Slider();
  d_s_slider->setStep(2);
  d_s_slider->setValue(static_cast<int>(flock_.par_.d_s_));
  // set maximum d_s value to sqrt(box_size^2 / n_birds)
  d_s_slider->setCallback([&]() { flock_.par_.d_s_ = d_s_slider->getValue() * 0.01 * max_d_s; });
  form->addRow("d_s distance:", d_s_slider);

  // s parameter slider
  gui::Slider* s_slider = new gui::Slider();
  s_slider->setStep(2);
  s_slider->setValue(static_cast<int>(flock_.par_.s_ * 100));
  s_slider->setCallback([&]() { flock_.par_.s_ = 0.01 * s_slider->getValue(); });
  form->addRow("s par:", s_slider);

  // a parameter slider
  gui::Slider* a_slider = new gui::Slider();
  a_slider->setStep(2);
  a_slider->setValue(static_cast<int>(flock_.par_.a_ * 100));
  a_slider->setCallback([&]() { flock_.par_.a_ = 0.001 * a_slider->getValue(); });
  form->addRow("a par:", a_slider);

  // c parameter slider
  gui::Slider* c_slider = new gui::Slider();
  c_slider->setStep(2);
  c_slider->setValue(static_cast<int>(flock_.par_.c_ * 100));
  c_slider->setCallback([&]() { flock_.par_.c_ = 0.01 * c_slider->getValue(); });
  form->addRow("c par:", c_slider);

  // max_bird_velocity parameter slider
  gui::Slider* mv_slider = new gui::Slider();
  mv_slider->setStep(5);
  mv_slider->setValue(static_cast<int>((flock_.par_.max_bird_velocity_ - 40) * (1. / 5.)));
  mv_slider->setCallback([&]() { flock_.par_.max_bird_velocity_ = mv_slider->getValue() * 4. + 100.; });
  form->addRow("max_bird_velocity:", mv_slider);

  // d checkbox
  gui::CheckBox* d_checkbox = new gui::CheckBox();
  d_checkbox->setCallback([&]() {
    if (d_checkbox->isChecked()) {
      show_d_ = true;
    } else {
      show_d_ = false;
    }
  });
  form->addRow("show d radius", d_checkbox);

  // d_s checkbox
  gui::CheckBox* d_s_checkbox = new gui::CheckBox();
  d_s_checkbox->setCallback([&]() {
    if (d_s_checkbox->isChecked()) {
      show_d_s_ = true;
    } else {
      show_d_s_ = false;
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

  sf::RectangleShape menu_background{{380, 1000}};
  menu_background.setPosition(1000, 0);
  menu_background.setFillColor(sf::Color::White);

  // create a text table to store data in a .txt file
  std::vector<std::string> columns{"Average Velocity", "Average distance", "Time"};
  tb::table_file_management text_file("statistics_at_time_table.txt", columns);
  text_file.create_table();
  double time{0.};

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
    window.draw(menu_background);
    window.draw(wall);
    window.draw(menu);

    update_shapes_positions(window);

    // realtime statistics on window and statistics data saved on a .txt file
    average_bird_to_bird_distance_ = fl::calc_average_bird_to_bird_distance(flock_.get_birds());
    average_velocity_norm_         = fl::calc_average_velocity_norm(flock_.get_birds());

    std::string window_string;
    std::string average_distance;
    std::string average_velocity;
    std::vector<std::string> row_to_insert_into_table;

    round_two_dec_digits(average_distance, average_bird_to_bird_distance_.mean_);
    average_distance.append(" +/- ");
    round_two_dec_digits(average_distance, average_bird_to_bird_distance_.mean_err_);
    row_to_insert_into_table.push_back(average_distance);

    window_string.append("Average bird-bird distance:     ");
    window_string.append(average_distance);
    window_string.append("\n");

    round_two_dec_digits(average_velocity, average_velocity_norm_.mean_);
    average_velocity.append(" +/- ");
    round_two_dec_digits(average_velocity, average_velocity_norm_.mean_err_);
    row_to_insert_into_table.push_back(average_velocity);

    window_string.append("Average bird velocity:          ");
    window_string.append(average_velocity);

    time += delta_time_;
    std::string total_time{std::to_string(time)};
    row_to_insert_into_table.push_back(total_time);

    text_file.insert_row_into_table(row_to_insert_into_table);

    // update all labels
    set_text_settings(statistics_text, window_string, 20, font_, sf::Color::Black, {1020, 400});
    window.draw(statistics_text);
    set_text_settings(d_slider_label, std::to_string(flock_.par_.d_), 11, font_, sf::Color::Black, {1315, 15});
    window.draw(d_slider_label);
    set_text_settings(d_s_slider_label, std::to_string(flock_.par_.d_s_), 11, font_, sf::Color::Black, {1315, 47});
    window.draw(d_s_slider_label);
    set_text_settings(s_slider_label, std::to_string(flock_.par_.s_), 11, font_, sf::Color::Black, {1315, 79});
    window.draw(s_slider_label);
    set_text_settings(a_slider_label, std::to_string(flock_.par_.a_), 11, font_, sf::Color::Black, {1315, 109});
    window.draw(a_slider_label);
    set_text_settings(c_slider_label, std::to_string(flock_.par_.c_), 11, font_, sf::Color::Black, {1315, 141});
    window.draw(c_slider_label);
    set_text_settings(max_vel_slider_label, std::to_string(flock_.par_.max_bird_velocity_), 11, font_, sf::Color::Black, {1310, 173});
    window.draw(max_vel_slider_label);

    window.display();

    // updates flock in order to render next frame
    flock_.update_birds_position(delta_time_);
  }
}

} // namespace gp