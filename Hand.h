#pragma once
#include "Card.h"
#include <vector>


class Hand
{
private:
	std::vector<Card> cards;

public:
	Hand();

	void display();
	int score();
	void add_card(Card);
	std::vector<Card>& getCards();
};

