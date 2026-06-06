#include "MainWindow.h"

#include <QLabel>
#include <QListWidget>
#include <QTableWidget>
#include <QSplitter>
#include <QHeaderView>
#include <QDebug>

#include "repositories/DeckRepository.h"
#include "repositories/FlashcardRepository.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    loadDecks();
    loadFlashcards();

}

void MainWindow::setupUi()
{

    setWindowTitle("Flashcard App");
    resize(900, 600);

    deckList = new QListWidget(this);
    flashcardTable = new QTableWidget(this);

    auto* splitter = new QSplitter(this);

    deckList = new QListWidget(splitter);

    flashcardTable = new QTableWidget(splitter);
    flashcardTable->setColumnCount(2);
    flashcardTable->setHorizontalHeaderLabels({"Front", "Back"});
    flashcardTable->horizontalHeader()->setStretchLastSection(true);

    splitter->setSizes({200, 700});

    setCentralWidget(splitter);    

}

void MainWindow::loadDecks()
{
    auto decks = deckRepository.findAll();
    qDebug() << "Deck count:" << decks.size();
    deckList->clear();

    for (const auto& deck : decks)
    {
        deckList->addItem(QString::fromStdString(deck.name));
    }
}

void MainWindow::loadFlashcards()
{
    auto flashcards = flashcardRepository.findAll();
    qDebug() << "Flashcard count:" << flashcards.size();
    flashcardTable->setRowCount(static_cast<int>(flashcards.size()));

    for (int row = 0; row < flashcards.size(); ++row)
    {
        const auto& flashcard = flashcards[row];
        flashcardTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(flashcard.front)));
        flashcardTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(flashcard.back)));
    }
}
