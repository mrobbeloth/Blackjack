#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/Card.h"

// ── Card tests ──────────────────────────────────────────────

TEST_CASE("Card stores value and suit") {
    Card c(10, 'H');
    CHECK(c.get_value() == 10);
    CHECK(c.get_suit() == 'H');
}

TEST_CASE("Ace card") {
    Card c(1, 'S');
    CHECK(c.get_value() == 1);
    CHECK(c.get_suit() == 'S');
}
