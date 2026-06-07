#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include "domain/Deck.h"
#include "repositories/DeckRepository.h"

bool DeckRepository::save(const Deck &deck)
{
    QSqlQuery query;

    query.prepare(R"(
        INSERT INTO decks (name)
        VALUES (:name)
    )");

    query.bindValue(":name", QString::fromStdString(deck.name));

    if (!query.exec())
    {
        qCritical() << "Failed to save deck:"
                    << query.lastError().text();
        return false;
    }

    return true;
}

std::vector<Deck> DeckRepository::findAll()
{
    std::vector<Deck> decks;

    QSqlQuery query;

    if (!query.exec(R"(
      SELECT id, name
      FROM decks
      ORDER BY name
    )"))
    {
        qCritical() << "Failed to find decks:"
                    << query.lastError().text();

        return decks;
    }

    while (query.next())
    {
        Deck deck;
        deck.id = query.value("id").toInt();
        deck.name = query.value("name").toString().toStdString();

        decks.push_back(deck);
    }

    return decks;
}
