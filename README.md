# Library Management System - C++

This project implements a simple library management system using C++ and object-oriented programming.

## Project Aim

This project aims at the implementation of a simple library management system by using OOP in C++. The system shall perform the following functions: store book details such as ID, title, author, and status; add new books to the library; borrowing and returning of books; utilize the principles of OOP, which include modularity and reusability of code; employ file handling for the persistence of book records.

## System Design and Architecture

### Book Structure

- Attributes: `ID`, `title`, `author`, `isBorrowed`
- Used to store information about each book

### Library Class

- Attributes: collection of books and book count
- Methods:
  - `addBook()` adds a new book to the library
  - `showBooks()` displays books and their status
  - `borrowBook()` allows borrowing of books
  - `returnBook()` allows returning of books

## Folder Structure

- `src/` C++ source code
- `data/books.txt` persistent storage for records
- `data/library.json` JSON snapshot written by the backend
- `frontend/` Tailwind-based multi-page presentation

## Build and Run

Compile with a C++17 compiler:

```bash
g++ -std=c++17 src/main.cpp src/Library.cpp -o library-management
```

Run the executable:

```bash
./library-management
```

On Windows with MinGW, the command is similar:

```bash
g++ -std=c++17 src/main.cpp src/Library.cpp -o library-management.exe
```

On Windows, you can also run the helper script from the project root:

```powershell
.\run-backend.ps1
```

Or double-click [run-backend.bat](run-backend.bat).

## Data Format

Each record is saved as:

```text
ID|Title|Author|Status
```

Where status is `0` for available and `1` for borrowed.

## Notes

- The C++ backend handles all library operations and file persistence.
- The backend also exports a JSON snapshot to `data/library.json` for the frontend pages.
- The frontend is split into separate pages: `index.html`, `login.html`, `account.html`, `cart.html`, and `cards.html`.
