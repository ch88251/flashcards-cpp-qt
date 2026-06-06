#pragma once

#include <vector>

#include "domain/Deck.h"

class DeckRepository
{
public:
  bool save(const Deck& deck);
  std::vector<Deck> findAll();
};
