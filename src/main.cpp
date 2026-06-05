#include <QApplication>
#include <QDebug>

#include "MainWindow.h"
#include "db/DatabaseManager.h"
#include "domain/Flashcard.h"
#include "repositories/FlashcardRepository.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    DatabaseManager dbManager;
    if (!dbManager.open()) {
        qCritical() << "Application startup failed: database could not be opened.";
        return 1;
    }

    if (!dbManager.initializeSchema()) {
        qCritical() << "Application startup failed: database schema could not be initialized.";
        return 1;
    }

    FlashcardRepository repository;

    Flashcard card;
    card.deckId = 1;
    card.front = "What is Python?";
    card.back = "A programming language";

    if (!repository.save(card)) {
      qCritical() << "Error saving card!";
    }

    auto cards = repository.findAll();

    for (const auto& c : cards) {
      qDebug()
        << "DeckId: " << c.deckId
        << "Front: " << QString::fromStdString(c.front)
        << "Back: " << QString::fromStdString(c.back);
    }

    MainWindow window;
    window.show();

    return app.exec();
}
