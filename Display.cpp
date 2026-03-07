#include "Display.h"
#include "Hand.h"
#include <iostream>
#include <iomanip>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

DisplayMode Display::currentMode = FULL_ART;

void Display::initDisplay() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
}

void Display::setMode(DisplayMode mode) {
    currentMode = mode;
}

DisplayMode Display::getMode() {
    return currentMode;
}

std::string Display::suitSymbol(char suit) {
    switch (suit) {
    case 'S': return "\xe2\x99\xa0"; // ♠
    case 'H': return "\xe2\x99\xa5"; // ♥
    case 'D': return "\xe2\x99\xa6"; // ♦
    case 'C': return "\xe2\x99\xa3"; // ♣
    default:  return "?";
    }
}

std::string Display::suitColor(char suit) {
    if (suit == 'H' || suit == 'D') return Ansi::RED;
    return Ansi::WHITE;
}

std::string Display::rankString(int value) {
    switch (value) {
    case 1:  return "A";
    case 11: return "J";
    case 12: return "Q";
    case 13: return "K";
    default:
        return std::to_string(value);
    }
}

std::string Display::renderCardInline(Card& card) {
    std::string color = suitColor(card.get_suit());
    std::string rank = rankString(card.get_value());
    std::string suit = suitSymbol(card.get_suit());
    return color + rank + suit + Ansi::RESET;
}

std::vector<std::string> Display::renderCardBox(Card& card) {
    std::string color = suitColor(card.get_suit());
    std::string rank = rankString(card.get_value());
    std::string suit = suitSymbol(card.get_suit());
    std::string r = Ansi::RESET;

    // Rank can be 1 or 2 chars wide ("10" vs "A")
    // Top-left: rank left-aligned, Bottom-right: rank right-aligned
    std::string topRank, botRank;
    if (rank.size() == 2) {
        topRank = rank + "   ";  // "10   " = 5 chars
        botRank = "   " + rank;  // "   10" = 5 chars
    } else {
        topRank = rank + "    ";  // "A    " = 5 chars
        botRank = "    " + rank;  // "    A" = 5 chars
    }

    std::vector<std::string> lines;
    lines.push_back(color + "\xe2\x94\x8c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x90" + r); // ┌─────┐
    lines.push_back(color + "\xe2\x94\x82" + topRank + "\xe2\x94\x82" + r);                                                   // │A    │
    lines.push_back(color + "\xe2\x94\x82     \xe2\x94\x82" + r);                                                             // │     │
    lines.push_back(color + "\xe2\x94\x82  " + suit + "  \xe2\x94\x82" + r);                                                  // │  ♠  │
    lines.push_back(color + "\xe2\x94\x82     \xe2\x94\x82" + r);                                                             // │     │
    lines.push_back(color + "\xe2\x94\x82" + botRank + "\xe2\x94\x82" + r);                                                   // │    A│
    lines.push_back(color + "\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x98" + r); // └─────┘
    return lines;
}

std::vector<std::string> Display::renderHiddenCardBox() {
    std::string d = Ansi::DIM;
    std::string r = Ansi::RESET;

    std::vector<std::string> lines;
    lines.push_back(d + "\xe2\x94\x8c\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x90" + r); // ┌─────┐
    lines.push_back(d + "\xe2\x94\x82\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x94\x82" + r); // │░░░░░│
    lines.push_back(d + "\xe2\x94\x82\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x94\x82" + r); // │░░░░░│
    lines.push_back(d + "\xe2\x94\x82\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x94\x82" + r); // │░░░░░│
    lines.push_back(d + "\xe2\x94\x82\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x94\x82" + r); // │░░░░░│
    lines.push_back(d + "\xe2\x94\x82\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x96\x91\xe2\x94\x82" + r); // │░░░░░│
    lines.push_back(d + "\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x80\xe2\x94\x98" + r); // └─────┘
    return lines;
}

