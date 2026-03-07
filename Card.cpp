#include "Card.h"
#include "Display.h"
#include <iostream>

Card::Card(int v, char s) {
	/* value is an integer between 1 and 13 (1 = Ace, 2 = Two, ...11 = Jack, 12 = Queen, 13 = King*/

	/*suit is a character, 'H' = hearts, 'D' = diamonds, 'C' = clubs, 'S' = spades*/

	value = v;
	suit = s;
}

int Card::get_value() {
	return value;
}

char Card::get_suit() {
	return suit;
}

void Card::display() {
	std::cout << Display::renderCardInline(*this);
}
