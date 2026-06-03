#pragma once

#include <vector>
#include "Flashcard.h"

class FlashcardRepository
{
public:
    bool save(const Flashcard& flashcard);
    std::vector<Flashcard> findAll();
};