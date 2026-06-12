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
    void refreshUi();
    void loadDecks();
    void loadFlashcards();
    void onDeckSelected(int row);
    void createDeck();
    void createFlashcard();
    void editFlashcard(int row);
    void deleteFlashcard(int row);
    void showFlashcardContextMenu(const QPoint& pos);

    QListWidget* deckList{nullptr};
    QTableWidget* flashcardTable{nullptr};

    int selectedDeckId{-1};

    DeckRepository deckRepository;
    FlashcardRepository flashcardRepository;    
};