void Display::renderHand(std::vector<Card>& cards, const std::string& indent) {
    if (currentMode == COMPACT) {
        std::cout << indent;
        for (unsigned int i = 0; i < cards.size(); i++) {
            std::cout << renderCardInline(cards[i]);
            if (i < cards.size() - 1) std::cout << " ";
        }
        std::cout << std::endl;
    } else {
        // Full-art: collect card boxes, print side-by-side
        std::vector<std::vector<std::string>> boxes;
        for (unsigned int i = 0; i < cards.size(); i++) {
            boxes.push_back(renderCardBox(cards[i]));
        }
        for (int line = 0; line < 7; line++) {
            std::cout << indent;
            for (unsigned int i = 0; i < boxes.size(); i++) {
                std::cout << boxes[i][line];
                if (i < boxes.size() - 1) std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
}

void Display::renderHandWithHidden(std::vector<Card>& cards, const std::string& indent) {
    if (cards.empty()) return;

    if (currentMode == COMPACT) {
        std::cout << indent << Ansi::DIM << "[??]" << Ansi::RESET;
        for (unsigned int i = 1; i < cards.size(); i++) {
            std::cout << " " << renderCardInline(cards[i]);
        }
        std::cout << std::endl;
    } else {
        std::vector<std::vector<std::string>> boxes;
        boxes.push_back(renderHiddenCardBox());
        for (unsigned int i = 1; i < cards.size(); i++) {
            boxes.push_back(renderCardBox(cards[i]));
        }
        for (int line = 0; line < 7; line++) {
            std::cout << indent;
            for (unsigned int i = 0; i < boxes.size(); i++) {
                std::cout << boxes[i][line];
                if (i < boxes.size() - 1) std::cout << " ";
            }
            std::cout << std::endl;
        }
    }
}

void Display::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void Display::renderTable(const std::string& dealerLabel,
                           std::vector<Card>& dealerCards,
                           int dealerScore,
                           const std::string& playerLabel,
                           std::vector<Card>& playerCards,
                           int playerScore,
                           float balance, float bet,
                           bool hideDealerFirst) {
    clearScreen();

    // Title
    std::cout << Ansi::BOLD << Ansi::CYAN;
    std::cout << "\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90"  // ═════
              << " BLACKJACK "
              << "\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90";  // ═════
    std::cout << Ansi::RESET << std::endl << std::endl;

    // Balance & bet info
    std::cout << Ansi::YELLOW << "  Balance: $" << std::fixed << std::setprecision(0) << balance
              << "  |  Bet: $" << bet << Ansi::RESET << std::endl << std::endl;

    // Dealer zone
    std::cout << Ansi::CYAN << "  " << dealerLabel << Ansi::RESET;
    if (!hideDealerFirst) {
        std::cout << "  (Score: " << dealerScore << ")";
    } else {
        std::cout << "  (Score: ??)";
    }
    std::cout << std::endl;
    // Indent card output
    if (hideDealerFirst) {
        renderHandWithHidden(dealerCards, "  ");
    } else {
        renderHand(dealerCards, "  ");
    }
    std::cout << std::endl;

    // Player zone
    std::cout << Ansi::CYAN << "  " << playerLabel << Ansi::RESET
              << "  (Score: " << playerScore << ")" << std::endl;
    renderHand(playerCards, "  ");
    std::cout << std::endl;
}

void Display::printPlayerWins(const std::string& msg) {
    std::cout << Ansi::BOLD << Ansi::GREEN << msg << Ansi::RESET << std::endl;
}

void Display::printDealerWins(const std::string& msg) {
    std::cout << Ansi::BOLD << Ansi::RED << msg << Ansi::RESET << std::endl;
}

void Display::printBust(const std::string& msg) {
    std::cout << Ansi::BOLD << Ansi::YELLOW << msg << Ansi::RESET << std::endl;
}

void Display::printPrompt(const std::string& msg) {
    std::cout << Ansi::WHITE << msg << Ansi::RESET;
}

void Display::printInfo(const std::string& msg) {
    std::cout << Ansi::CYAN << msg << Ansi::RESET << std::endl;
}
