cmake_minimum_required(VERSION 3.20)
project(Blackjack LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

add_executable(blackjack
    src/Blackjack.cpp
    src/Card.cpp
    src/Deck.cpp
    src/Display.cpp
    src/Hand.cpp
)

# ── Tests (doctest) ──────────────────────────────────────────
enable_testing()

add_executable(test_card tests/test_card.cpp src/Card.cpp src/Display.cpp)
target_include_directories(test_card PRIVATE tests)
add_test(NAME CardTests COMMAND test_card)

add_executable(test_deck tests/test_deck.cpp src/Card.cpp src/Deck.cpp src/Display.cpp)
target_include_directories(test_deck PRIVATE tests)
add_test(NAME DeckTests COMMAND test_deck)

add_executable(test_hand tests/test_hand.cpp src/Card.cpp src/Hand.cpp src/Display.cpp)
target_include_directories(test_hand PRIVATE tests)
add_test(NAME HandTests COMMAND test_hand)

add_executable(test_bet tests/test_bet.cpp)
target_include_directories(test_bet PRIVATE tests)
add_test(NAME BetTests COMMAND test_bet)
