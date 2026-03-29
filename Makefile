CXX ?= g++
CXXFLAGS ?= -std=c++20 -Wall -Wextra -pedantic

DESTINATION_DIR := output/
SRC_DIR := src/
TARGET := $(DESTINATION_DIR)blackjack.exe
OBJS := $(DESTINATION_DIR)Blackjack.o $(DESTINATION_DIR)Card.o $(DESTINATION_DIR)Deck.o \
        $(DESTINATION_DIR)Display.o $(DESTINATION_DIR)Hand.o

TEST_DIR := tests/
TEST_TARGET := $(DESTINATION_DIR)blackjack_tests
TEST_OBJS := $(DESTINATION_DIR)test_card.o $(DESTINATION_DIR)Card.o $(DESTINATION_DIR)Deck.o \
             $(DESTINATION_DIR)Display.o $(DESTINATION_DIR)Hand.o

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

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(TEST_OBJS) -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

run: $(TARGET)
    ./$(TARGET)

clean:
    $(RM) $(OBJS) $(TEST_OBJS) $(TARGET) $(TEST_TARGET)
    -rmdir $(DESTINATION_DIR) 2>/dev/null || true