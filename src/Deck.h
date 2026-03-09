#pragma once
#include "Card.h"
#include <vector>

class Deck
{
private:
	std::vector<Card> cards;
public:
	Deck();
	void shuffle();
	Card deal();	
};

