#include "FlashcardRepository.h"
#include "domain/Flashcard.h"
#include <vector>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

bool FlashcardRepository::save(const Flashcard& flashcard)
{
    QSqlQuery query;

    query.prepare(R"(
        INSERT INTO cards (deck_id, front, back)
        VALUES (:deck_id, :front, :back)
    )");

    query.bindValue(":deck_id", flashcard.deckId);
    query.bindValue(":front", QString::fromStdString(flashcard.front));
    query.bindValue(":back", QString::fromStdString(flashcard.back));

    if (!query.exec()) {
        qCritical() << "Failed to save flashcard:"
                    << query.lastError().text();
        return false;
    }

    return true;
}

std::vector<Flashcard> FlashcardRepository::findAll()
{
    std::vector<Flashcard> flashcards;

    QSqlQuery query;

    if (!query.exec(R"(
        SELECT id, deck_id, front, back
        FROM cards
        ORDER BY id
    )"))
    {
        qCritical() << "Failed to find flashcards:"
                    << query.lastError().text();

        return flashcards;
    }

    while (query.next())
    {
        Flashcard flashcard;
        flashcard.id = query.value("id").toInt();
        flashcard.deckId = query.value("deck_id").toInt();
        flashcard.front = query.value("front").toString().toStdString();
        flashcard.back = query.value("back").toString().toStdString();

        flashcards.push_back(flashcard);
    }

    return flashcards;
}

std::vector<Flashcard> FlashcardRepository::findByDeckId(int deckId)
{
    std::vector<Flashcard> flashcards;

    QSqlQuery query;
    query.prepare(R"(
        SELECT id, deck_id, front, back
        FROM cards
        WHERE deck_id = :deck_id
        ORDER BY id
    )");
    query.bindValue(":deck_id", deckId);

    if (!query.exec())
    {
        qCritical() << "Failed to find flashcards by deck id:"
                    << query.lastError().text();

        return flashcards;
    }

    while (query.next())
    {
        Flashcard flashcard;
        flashcard.id = query.value("id").toInt();
        flashcard.deckId = query.value("deck_id").toInt();
        flashcard.front = query.value("front").toString().toStdString();
        flashcard.back = query.value("back").toString().toStdString();

        flashcards.push_back(flashcard);
    }

    return flashcards;
}

bool FlashcardRepository::update(const Flashcard& flashcard)
{
  QSqlQuery query;

  query.prepare(R"(
    UPDATE cards
    SET front = :front,
        back = :back
    WHERE id = :id
  )");

  query.bindValue(":front", QString::fromStdString(flashcard.front));
  query.bindValue(":back", QString::fromStdString(flashcard.back));
  query.bindValue(":id", flashcard.id);

  if (!query.exec()) {
    qCritical() << "Failed to update flashcard: "
                << query.lastError().text();
    return false;
  }

  return true;
}

bool FlashcardRepository::remove(int id)
{
  QSqlQuery query;

  query.prepare(R"(
    DELETE FROM cards
    WHERE id = :id
  )");

  query.bindValue(":id", id);

  if (!query.exec()) {
    qCritical() << "Failed to delete flashcard: "
                << query.lastError().text();
    return false;
  }

  return true;
}
