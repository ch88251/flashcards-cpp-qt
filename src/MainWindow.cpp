#include "MainWindow.h"

#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Flashcard App");
    resize(900, 600);

    auto *label = new QLabel("Flashcard App", this);
    label->setAlignment(Qt::AlignCenter);

    setCentralWidget(label);
}
