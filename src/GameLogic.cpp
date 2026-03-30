#include "Blackjack.h"

bool dealer_should_hit(int dealer_score, int player_score) {
    return (dealer_score <= player_score && dealer_score < 17) || dealer_score < 17;
}
