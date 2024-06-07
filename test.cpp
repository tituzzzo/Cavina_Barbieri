#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include <iostream>
#include <vector>
// file to test
#include "bird.hpp"
#include "flock.hpp"
#include "vector2d.hpp"

// REQUIRE controllo su variabili locali propedeutiche al test
// CHECK per testare ritnorni di funzione
// CHECK_THROWS per testare error handling

TEST_CASE("Testing Vector2D")
{
  SUBCASE("Testing norm function")
  {
    Vector2D v1{1., 2.};
    Vector2D v2{1., 3.};
    CHECK(Vector2D{1., 2.}.norm() == doctest::Approx(2.24).epsilon(0.01));
    CHECK(Vector2D{1., 1.}.norm() == doctest::Approx(1.41).epsilon(0.01));
    CHECK(Vector2D{0., 0.}.norm() == 0.0);
    CHECK(Vector2D{-1., 1.}.norm() == doctest::Approx(1.41).epsilon(0.01));
    CHECK(Vector2D{-1., -1.}.norm() == doctest::Approx(1.41).epsilon(0.01));
  }
}

TEST_CASE("Testing Bird")
{
  SUBCASE("Testing != operator")
  {
    Bird b1{1, Vector2D{1., 1.}};
    Bird b2{2, Vector2D{2., 2.}};
    CHECK(b1 != b2);
  }
}

TEST_CASE("Testing calc_bird_to_bird_distance function in flock.hpp")
{
  Bird b1{1, Vector2D{1., 1.}};
  Bird b2{2, Vector2D{2., 2.}};
  CHECK(calc_bird_to_bird_distance(b1, b2) == doctest::Approx(1.41).epsilon(0.01));
  Vector2D new_pos{1., 1.};
  b2.set_position(new_pos);
  CHECK(calc_bird_to_bird_distance(b1, b2) == 0.0);
}

TEST_CASE("Testing flock.hpp functions")
{
  Bird bird0{0, {1., 1.}};
  Bird bird1{1, {3., 1.}};
  Bird bird2{2, {4., 4.}};
  Bird bird3{3, {1., 4.}};
  bird0.set_velocity({2., 2.});
  bird1.set_velocity({2., 2.});
  bird2.set_velocity({-1., -2.});
  bird3.set_velocity({0., 0.});

  std::vector<Bird> flock_test{bird0, bird1, bird2, bird3};
  REQUIRE(flock_test.size() == 4);

  SUBCASE("Testing find_birds_within_distance function")
  {
    std::vector<int> birds_within_distance;
    find_birds_within_distance(birds_within_distance, bird0, 2.5, flock_test);
    CHECK(birds_within_distance.size() == 1);
    birds_within_distance.clear();
    find_birds_within_distance(birds_within_distance, bird0, 3., flock_test);
    CHECK(birds_within_distance.size() == 2);
    birds_within_distance.clear();
    find_birds_within_distance(birds_within_distance, bird0, 5., flock_test);
    CHECK(birds_within_distance.size() == 3);
  }

  SUBCASE("Testing separation_rule function")
  {
    double s_factor = 1.;
    CHECK(Vector2D{-2., 0.} == separation_rule(bird0, 2.5, s_factor, flock_test));
    s_factor = 0.5;
    CHECK(Vector2D{-1., 0.} == separation_rule(bird0, 2.5, s_factor, flock_test));
    s_factor = 1.;
    CHECK(Vector2D{-2., -3.} == separation_rule(bird0, 3., s_factor, flock_test));
    CHECK(Vector2D{-5., -6.} == separation_rule(bird0, 5., s_factor, flock_test));
  }

  SUBCASE("Testing alignment_rule function")
  {
    double a_factor = 1.;
    CHECK(Vector2D{0., 0.} == alignment_rule(bird0, 2.5, a_factor, flock_test));
    CHECK(Vector2D{-1., -1.} == alignment_rule(bird0, 3., a_factor, flock_test));

    Vector2D alignment_velocity{};
    alignment_velocity = alignment_rule(bird0, 5., a_factor, flock_test);
    CHECK(alignment_velocity.x == doctest::Approx(-1.66).epsilon(0.01));
    CHECK(alignment_velocity.y == -2.);

    a_factor           = 0.5;
    alignment_velocity = alignment_rule(bird0, 5., a_factor, flock_test);
    CHECK(alignment_velocity.x == doctest::Approx(-0.83).epsilon(0.01));
    CHECK(alignment_velocity.y == -1.);
  }

  SUBCASE("Testing mass_center function")
  {
    Vector2D mass_center_calc{};
    mass_center_calc = calc_mass_center(std::vector<int>{1}, flock_test);
    CHECK(Vector2D{3, 1} == mass_center_calc);

    mass_center_calc = calc_mass_center(std::vector<int>{1, 3}, flock_test);
    CHECK(Vector2D{2, 2.5} == mass_center_calc);

    mass_center_calc = calc_mass_center(std::vector<int>{1, 2, 3}, flock_test);
    CHECK(mass_center_calc.x == doctest::Approx(2.66).epsilon(0.01));
    CHECK(mass_center_calc.y == 3.);
  }

  SUBCASE("Testing cohesion_rule function")
  {
    double c_factor = 1.;
    CHECK(Vector2D{2., 0.} == cohesion_rule(bird0, 2.5, c_factor, flock_test));
    CHECK(Vector2D{1., 1.5} == cohesion_rule(bird0, 3., c_factor, flock_test));

    Vector2D cohesion_velocity{};
    cohesion_velocity = cohesion_rule(bird0, 5., c_factor, flock_test);
    CHECK(cohesion_velocity.x == doctest::Approx(1.66).epsilon(0.01));
    CHECK(cohesion_velocity.y == 2.);

    c_factor          = 0.5;
    cohesion_velocity = cohesion_rule(bird0, 5., c_factor, flock_test);
    CHECK(cohesion_velocity.x == doctest::Approx(0.83).epsilon(0.01));
    CHECK(cohesion_velocity.y == 1.);
  }

  SUBCASE("Testing wall_rule function")
  {
    double w_factor = 1.;
    double box_size = 6.;
    CHECK(Vector2D{2., 2.} == wall_rule(bird0, w_factor, box_size));
    w_factor = 0.5;
    CHECK(Vector2D{0., 1.} == wall_rule(bird1, w_factor, box_size));
    w_factor = 1.;
    CHECK(Vector2D{-1., -1.} == wall_rule(bird2, w_factor, box_size));
    CHECK(Vector2D{2., -1.} == wall_rule(bird3, w_factor, box_size));
  }

  SUBCASE("Testing calc_average_velocity_norm function")
  {
    Statistics average_velocity_stat{calc_average_velocity_norm(flock_test)};
    CHECK(average_velocity_stat.mean_ == doctest::Approx(1.97).epsilon(0.01));
    CHECK(average_velocity_stat.sigma_ == doctest::Approx(1.35).epsilon(0.01));
    CHECK(average_velocity_stat.mean_err_ == doctest::Approx(0.67).epsilon(0.01));
  }

  SUBCASE("Testing calc_average_bird_to_bird_distance function")
  {
    Statistics average_distance_stat{calc_average_bird_to_bird_distance(flock_test)};
    CHECK(average_distance_stat.mean_ == doctest::Approx(3.17).epsilon(0.01));
    CHECK(average_distance_stat.sigma_ == doctest::Approx(0.74).epsilon(0.01));
    CHECK(average_distance_stat.mean_err_ == doctest::Approx(0.30).epsilon(0.01));
  }
}
