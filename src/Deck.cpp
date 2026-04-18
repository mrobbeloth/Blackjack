#include "Deck.h"
#include "Card.h"
#include <vector>
#include <cstdlib>

Deck::Deck() {
	char s = '?';
	/*Create an ordered deck*/
	for (int k = 0; k < 4; k++) {
		switch (k) {
		case (0):
			s = 'S';
			break;
		case (1):
			s = 'C';
			break;
		case (2):
			s = 'D';
			break;
		case(3):
			s = 'H';
		}
		for (int j = 0; j < 13; j++) {
			Card c(j + 1, s);
			cards.push_back(c);
		}
	}
}

Card Deck::deal() {
	int v;
	char s;

	if (!cards.empty()) {
		/*copy the value and suit fields from the last card in the deck*/
		v = cards.back().get_value();
		s = cards.back().get_suit();

		/*remove that card from the deck*/
		cards.pop_back();

		/*return a card having the same value & suit*/
		return Card(v, s);
	}
	else {
		return Card(0, 'J');
	}

}

void Deck::shuffle() {

	/*Pick a card at random and put it on the back of the deck. Repeat 1000 times*/

	int n1;
	int temp_value;
	char temp_suit;

	for (int k = 0; k < 1000; k++) {
		n1 = rand() % 52;

		temp_value = cards.at(n1).get_value();
		temp_suit = cards.at(n1).get_suit();
		
		Card temp_card(temp_value, temp_suit);

		cards.erase(cards.begin()+n1);
		cards.push_back(temp_card);


	}
}
