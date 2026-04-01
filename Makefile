CXX ?= g++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -pedantic

DESTINATION_DIR := output/
SRC_DIR := src/
TARGET := $(DESTINATION_DIR)blackjack
OBJS := $(DESTINATION_DIR)Blackjack.o $(DESTINATION_DIR)Card.o $(DESTINATION_DIR)Deck.o \
	    $(DESTINATION_DIR)Display.o $(DESTINATION_DIR)Hand.o $(DESTINATION_DIR)GameLogic.o

TEST_DIR := tests/
TEST_TARGET1 := $(DESTINATION_DIR)test_card_tests
TEST_TARGET2 := $(DESTINATION_DIR)bet_tests
TEST_TARGET3 := $(DESTINATION_DIR)deck_tests
TEST_TARGET4 := $(DESTINATION_DIR)hand_tests
TEST_OBJS1 := $(DESTINATION_DIR)test_card.o $(DESTINATION_DIR)Card.o $(DESTINATION_DIR)Display.o
TEST_OBJS2 := $(DESTINATION_DIR)test_bet.o $(DESTINATION_DIR)Card.o $(DESTINATION_DIR)Hand.o $(DESTINATION_DIR)GameLogic.o $(DESTINATION_DIR)Display.o
TEST_OBJS3 := $(DESTINATION_DIR)test_deck.o $(DESTINATION_DIR)Card.o $(DESTINATION_DIR)Deck.o $(DESTINATION_DIR)Display.o
TEST_OBJS4 := $(DESTINATION_DIR)test_hand.o $(DESTINATION_DIR)Card.o $(DESTINATION_DIR)Hand.o $(DESTINATION_DIR)Display.o

.PHONY: all clean run test

all: $(TARGET)

# Pattern rule: src/%.cpp -> output/%.o
$(DESTINATION_DIR)%.o: $(SRC_DIR)%.cpp | $(DESTINATION_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DESTINATION_DIR):
	mkdir -p $@

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@

# Header dependencies
$(DESTINATION_DIR)Blackjack.o: $(SRC_DIR)Deck.h $(SRC_DIR)Hand.h $(SRC_DIR)Display.h
$(DESTINATION_DIR)Card.o:      $(SRC_DIR)Card.h $(SRC_DIR)Display.h
$(DESTINATION_DIR)Deck.o:      $(SRC_DIR)Deck.h $(SRC_DIR)Card.h
$(DESTINATION_DIR)Display.o:   $(SRC_DIR)Display.h $(SRC_DIR)Card.h $(SRC_DIR)Hand.h
$(DESTINATION_DIR)Hand.o:      $(SRC_DIR)Hand.h $(SRC_DIR)Card.h $(SRC_DIR)Display.h

# ── Test target ──────────────────────────────────────────────
$(DESTINATION_DIR)test_card.o: $(TEST_DIR)test_card.cpp | $(DESTINATION_DIR)
	$(CXX) $(CXXFLAGS) -I$(TEST_DIR) -c $< -o $@

$(DESTINATION_DIR)test_bet.o: $(TEST_DIR)test_bet.cpp | $(DESTINATION_DIR)
	$(CXX) $(CXXFLAGS) -I$(TEST_DIR) -c $< -o $@

$(DESTINATION_DIR)test_deck.o: $(TEST_DIR)test_deck.cpp | $(DESTINATION_DIR)
	$(CXX) $(CXXFLAGS) -I$(TEST_DIR) -c $< -o $@

$(DESTINATION_DIR)test_hand.o: $(TEST_DIR)test_hand.cpp | $(DESTINATION_DIR)
	$(CXX) $(CXXFLAGS) -I$(TEST_DIR) -c $< -o $@

$(TEST_TARGET1): $(TEST_OBJS1)
	$(CXX) $(TEST_OBJS1) -o $@

$(TEST_TARGET2): $(TEST_OBJS2) $(TARGET)
	$(CXX) $(TEST_OBJS2) -o $@

$(TEST_TARGET3): $(TEST_OBJS3)
	$(CXX) $(TEST_OBJS3) -o $@

$(TEST_TARGET4): $(TEST_OBJS4)
	$(CXX) $(TEST_OBJS4) -o $@

test: $(TEST_TARGET1) $(TEST_TARGET2) $(TEST_TARGET3) $(TEST_TARGET4)
	./$(TEST_TARGET1)
	./$(TEST_TARGET2)
	./$(TEST_TARGET3)
	./$(TEST_TARGET4)

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) $(OBJS) $(TEST_OBJS1) $(TEST_OBJS2) $(TEST_OBJS3) $(TEST_OBJS4) $(TARGET) $(TEST_TARGET1) $(TEST_TARGET2) $(TEST_TARGET3) $(TEST_TARGET4)
	-rmdir $(DESTINATION_DIR) 2>/dev/null || true