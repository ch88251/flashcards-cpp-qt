#pragma once

#include <vector>
#include "domain/Flashcard.h"

class FlashcardRepository
{
public:
    bool save(const Flashcard& flashcard);
    std::vector<Flashcard> findAll();
    std::vector<Flashcard> findByDeckId(int deckId);
};
