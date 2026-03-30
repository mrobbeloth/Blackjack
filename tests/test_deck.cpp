#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/Card.h"
#include "../src/Deck.h"

// ── Deck tests ──────────────────────────────────────────────

TEST_CASE("Deck has 52 cards") {
    Deck d;
    // Deal all 52 cards
    for (int i = 0; i < 52; i++) {
        Card c = d.deal();
        CHECK(c.get_value() >= 1);
        CHECK(c.get_value() <= 13);
    }
    // 53rd deal should return the sentinel card (value 0)
    Card sentinel = d.deal();
    CHECK(sentinel.get_value() == 0);
}

TEST_CASE("Deck shuffle does not lose cards") {
    Deck d;
    d.shuffle();
    int count = 0;
    for (int i = 0; i < 52; i++) {
        Card c = d.deal();
        if (c.get_value() != 0) count++;
    }
    CHECK(count == 52);
}
