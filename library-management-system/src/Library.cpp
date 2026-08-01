#include "Library.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

namespace {

std::string escapeJson(const std::string& value) {
    std::ostringstream escaped;
    for (char character : value) {
        switch (character) {
            case '\\': escaped << "\\\\"; break;
            case '"': escaped << "\\\""; break;
            case '\b': escaped << "\\b"; break;
            case '\f': escaped << "\\f"; break;
            case '\n': escaped << "\\n"; break;
            case '\r': escaped << "\\r"; break;
            case '\t': escaped << "\\t"; break;
            default:
                escaped << character;
                break;
        }
    }
    return escaped.str();
}

std::string boolText(bool value) {
    return value ? "true" : "false";
}

}  // namespace

Library::Library(const std::string& storageFile) : fileName(storageFile) {
    loadFromFile();
}

int Library::findBookIndex(int id) const {
    for (size_t index = 0; index < books.size(); ++index) {
        if (books[index].id == id) {
            return static_cast<int>(index);
        }
    }
    return -1;
}

void Library::loadFromFile() {
    books.clear();

    std::ifstream input(fileName);
    if (!input.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream stream(line);
        std::string idText;
        std::string title;
        std::string author;
        std::string borrowedText;

        if (std::getline(stream, idText, '|') &&
            std::getline(stream, title, '|') &&
            std::getline(stream, author, '|') &&
            std::getline(stream, borrowedText)) {
            try {
                int id = std::stoi(idText);
                bool isBorrowed = (borrowedText == "1");
                books.emplace_back(id, title, author, isBorrowed);
            } catch (...) {
                continue;
            }
        }
    }
}

void Library::saveToFile() const {
    std::ofstream output(fileName, std::ios::trunc);
    for (const Book& book : books) {
        output << book.id << '|' << book.title << '|' << book.author << '|' << (book.isBorrowed ? 1 : 0) << '\n';
    }

    exportJson();
}

void Library::exportJson(const std::string& outputFile) const {
    int borrowedCount = 0;
    for (const Book& book : books) {
        if (book.isBorrowed) {
            ++borrowedCount;
        }
    }

    int availableCount = static_cast<int>(books.size()) - borrowedCount;

    std::ofstream output(outputFile, std::ios::trunc);
    output << "{\n";
    output << "  \"stats\": [\n";
    output << "    { \"label\": \"Total Books\", \"value\": \"" << books.size() << "\" },\n";
    output << "    { \"label\": \"Borrowed\", \"value\": \"" << borrowedCount << "\" },\n";
    output << "    { \"label\": \"Available\", \"value\": \"" << availableCount << "\" }\n";
    output << "  ],\n";

    output << "  \"books\": [\n";
    for (size_t index = 0; index < books.size(); ++index) {
        const Book& book = books[index];
        output << "    {\n";
        output << "      \"id\": " << book.id << ",\n";
        output << "      \"title\": \"" << escapeJson(book.title) << "\",\n";
        output << "      \"author\": \"" << escapeJson(book.author) << "\",\n";
        output << "      \"isBorrowed\": " << boolText(book.isBorrowed) << "\n";
        output << "    }" << (index + 1 < books.size() ? "," : "") << "\n";
    }
    output << "  ],\n";

    output << "  \"featuredBooks\": [\n";
    for (size_t index = 0; index < books.size() && index < 3; ++index) {
        const Book& book = books[index];
        output << "    {\n";
        output << "      \"id\": " << book.id << ",\n";
        output << "      \"title\": \"" << escapeJson(book.title) << "\",\n";
        output << "      \"author\": \"" << escapeJson(book.author) << "\",\n";
        output << "      \"status\": \"" << (book.isBorrowed ? "Borrowed" : "Available") << "\"\n";
        output << "    }" << (index + 1 < books.size() && index < 2 ? "," : "") << "\n";
    }
    output << "  ],\n";

    output << "  \"cart\": [\n";
    bool firstCartEntry = true;
    for (const Book& book : books) {
        if (!book.isBorrowed) {
            continue;
        }

        if (!firstCartEntry) {
            output << ",\n";
        }
        firstCartEntry = false;

        output << "    {\n";
        output << "      \"title\": \"" << escapeJson(book.title) << "\",\n";
        output << "      \"author\": \"" << escapeJson(book.author) << "\",\n";
        output << "      \"status\": \"Borrowed\"\n";
        output << "    }";
    }
    if (!firstCartEntry) {
        output << '\n';
    }
    output << "  ],\n";

    output << "  \"account\": {\n";
    output << "    \"name\": \"Amina Khan\",\n";
    output << "    \"memberId\": \"LM-2048\",\n";
    output << "    \"plan\": \"Premium Reader\",\n";
    output << "    \"joined\": \"2025-08-12\"\n";
    output << "  },\n";

    output << "  \"activity\": [\n";
    output << "    \"Borrowing and returning books updates the JSON snapshot\",\n";
    output << "    \"The cart page shows borrowed books from the C++ backend\",\n";
    output << "    \"File handling keeps book records persistent\"\n";
    output << "  ],\n";

    output << "  \"loginCards\": [\n";
    output << "    { \"title\": \"Fast Sign In\", \"description\": \"Quick authentication for members.\" },\n";
    output << "    { \"title\": \"Saved Account\", \"description\": \"Store member details for faster access.\" },\n";
    output << "    { \"title\": \"Secure Session\", \"description\": \"Simple protected dashboard preview.\" }\n";
    output << "  ],\n";

    output << "  \"cards\": [\n";
    output << "    { \"title\": \"Home Overview\", \"description\": \"Quick summary of library activity and important numbers.\", \"tag\": \"home\" },\n";
    output << "    { \"title\": \"Member Account\", \"description\": \"Profile information, membership tier, and saved preferences.\", \"tag\": \"account\" },\n";
    output << "    { \"title\": \"Borrowed Items\", \"description\": \"Books currently checked out by the user.\", \"tag\": \"cart\" },\n";
    output << "    { \"title\": \"Login Access\", \"description\": \"Secure sign-in panel for the dashboard.\", \"tag\": \"login\" },\n";
    output << "    { \"title\": \"Library Cards\", \"description\": \"Compact action cards for browsing and checkout.\", \"tag\": \"cards\" }\n";
    output << "  ]\n";
    output << "}\n";
}

