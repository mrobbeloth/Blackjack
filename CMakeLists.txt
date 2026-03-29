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

add_executable(blackjack_tests
    tests/test_card.cpp
    src/Card.cpp
    src/Deck.cpp
    src/Display.cpp
    src/Hand.cpp
)

target_include_directories(blackjack_tests PRIVATE tests)

add_test(NAME BlackjackTests COMMAND blackjack_tests)
