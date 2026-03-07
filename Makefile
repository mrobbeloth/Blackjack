CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic

TARGET := blackjack.exe
SRCS := Blackjack.cpp Card.cpp Deck.cpp Display.cpp Hand.cpp
OBJS := $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@

Blackjack.o: Blackjack.cpp Deck.h Hand.h Display.h
Card.o: Card.cpp Card.h Display.h
Deck.o: Deck.cpp Deck.h Card.h
Display.o: Display.cpp Display.h Card.h Hand.h
Hand.o: Hand.cpp Hand.h Card.h Display.h

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(OBJS) $(TARGET)
