#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../src/Blackjack.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <array>
#include <fstream>
#include <chrono>


#ifdef _WIN32
#define popen  _popen
#define pclose _pclose
#endif

// Helper: write input lines to a temp file, run blackjack.exe with stdin
// redirected from that file, and return its stdout.
static std::string run_with_input(const std::vector<std::string>& lines) {
    // Write input to a temp file (one token/line per entry)
#ifdef _WIN32
    std::string tmpfile = "output\\test_input.txt";
    std::string cmd = "output\\blackjack.exe < " + tmpfile + " 2>&1";
#else
    std::string tmpfile = "output/test_input.txt";
    std::string cmd = "output/blackjack < " + tmpfile + " 2>&1";
#endif
    {
        std::ofstream ofs(tmpfile);
        for (const auto& line : lines)
            ofs << line << "\n";
    }

    std::array<char, 4096> buffer;
    std::string result;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR: popen failed";
    while (fgets(buffer.data(), (int)buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }
    pclose(pipe);
    std::remove(tmpfile.c_str());
    return result;
}

// ── Bet input validation tests (subprocess) ─────────────────

TEST_CASE("Alpha bet input does not cause infinite loop") {
    // Input sequence: Enter (welcome), "abc" (bad bet), "100" (good bet),
    //                 "S" (stay), "N" (don't play again)
    auto start = std::chrono::steady_clock::now();
    std::string output = run_with_input({"", "abc", "100", "S", "N"});
    auto elapsed = std::chrono::steady_clock::now() - start;
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();

    CHECK(secs < 30);
    CHECK(output.find("Invalid input") != std::string::npos);
}

TEST_CASE("Numeric bet input works normally") {
    auto start = std::chrono::steady_clock::now();
    std::string output = run_with_input({"", "100", "S", "N"});
    auto elapsed = std::chrono::steady_clock::now() - start;
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();

    CHECK(secs < 30);
    CHECK(output.find("balance") != std::string::npos);
}

TEST_CASE("Multiple alpha inputs then valid bet does not hang") {
    auto start = std::chrono::steady_clock::now();
    std::string output = run_with_input({"", "abc", "xyz", "50", "S", "N"});
    auto elapsed = std::chrono::steady_clock::now() - start;
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();

    CHECK(secs < 30);
}

TEST_CASE("Negative bet then valid bet does not hang") {
    auto start = std::chrono::steady_clock::now();
    std::string output = run_with_input({"", "-10", "50", "S", "N"});
    auto elapsed = std::chrono::steady_clock::now() - start;
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();

    CHECK(secs < 30);
    CHECK(output.find("greater than") != std::string::npos);
}

TEST_CASE("Dealer should hit on less than 17 when player has more") {
    // Simulate the scenario: dealer has 17, player has 20
    Hand dealer_hand;
    dealer_hand.add_card(Card(10, 'S'));  // 10
    dealer_hand.add_card(Card(6, 'H'));   // 6  → score = 16

    Hand player_hand;
    player_hand.add_card(Card(10, 'D'));  // 10
    player_hand.add_card(Card(10, 'C'));  // 10 → score = 20

    int ds = dealer_hand.score();
    int ps = player_hand.score();

    bool should_hit = dealer_should_hit(ds, ps);

    // Dealer has 16, should HIT in real blackjack
    CHECK(should_hit == true);  
}

TEST_CASE("Dealer should hit on less than 17 when player has less") {
    // Simulate the scenario: dealer has 17, player has 20
    Hand dealer_hand;
    dealer_hand.add_card(Card(10, 'S'));  // 10
    dealer_hand.add_card(Card(6, 'H'));   // 6  → score = 16

    Hand player_hand;
    player_hand.add_card(Card(5, 'D'));  // 5
    player_hand.add_card(Card(6, 'C'));  // 6 → score = 11

    int ds = dealer_hand.score();
    int ps = player_hand.score();

    bool should_hit = dealer_should_hit(ds, ps);

    // Dealer has 16, should HIT in real blackjack
    CHECK(should_hit == true); 
}

TEST_CASE("Dealer should stand on 17 when player has more") {
    // Simulate the scenario: dealer has 17, player has 20
    Hand dealer_hand;
    dealer_hand.add_card(Card(10, 'S'));  // 10
    dealer_hand.add_card(Card(7, 'H'));   // 7  → score = 17

    Hand player_hand;
    player_hand.add_card(Card(10, 'D'));  // 10
    player_hand.add_card(Card(10, 'C'));  // 10 → score = 20

    int ds = dealer_hand.score();
    int ps = player_hand.score();

    bool should_hit = dealer_should_hit(ds, ps);

    // Dealer has 17, should STAND in real blackjack
    CHECK(should_hit == false); 
}

TEST_CASE("Dealer should stand on 17 when player has less") {
    // Simulate the scenario: dealer has 17, player has 20
    Hand dealer_hand;
    dealer_hand.add_card(Card(10, 'S'));  // 10
    dealer_hand.add_card(Card(7, 'H'));   // 7  → score = 17

    Hand player_hand;
    player_hand.add_card(Card(5, 'D'));  // 5
    player_hand.add_card(Card(6, 'C'));  // 6 → score = 11

    int ds = dealer_hand.score();
    int ps = player_hand.score();

    bool should_hit = dealer_should_hit(ds, ps);

    // Dealer has 17, should STAND in real blackjack
    CHECK(should_hit == false); 
}

// ── Hit/Stay input validation tests (issue #3) ───────────────

TEST_CASE("Invalid hit/stay input does not cause hang or silent stay") {
    // 'g' is invalid — game should re-prompt, then accept 'S'
    auto start = std::chrono::steady_clock::now();
    std::string output = run_with_input({"", "100", "g", "S", "N"});
    auto elapsed = std::chrono::steady_clock::now() - start;
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();

    CHECK(secs < 30);
    CHECK(output.find("Invalid input") != std::string::npos);
}

TEST_CASE("Multiple invalid hit/stay inputs then valid stay does not hang") {
    // Several bad inputs before a valid 'S'
    auto start = std::chrono::steady_clock::now();
    std::string output = run_with_input({"", "100", "g", "x", "z", "S", "N"});
    auto elapsed = std::chrono::steady_clock::now() - start;
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();

    CHECK(secs < 30);
    CHECK(output.find("Invalid input") != std::string::npos);
}

TEST_CASE("Invalid hit/stay input then valid hit does not hang") {
    // 'q' is invalid — game should re-prompt, then accept 'H' to hit, then 'S' to stay
    auto start = std::chrono::steady_clock::now();
    std::string output = run_with_input({"", "100", "q", "H", "S", "N"});
    auto elapsed = std::chrono::steady_clock::now() - start;
    auto secs = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();

    CHECK(secs < 30);
    CHECK(output.find("Invalid input") != std::string::npos);
}