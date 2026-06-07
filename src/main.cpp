#include <QApplication>
#include <QDebug>

#include "gui/MainWindow.h"
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

    MainWindow window;
    window.show();

    return app.exec();
}
