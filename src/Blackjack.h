#pragma once

// import statements
#include <iostream>
#include <cstring>
#include <limits>
#include "Hand.h"
#include "Deck.h"
#include "Display.h"

// Constants for game results
#define PLAYER_WINS 1
#define DEALER_WINS 0
#define PLAYER_BUSTED -1
#define DEALER_BUSTED 2

// externals
int play_hand(float, float);
float get_bet(float);
bool dealer_should_hit(int, int);