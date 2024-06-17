#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "third_party/doctest.h"
#include <vector>

// file to test
#include "bird.hpp"
#include "flock.hpp"
#include "gui.hpp"
#include "vector2d.hpp"

TEST_CASE("Testing Vector2D")
{
  SUBCASE("Testing norm function")
  {
    fl::Vector2D v1{1., 2.};
    fl::Vector2D v2{1., 3.};
    CHECK(fl::Vector2D{1., 2.}.norm() == doctest::Approx(2.24).epsilon(0.01));
    CHECK(fl::Vector2D{1., 1.}.norm() == doctest::Approx(1.41).epsilon(0.01));
    CHECK(fl::Vector2D{0., 0.}.norm() == 0.0);
    CHECK(fl::Vector2D{-1., 1.}.norm() == doctest::Approx(1.41).epsilon(0.01));
    CHECK(fl::Vector2D{-1., -1.}.norm() == doctest::Approx(1.41).epsilon(0.01));
  }
}

TEST_CASE("Testing Bird")
{
  SUBCASE("Testing != operator")
  {
    fl::Bird b1{1, fl::Vector2D{1., 1.}};
    fl::Bird b2{2, fl::Vector2D{2., 2.}};
    CHECK(b1 != b2);
  }
}

TEST_CASE("Testing calc_bird_to_bird_distance function in flock.hpp")
{
  fl::Bird b1{1, fl::Vector2D{1., 1.}};
  fl::Bird b2{2, fl::Vector2D{2., 2.}};
  CHECK(calc_bird_to_bird_distance(b1, b2)
        == doctest::Approx(1.41).epsilon(0.01));
  fl::Vector2D new_pos{1., 1.};
  b2.set_position(new_pos);
  CHECK(calc_bird_to_bird_distance(b1, b2) == 0.0);
}

