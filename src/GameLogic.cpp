#include "Blackjack.h"

bool dealer_should_hit(int dealer_score, int player_score) {
    return (dealer_score <= player_score && dealer_score < 17) || dealer_score < 17;
}

char get_char_input(const std::string& prompt, const std::string& valid_chars) {
    char response;
    Display::printPrompt(prompt);
    std::cin >> response;
    while (valid_chars.find(response) == std::string::npos) {
        Display::printBust("Invalid input. Please enter one of: " + valid_chars);
        Display::printPrompt(prompt);
        std::cin >> response;
    }
    return response;
}
