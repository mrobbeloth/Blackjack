#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/Card.h"
#include "../src/Hand.h"

// ── Hand tests ──────────────────────────────────────────────

TEST_CASE("Empty hand scores zero") {
    Hand h;
    CHECK(h.score() == 0);
}

TEST_CASE("Hand with number cards") {
    Hand h;
    h.add_card(Card(5, 'H'));
    h.add_card(Card(7, 'D'));
    CHECK(h.score() == 12);
}

TEST_CASE("Face cards count as 10") {
    Hand h;
    h.add_card(Card(11, 'C')); // Jack
    h.add_card(Card(12, 'S')); // Queen
    CHECK(h.score() == 20);
}

TEST_CASE("Single ace counts as 11") {
    Hand h;
    h.add_card(Card(1, 'H'));  // Ace
    h.add_card(Card(5, 'D'));
    CHECK(h.score() == 16);
}

TEST_CASE("Ace counts as 1 when 11 would bust") {
    Hand h;
    h.add_card(Card(1, 'H'));  // Ace
    h.add_card(Card(10, 'D'));
    h.add_card(Card(10, 'S'));
    CHECK(h.score() == 21);
}

TEST_CASE("Blackjack: ace + face card") {
    Hand h;
    h.add_card(Card(1, 'S'));   // Ace
    h.add_card(Card(13, 'H')); // King
    CHECK(h.score() == 21);
}
