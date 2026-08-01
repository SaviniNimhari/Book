#ifndef BOOK_H
#define BOOK_H

#include <string>

struct Book {
    int id;
    std::string title;
    std::string author;
    bool isBorrowed;

    Book() : id(0), title(""), author(""), isBorrowed(false) {}
    Book(int bookId, const std::string& bookTitle, const std::string& bookAuthor, bool borrowed = false)
        : id(bookId), title(bookTitle), author(bookAuthor), isBorrowed(borrowed) {}
};

#endif