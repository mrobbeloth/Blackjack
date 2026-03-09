#include "Hand.h"
#include "Display.h"
#include <iostream>

Hand::Hand() {

}

void Hand::display() {
	Display::renderHand(cards);
}

int Hand::score() {
	int card_val;
	int card_weight;
	int score1 = 0;
	int num_aces = 0;

	/*Add everything except the aces first
	  J, Q, and K count as 10*/

	for (unsigned int k = 0; k < cards.size(); k++) {
		card_val = cards.at(k).get_value();

		switch (card_val) {
		case(1):

			num_aces += 1;
			card_weight = 0;

			break;
		case(11):
		case(12):
		case(13):
			card_weight = 10;
			break;
		default:
			card_weight = card_val;
		}
		score1 += card_weight;
	}

	/*Now add the Aces based on the sum of the other cards*/
	if (num_aces > 0) {
		if (score1 > 10 - (num_aces - 1)) {
			score1 += num_aces;  //all aces count as 1
		}
		else  {
			score1 += (10 + num_aces); //first ace counts as 11, others as 1
		}
	}
	return score1;
}

void Hand::add_card(Card c) {
	cards.push_back(c);
}

std::vector<Card>& Hand::getCards() {
	return cards;
}
