#include "Blackjack.h"

bool dealer_should_hit(int dealer_score, int player_score) {
    return (dealer_score <= player_score && dealer_score < 17) || dealer_score < 17;
}

char get_char_input(const std::string& prompt, const std::string& valid_chars) {
    if (valid_chars.empty()) {
        return '\0';
    }

    while (true) {
        Display::printPrompt(prompt);
        char response;
        if (!(std::cin >> response)) {
            if (std::cin.eof()) {
                return '\0';
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            Display::printInfo("Invalid input. Please enter one of: " + valid_chars);
            continue;
        }

        if (valid_chars.find(response) != std::string::npos) {
            return response;
        }

        Display::printInfo("Invalid input. Please enter one of: " + valid_chars);
    }
}
