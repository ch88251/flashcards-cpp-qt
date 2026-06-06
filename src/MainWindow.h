#pragma once

#include <QMainWindow>
#include "repositories/DeckRepository.h"
#include "repositories/FlashcardRepository.h"

class QListWidget;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUi();
    void loadDecks();
    void loadFlashcards();

    QListWidget* deckList{nullptr};
    QTableWidget* flashcardTable{nullptr};

    DeckRepository deckRepository;
    FlashcardRepository flashcardRepository;    
};
