#pragma once

// import statements
#include <iostream>
#include <cstring>
#include <limits>
#include "Hand.h"
#include "Deck.h"
#include "Display.h"

// Game result enum class for better type safety
enum class GameResult
{
    PLAYER_WINS = 1,
    DEALER_WINS = 0,
    PLAYER_BUSTED = -1,
    DEALER_BUSTED = 2,
    PUSH = 3,     // Tie/Draw (added for completeness)
    BLACKJACK = 4 // Natural blackjack (added for future implementation)
};

// externals
int play_hand(float, float);
float get_bet(float);
bool dealer_should_hit(int, int);