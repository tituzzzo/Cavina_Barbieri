#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include <iostream>
// file to test
#include "vector2d.hpp"
#include "bird.hpp"
#include "flock.hpp"

// REQUIRE controllo su variabili locali propedeutiche al test
// CHECK_THROWS per testare error handling
// CHECK per testare ritnorni di funzione

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

} // test_case

TEST_CASE("Testing Bird")
{
  SUBCASE("Testing != operator")
  {
    Bird b1{1, Vector2D{1., 1.}};
    Bird b2{2, Vector2D{2., 2.}};
    CHECK(b1 != b2);
  }
}

TEST_CASE("Testing calc_bird_to_bird_distance function (in flock.hpp)")
{
    Bird b1{1, Vector2D{1., 1.}};
    Bird b2{2, Vector2D{2., 2.}};
    CHECK(calc_bird_to_bird_distance(b1, b2) == doctest::Approx(1.41).epsilon(0.01));
    Vector2D new_pos{1.,1.};
    b2.set_position(new_pos);
    CHECK(calc_bird_to_bird_distance(b1, b2) == 0.0);
}

TEST_CASE("Testing Flock Class")
{
  SUBCASE("Testing find_birds_within_distance function")
  {
    // int n_birds = 3;
    // Flock flock{n_birds, 15.};
    // flock.set_bird_position(0, {1.,1.});
    // flock.set_bird_position(1, {2.,2.});
    // flock.set_bird_position(2, {3.,1.});
    // CHECK(flock.);
  }
}