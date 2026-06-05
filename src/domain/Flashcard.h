#pragma once

#include <string>

class Flashcard
{
public:
    int id{};
    int deckId{};
    std::string front;
    std::string back;
};
