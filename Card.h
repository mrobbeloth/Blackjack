#pragma once
class Card
{
private:
	int value;
	char suit;

public:
	Card(int, char);
	void display();
	int get_value();
	char get_suit();
};

