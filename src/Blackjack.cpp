// Blackjack.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define PLAYER_WINS 1
#define DEALER_WINS 0
#define PLAYER_BUSTED -1
#define DEALER_BUSTED 2

#include <iostream>
#include <cstring>
#include "Hand.h"
#include "Deck.h"
#include "Display.h"

int play_hand(float balance, float bet);
float get_bet(float);

int main(int argc, char* argv[])
{
    Display::initDisplay();

    // Parse command-line flags
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "--compact") == 0) {
            Display::setMode(COMPACT);
        } else if (std::strcmp(argv[i], "--full") == 0) {
            Display::setMode(FULL_ART);
        }
    }

    bool play_again = true;
    char response;

    float balance = 1000.0f; //player starts with $1,000
    float bet;

    Display::clearScreen();
    std::cout << Ansi::BOLD << Ansi::CYAN
              << "\xe2\x95\x94\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x97" << std::endl  // ╔══════════════════════════╗
              << "\xe2\x95\x91   Welcome to BLACKJACK   \xe2\x95\x91" << std::endl             // ║   Welcome to BLACKJACK   ║
              << "\xe2\x95\x9a\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x90\xe2\x95\x9d" << std::endl  // ╚══════════════════════════╝
              << Ansi::RESET << std::endl;
    Display::printPrompt("Hit Enter to continue...");
    getchar();

    while (play_again) {

        bet = get_bet(balance);

        int result = play_hand(balance, bet);

        switch (result) {
        case (PLAYER_BUSTED):
            Display::printBust("Player is Busted!");
            Display::printDealerWins();
            balance -= bet;
            break;
        case (DEALER_WINS):
            Display::printDealerWins();
            balance -= bet;
            break;
        case (DEALER_BUSTED):
            Display::printBust("Dealer is Busted!");
            Display::printPlayerWins();
            balance += bet;
            break;
        case (PLAYER_WINS):
            Display::printPlayerWins();
            balance += bet;
            break;
        }

        std::cout << std::endl;
        Display::printInfo("Balance: $" + std::to_string((int)balance));
        std::cout << std::endl;
        Display::printPrompt("Would you like to play another hand? (Y/N) ");
        std::cin >> response;
        if (response == 'y' || response == 'Y') {
            play_again = true;
        }
        else {
            play_again = false;
        }
    }
    std::cout << std::endl;
    Display::printInfo("Thanks for playing! Final balance: $" + std::to_string((int)balance));
    std::cout << std::endl;
}

int play_hand(float balance, float bet){
    bool hit = false;
    char response;
    int result=0;

    srand((unsigned int)time(NULL));  //seed the random number generator

    /*Create and shuffle deck*/

    Deck d1;
    d1.shuffle();

    /*Initialize player's and dealer's hands*/

    Hand player_hand;
    Hand dealer_hand;
    player_hand.add_card(d1.deal());
    player_hand.add_card(d1.deal());

    dealer_hand.add_card(d1.deal());
    dealer_hand.add_card(d1.deal());

    /*Display table with dealer's first card hidden*/
    Display::renderTable("DEALER", dealer_hand.getCards(), dealer_hand.score(),
                         "PLAYER", player_hand.getCards(), player_hand.score(),
                         balance, bet, true);

    /*Deal another card to the player as long as they want to hit*/
    Display::printPrompt("(H)it or (S)tay? ");
    std::cin >> response;

    hit = (response == 'H' || response == 'h');

    while (hit && player_hand.score()<21) {
        player_hand.add_card(d1.deal());

        Display::renderTable("DEALER", dealer_hand.getCards(), dealer_hand.score(),
                             "PLAYER", player_hand.getCards(), player_hand.score(),
                             balance, bet, true);

        if (player_hand.score() < 21) {
            Display::printPrompt("(H)it or (S)tay? ");
            std::cin >> response;

            hit = (response == 'H' || response == 'h');
        }
    }

    if (player_hand.score() < 22) {

        /*Display dealer's hand revealed*/
        Display::renderTable("DEALER", dealer_hand.getCards(), dealer_hand.score(),
                             "PLAYER", player_hand.getCards(), player_hand.score(),
                             balance, bet, false);

        while (dealer_hand.score() <= player_hand.score() && dealer_hand.score() < 21 || dealer_hand.score() < 17) {
            Display::printInfo("Dealer hits...");
            dealer_hand.add_card(d1.deal());

            Display::renderTable("DEALER", dealer_hand.getCards(), dealer_hand.score(),
                                 "PLAYER", player_hand.getCards(), player_hand.score(),
                                 balance, bet, false);

            if (dealer_hand.score() >= 22) {
                result = DEALER_BUSTED;
            }
        }
    }
    else {

        result = PLAYER_BUSTED;
    }
    int ds = dealer_hand.score();
    int ps = player_hand.score();
    if (ds < 22 &&  ps < 22) {
        if (ds > ps) {

            result = DEALER_WINS;
        }
        else {

            result = PLAYER_WINS;
        }
    }
    return result;
}

float get_bet(float balance) {
    float bet = 0;
    std::cout << std::endl;
    Display::printInfo("Your bank balance is $" + std::to_string((int)balance));
    Display::printPrompt("How much do you want to bet? $");
    std::cin >> bet;

    while (bet <= 0 || bet > balance) {
        if (bet <= 0)
            Display::printBust("Bet must be greater than $0.");
        else
            Display::printBust("You can't bet more than your balance ($" + std::to_string((int)balance) + ")");
        Display::printPrompt("How much do you want to bet? $");
        std::cin >> bet;
    }
    return bet;
}


