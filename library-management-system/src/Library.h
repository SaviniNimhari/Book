#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"

#include <string>
#include <vector>

class Library {
private:
    std::vector<Book> books;
    std::string fileName;

    int findBookIndex(int id) const;
    void loadFromFile();
    void saveToFile() const;
    void exportJson(const std::string& outputFile = "data/library.json") const;

public:
    explicit Library(const std::string& storageFile = "data/books.txt");

    bool addBook(const Book& book);
    void addBook();

    void showBooks() const;

    bool borrowBook(int id);
    void borrowBook();

    bool returnBook(int id);
    void returnBook();
};

#endif