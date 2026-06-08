#include "MainWindow.h"

#include <QToolBar>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QSplitter>
#include <QHeaderView>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QTextEdit>
#include <QDialogButtonBox>
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

    QDialog dialog(this);
    dialog.setWindowTitle("Create Deck");
    dialog.resize(300, 150);

    auto* layout = new QVBoxLayout(&dialog);
    auto* formLayout = new QFormLayout();

    auto* nameEdit = new QLineEdit(&dialog);
    formLayout->addRow("Deck Name:", nameEdit);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel,
        &dialog);

    layout->addLayout(formLayout);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QString deckName = nameEdit->text().trimmed();

    Deck deck;
    deck.name = deckName.toStdString();

    if (deckRepository.save(deck))
    {
        loadDecks();
    }    
}

void MainWindow::createFlashcard()
{
    if (selectedDeckId <= 0)
    {
        QMessageBox::warning(
            this,
            "No Deck Selected",
            "Please select a deck before adding a flashcard.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Create Flashcard");
    dialog.resize(500, 350);

    auto* layout = new QVBoxLayout(&dialog);
    auto* formLayout = new QFormLayout();

    auto* frontEdit = new QTextEdit(&dialog);
    auto* backEdit = new QTextEdit(&dialog);

    frontEdit->setMinimumHeight(100);
    backEdit->setMinimumHeight(100);

    formLayout->addRow("Front:", frontEdit);
    formLayout->addRow("Back:", backEdit);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel,
        &dialog);

    layout->addLayout(formLayout);
    layout->addWidget(buttons);    

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    QString front = frontEdit->toPlainText().trimmed();
    QString back = backEdit->toPlainText().trimmed();

    if (front.isEmpty() || back.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Missing Information",
            "Both front and back are required.");
        return;
    }

    Flashcard flashcard;
    flashcard.deckId = selectedDeckId;
    flashcard.front = front.toStdString();
    flashcard.back = back.toStdString();

    if (flashcardRepository.save(flashcard))
    {
        loadFlashcards();
    }    
}