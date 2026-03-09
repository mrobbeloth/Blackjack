#pragma once
#include "Card.h"
#include <string>
#include <vector>

enum DisplayMode { COMPACT, FULL_ART };

// ANSI color codes
namespace Ansi {
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[91m";
    const std::string GREEN   = "\033[92m";
    const std::string YELLOW  = "\033[93m";
    const std::string CYAN    = "\033[96m";
    const std::string WHITE   = "\033[97m";
    const std::string BOLD    = "\033[1m";
    const std::string DIM     = "\033[2m";
}

class Hand;  // forward declaration

class Display {
public:
    static void initDisplay();
    static void setMode(DisplayMode mode);
    static DisplayMode getMode();

    // Card rendering
    static std::string suitSymbol(char suit);
    static std::string suitColor(char suit);
    static std::string rankString(int value);

    // Inline compact rendering
    static std::string renderCardInline(Card& card);

    // Full-art box rendering (7 lines per card)
    static std::vector<std::string> renderCardBox(Card& card);
    static std::vector<std::string> renderHiddenCardBox();

    // Hand rendering
    static void renderHand(std::vector<Card>& cards, const std::string& indent = "");
    static void renderHandWithHidden(std::vector<Card>& cards, const std::string& indent = "");

    // Screen management
    static void clearScreen();

    // Table layout
    static void renderTable(const std::string& dealerLabel,
                            std::vector<Card>& dealerCards,
                            int dealerScore,
                            const std::string& playerLabel,
                            std::vector<Card>& playerCards,
                            int playerScore,
                            float balance, float bet,
                            bool hideDealerFirst = false);

    // Colored status messages
    static void printPlayerWins(const std::string& msg = "Player wins!");
    static void printDealerWins(const std::string& msg = "Dealer wins");
    static void printBust(const std::string& msg);
    static void printPrompt(const std::string& msg);
    static void printInfo(const std::string& msg);

private:
    static DisplayMode currentMode;
};