bool Library::addBook(const Book& book) {
    if (findBookIndex(book.id) != -1) {
        return false;
    }

    books.push_back(book);
    saveToFile();
    return true;
}

void Library::addBook() {
    int id;
    std::string title;
    std::string author;

    std::cout << "Enter book ID: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter book title: ";
    std::getline(std::cin, title);

    std::cout << "Enter author name: ";
    std::getline(std::cin, author);

    if (addBook(Book(id, title, author, false))) {
        std::cout << "Book added successfully.\n";
    } else {
        std::cout << "A book with that ID already exists.\n";
    }
}

void Library::showBooks() const {
    if (books.empty()) {
        std::cout << "No books available.\n";
        return;
    }

    std::cout << "\nCurrent Books\n";
    std::cout << std::left << std::setw(8) << "ID"
              << std::setw(30) << "Title"
              << std::setw(25) << "Author"
              << "Status\n";
    std::cout << std::string(75, '-') << '\n';

    for (const Book& book : books) {
        std::cout << std::left << std::setw(8) << book.id
                  << std::setw(30) << book.title
                  << std::setw(25) << book.author
                  << (book.isBorrowed ? "Borrowed" : "Available") << '\n';
    }
}

bool Library::borrowBook(int id) {
    int index = findBookIndex(id);
    if (index == -1 || books[index].isBorrowed) {
        return false;
    }

    books[index].isBorrowed = true;
    saveToFile();
    return true;
}

void Library::borrowBook() {
    int id;
    std::cout << "Enter book ID to borrow: ";
    std::cin >> id;

    if (borrowBook(id)) {
        std::cout << "Book borrowed successfully.\n";
    } else {
        std::cout << "Book not found or already borrowed.\n";
    }
}

bool Library::returnBook(int id) {
    int index = findBookIndex(id);
    if (index == -1 || !books[index].isBorrowed) {
        return false;
    }

    books[index].isBorrowed = false;
    saveToFile();
    return true;
}

void Library::returnBook() {
    int id;
    std::cout << "Enter book ID to return: ";
    std::cin >> id;

    if (returnBook(id)) {
        std::cout << "Book returned successfully.\n";
    } else {
        std::cout << "Book not found or it was not borrowed.\n";
    }
}