TEST_CASE("Testing flock.hpp functions")
{
  fl::Bird bird0{0, {1., 1.}};
  fl::Bird bird1{1, {3., 1.}};
  fl::Bird bird2{2, {4., 4.}};
  fl::Bird bird3{3, {1., 4.}};
  bird0.set_velocity({2., 2.});
  bird1.set_velocity({2., 2.});
  bird2.set_velocity({-1., -2.});
  bird3.set_velocity({0., 0.});

  std::vector<fl::Bird> flock_test{bird0, bird1, bird2, bird3};
  REQUIRE(flock_test.size() == 4);

  SUBCASE("Testing separation_rule function")
  {
    double s_factor = 1.;
    CHECK(fl::Vector2D{-5, 0.}
          == separation_rule(bird0, 2.5, s_factor, flock_test));
    s_factor = 0.5;
    CHECK(fl::Vector2D{-2.5, 0.}
          == separation_rule(bird0, 2.5, s_factor, flock_test));
    s_factor = 1.;
    fl::Vector2D temp{separation_rule(bird0, 3., s_factor, flock_test)};
    CHECK(temp.x_ == -5.);
    CHECK(temp.y_ == doctest::Approx(-3.33).epsilon(0.01));

    temp = separation_rule(bird0, 5., s_factor, flock_test);
    CHECK(temp.x_ == doctest::Approx(-6.66).epsilon(0.01));
    CHECK(temp.y_ == -5.);
  }

  SUBCASE("Testing alignment_rule function")
  {
    double a_factor = 1.;
    CHECK(fl::Vector2D{0., 0.}
          == alignment_rule(bird0, 2.5, a_factor, flock_test));
    CHECK(fl::Vector2D{-1., -1.}
          == alignment_rule(bird0, 3., a_factor, flock_test));

    fl::Vector2D alignment_velocity{};
    alignment_velocity = alignment_rule(bird0, 5., a_factor, flock_test);
    CHECK(alignment_velocity.x_ == doctest::Approx(-1.66).epsilon(0.01));
    CHECK(alignment_velocity.y_ == -2.);

    a_factor           = 0.5;
    alignment_velocity = alignment_rule(bird0, 5., a_factor, flock_test);
    CHECK(alignment_velocity.x_ == doctest::Approx(-0.83).epsilon(0.01));
    CHECK(alignment_velocity.y_ == -1.);
  }

  SUBCASE("Testing mass_center function")
  {
    fl::Vector2D mass_center_calc{};
    mass_center_calc = calc_mass_center(bird0, 2.5, flock_test);
    CHECK(fl::Vector2D{3, 1} == mass_center_calc);

    mass_center_calc = calc_mass_center(bird0, 3.5, flock_test);
    CHECK(fl::Vector2D{2, 2.5} == mass_center_calc);

    mass_center_calc = calc_mass_center(bird0, 5., flock_test);
    CHECK(mass_center_calc.x_ == doctest::Approx(2.66).epsilon(0.01));
    CHECK(mass_center_calc.y_ == 3.);
  }

  SUBCASE("Testing cohesion_rule function")
  {
    double c_factor = 1.;
    CHECK(fl::Vector2D{2., 0.}
          == cohesion_rule(bird0, 2.5, c_factor, flock_test));
    CHECK(fl::Vector2D{1., 1.5}
          == cohesion_rule(bird0, 3., c_factor, flock_test));

    fl::Vector2D cohesion_velocity{};
    cohesion_velocity = cohesion_rule(bird0, 5., c_factor, flock_test);
    CHECK(cohesion_velocity.x_ == doctest::Approx(1.66).epsilon(0.01));
    CHECK(cohesion_velocity.y_ == 2.);

    c_factor          = 0.5;
    cohesion_velocity = cohesion_rule(bird0, 5., c_factor, flock_test);
    CHECK(cohesion_velocity.x_ == doctest::Approx(0.83).epsilon(0.01));
    CHECK(cohesion_velocity.y_ == 1.);
  }

  SUBCASE("Testing limit_bird_velocity_to_value function")
  {
    double max_vel{300.};
    fl::Vector2D too_fast_bird_vel{500., 500.};

    limit_bird_velocity_to_value(too_fast_bird_vel, max_vel);
    CHECK(too_fast_bird_vel.norm() == max_vel);

    too_fast_bird_vel = {1000., 1000.};
    limit_bird_velocity_to_value(too_fast_bird_vel, max_vel);
    CHECK(too_fast_bird_vel.norm() == max_vel);

    too_fast_bird_vel = {300., 300.};
    limit_bird_velocity_to_value(too_fast_bird_vel, max_vel);
    CHECK(too_fast_bird_vel.norm() == max_vel);

    too_fast_bird_vel = {0., 0.};
    limit_bird_velocity_to_value(too_fast_bird_vel, max_vel);
    CHECK(too_fast_bird_vel.norm() == 0.);
  }

  SUBCASE("Testing calc_average_velocity_norm function")
  {
    fl::Statistics average_velocity_stat{
        calc_average_velocity_norm(flock_test)};
    CHECK(average_velocity_stat.mean_ == doctest::Approx(1.97).epsilon(0.01));
    CHECK(average_velocity_stat.sigma_ == doctest::Approx(1.35).epsilon(0.01));
    CHECK(average_velocity_stat.mean_err_
          == doctest::Approx(0.67).epsilon(0.01));
  }

  SUBCASE("Testing calc_average_bird_to_bird_distance function")
  {
    fl::Statistics average_distance_stat{
        calc_average_bird_to_bird_distance(flock_test)};
    CHECK(average_distance_stat.mean_ == doctest::Approx(3.17).epsilon(0.01));
    CHECK(average_distance_stat.sigma_ == doctest::Approx(0.74).epsilon(0.01));
    CHECK(average_distance_stat.mean_err_
          == doctest::Approx(0.30).epsilon(0.01));
  }
}

TEST_CASE("Testing gui.cpp function")
{
  SUBCASE("Testing round_two_dec_digits function")
  {
    std::string test;
    double value{123.1234};
    gp::round_two_dec_digits(test, value);
    CHECK(test == "123.12");

    value = 123.567;
    test.clear();
    gp::round_two_dec_digits(test, value);
    CHECK(test == "123.57");

    value = 123.000;
    test.clear();
    gp::round_two_dec_digits(test, value);
    CHECK(test == "123.00");

    value = 0.000;
    test.clear();
    gp::round_two_dec_digits(test, value);
    CHECK(test == "0.00");
  }

  SUBCASE("Testing check_given_input_correctness function")
  {
    CHECK(gp::check_given_input_correctness("2", 1, 3));
    CHECK(gp::check_given_input_correctness("1", 1, 3));
    CHECK(gp::check_given_input_correctness("3", 1, 3));
    CHECK(!gp::check_given_input_correctness("1", 2, 3));
    CHECK(!gp::check_given_input_correctness("3", 1, 2));
  }
}
