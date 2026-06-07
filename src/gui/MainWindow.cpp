#include "MainWindow.h"

#include <QToolBar>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QSplitter>
#include <QHeaderView>
#include <QInputDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

#include "repositories/DeckRepository.h"
#include "repositories/FlashcardRepository.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    refreshUi();
}

void MainWindow::setupUi()
{
    setWindowTitle("Flashcard App");
    resize(900, 600);

    auto* centralWidget = new QWidget(this);

    QToolBar* toolbar = addToolBar("Main");
    QAction* addDeckAction = toolbar->addAction("Add Deck");
    QAction* addFlashcardAction = toolbar->addAction("Add Flashcard");

    connect(
        addDeckAction,
        &QAction::triggered,
        this,
        &MainWindow::createDeck);

    connect(
        addFlashcardAction,
        &QAction::triggered,
        this,        
        &MainWindow::createFlashcard);

    auto* mainLayout = new QVBoxLayout(centralWidget);

    auto* splitter = new QSplitter(this);

    mainLayout->addWidget(splitter);


    deckList = new QListWidget(splitter);

    connect(
        deckList,
        &QListWidget::currentRowChanged,
        this,
        &MainWindow::onDeckSelected);

    flashcardTable = new QTableWidget(splitter);
    flashcardTable->setColumnCount(2);
    flashcardTable->setHorizontalHeaderLabels({"Front", "Back"});
    flashcardTable->horizontalHeader()->setStretchLastSection(true);

    splitter->setSizes({200, 700});

    setCentralWidget(centralWidget); 

}

void MainWindow::refreshUi()
{
    loadDecks();
    loadFlashcards();
}

void MainWindow::loadDecks()
{
    auto decks = deckRepository.findAll();
    deckList->clear();

    for (const auto& deck : decks)
    {
        auto* item = new QListWidgetItem(QString::fromStdString(deck.name));
        item->setData(Qt::UserRole, deck.id);
        deckList->addItem(item);
    }

    if (deckList->count() > 0)
    {
        deckList->setCurrentRow(0);
    }
}

void MainWindow::loadFlashcards()
{
    std::vector<Flashcard> flashcards;

    if (selectedDeckId > 0)
    {
        flashcards = flashcardRepository.findByDeckId(selectedDeckId);
        qDebug() << "Flashcard count for deck id" << selectedDeckId << ":" << flashcards.size();
    }
    else
    {
        flashcards = flashcardRepository.findAll();
        qDebug() << "Flashcard count:" << flashcards.size();
    }

    flashcardTable->setRowCount(static_cast<int>(flashcards.size()));

    for (int row = 0; row < flashcards.size(); ++row)
    {
        const auto& flashcard = flashcards[row];
        flashcardTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(flashcard.front)));
        flashcardTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(flashcard.back)));
    }
}

void MainWindow::onDeckSelected(int row)
{
    if (row < 0) {
        selectedDeckId = -1;
        qDebug() << "No deck selected";
        return;
    }

    QListWidgetItem* item = deckList->item(row);

    if (!item)
    {
        selectedDeckId = -1;
        return;
    }

    selectedDeckId = item->data(Qt::UserRole).toInt();

    qDebug() << "Selected deck id:" << selectedDeckId;

    loadFlashcards();
}

void MainWindow::createDeck()
{
    QString deckName =
        QInputDialog::getText(
            this,
            "Add Deck",
            "Deck Name:")
            .trimmed();

    if (deckName.isEmpty())
    {
        return;
    }
    
    Deck deck;
    deck.name = deckName.toStdString();

    if (deckRepository.save(deck))
    {
        loadDecks();
    }    
}

void MainWindow::createFlashcard()
{
    QString front =
        QInputDialog::getText(
            this,
            "Add Flashcard",
            "Front:");

    QString back =
        QInputDialog::getText(
            this,
            "Add Flashcard",
            "Back:");
}