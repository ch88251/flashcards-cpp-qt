#include "DatabaseManager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDir>

bool DatabaseManager::open()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName("flashcards.db");
    qDebug() << "Current working directory:" << QDir::currentPath();
    qDebug() << "Opening database:" << m_database.databaseName();
    if (!m_database.open()) {
        qDebug() << "Failed to open database:" << m_database.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::initializeSchema()
{
    QSqlQuery query;

    const char *createDecksTable = R"(
        CREATE TABLE IF NOT EXISTS decks (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL UNIQUE
        )
    )";

    if (!query.exec(createDecksTable)) {
        qDebug() << "Failed to create decks table:" << query.lastError().text();
        return false;
    }

    const char *createCardsTable = R"(
        CREATE TABLE IF NOT EXISTS cards (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            deck_id INTEGER NOT NULL,
            front TEXT NOT NULL,
            back TEXT NOT NULL,
            created_at TEXT DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(deck_id) REFERENCES decks(id)
        )
    )";

    if (!query.exec(createCardsTable)) {
        qDebug() << "Failed to create cards table:" << query.lastError().text();
        return false;
    }

    query.exec(R"(
        INSERT OR IGNORE INTO decks (id, name)
        VALUES (1, 'Default')
    )");
    return true;
}