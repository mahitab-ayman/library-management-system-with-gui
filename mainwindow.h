#include <QApplication>
#include <QApplication>
#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QStringList>
#include <QVBoxLayout>
#include <iostream>
#include <string>
#include <algorithm>

// Book class definition
class Book {
public:
    Book(const std::string& title, const std::string& author, const std::string& isbn, const std::string& publicationDate)
        : title(title), author(author), isbn(isbn), publicationDate(publicationDate) {}

    const std::string& getTitle() const { return title; }
    const std::string& getAuthor() const { return author; }
    const std::string& getISBN() const { return isbn; }
    const std::string& getPublicationDate() const { return publicationDate; }
    void setTitle(const std::string& title)  { this->title = title; }
    void setAuthor(const std::string& author)  { this->author = author; }
    void setISBN(const std::string& isbn)  { this->isbn = isbn; }
    void setPublicationDate(const std::string& publicationDate)  { this->publicationDate = publicationDate; }

    std::string title;
    std::string author;
    std::string isbn;
    std::string publicationDate;
};



struct Node {
    Book* book;
    Node* next;

    Node(Book* book) : book(book), next(nullptr) {}
};


// Linked list data structure
class LinkedList {
public:

    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current->book;
            delete current;
            current = next;
        }
    }
    bool containsBook(Book* book) {
        Node* current = head;
        while (current) {
            if (current->book->getTitle() == book->getTitle()) {
                return true;
            }
            current = current->next;
        }
        return false;
    }


    void edit(int selectedRow,
                          std::string& title, std::string& author,
                          std::string& isbn, std::string& pubDate) {

        Node* current = head;
        int row = 0;

        while (current && row != selectedRow) {
            current = current->next;
            row++;
        }

        if (current) {
            current->book->setTitle(title);
            current->book->setAuthor(author);
            current->book->setISBN(isbn);
            current->book->setPublicationDate(pubDate);
        } else {
            QMessageBox::warning(nullptr, "Error", "Book not found.");
        }
    }



    void addBook(Book* book) {
        Node* newNode = new Node(book);

        if (!head) {
            head = newNode;
            return;
        }

        Node* current = head;
        Node* prev = nullptr;

        while (current && current->book->getTitle() < book->getTitle()) {
            prev = current;
            current = current->next;
        }

        if (current && current->book->getTitle() == book->getTitle()) {
            std::cout << "Duplicate book found. Book not inserted." << std::endl;
            delete newNode;
            return;
        }

        if (!prev) {
            newNode->next = head;
            head = newNode;
        } else {
            prev->next = newNode;
            newNode->next = current;
        }
    }
    void removeBook(const std::string& title) {
        Node* current = head;
        Node* prev = nullptr;


        while (current && current->book->getTitle().compare(title) != 0) {
    prev = current;
    current = current->next;
}

if (prev) {
   prev->next = current->next;
}

        if (!current) {
            std::cout << "Book not found." << std::endl;
            return;
        }

        if (!prev) {
            head = current->next;
        } else {
            prev->next = current->next;
        }

        delete current->book;
        delete current;

        QMessageBox::information(nullptr, "Book Removed",
                                 QString::fromStdString("Book removed successfully."));
    }

    Node* getHead() const { return head; }


    Node* head;
};

// Binary search tree node definition
struct BSTNode {
    Book* book;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Book* book) : book(book), left(nullptr), right(nullptr) {}
};

// Binary search tree data structure
class BinarySearchTree {
public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        clear(root);
    }

    void insertBook(Book* book) {
        root = insert(root, book);
    }

    void removeBook(const std::string& title) {
        root = remove(root, title);
    }

    void searchBooks(const std::string& key, const std::string& searchBy) {
        if (searchBy == "Title") {
            searchByTitle(root, key);
        } else if (searchBy == "Author") {
            searchByAuthor(root, key);
        } else if (searchBy == "ISBN") {
            searchByISBN(root, key);
        } else {
            std::cout << "Invalid search criteria." << std::endl;
        }
    }

    void displayBooks() {
        displayInOrder(root);
    }

    int countBooksByAuthor(const std::string& author) {
        return countBooksByAuthor(root, author);
    }
    void displayBooksByAuthor(const std::string& author) {
        displayByAuthor(root, author);
    }
private:
    BSTNode* root;

    BSTNode* insert(BSTNode* node, Book* book) {
        if (!node) {
            return new BSTNode(book);
        }

        if (book->getTitle() < node->book->getTitle()) {
            node->left = insert(node->left, book);
        } else if (book->getTitle() > node->book->getTitle()) {
            node->right = insert(node->right, book);
        } else {
            std::cout << "Duplicate book found. Book not inserted." << std::endl;
            return node;
        }

        return node;
    }


    BSTNode* remove(BSTNode* node, const std::string& title) {
        if (!node) {

            std::cout << "Book not found." << std::endl;
            return nullptr;
        }

        if (title < node->book->getTitle()) {
            node->left = remove(node->left, title);
        } else if (title > node->book->getTitle()) {
            node->right = remove(node->right, title);
        } else {
            if (!node->left && !node->right) {
                delete node->book;
                delete node;
                return nullptr;
            } else if (!node->left) {
                BSTNode* temp = node->right;
                delete node->book;
                delete node;
                return temp;
            } else if (!node->right) {
                BSTNode* temp = node->left;
                delete node->book;
                delete node;
                return temp;
            } else {
                BSTNode* successor = findMin(node->right);
                node->book = successor->book;
                node->right = remove(node->right, successor->book->getTitle());
            }
        }

        return node;
    }

    BSTNode* findMin(BSTNode* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }



    void searchByTitle(BSTNode* node, const std::string& title) {
        if (!node) {

            QMessageBox::information(nullptr, "Book not found",
                                     QString::fromStdString("Book not found."));
            return;
        }

        if (title < node->book->getTitle()) {
            searchByTitle(node->left, title);
        } else if (title > node->book->getTitle()) {
            searchByTitle(node->right, title);
        } else {
            QMessageBox::information(nullptr, "Book Found",
                                     QString::fromStdString("Book found: "
                                                            + node->book->getTitle()));

        }
    }

    void searchByAuthor(BSTNode* node, const std::string& author) {
        if (!node) {

            return;
        }

        searchByAuthor(node->left, author);
        if (node->book->getAuthor() == author) {
            QMessageBox::information(nullptr, "Book Found",
                                     QString::fromStdString("Book found: "
                                                            + node->book->getTitle()));

        }
        searchByAuthor(node->right, author);
    }

    void searchByISBN(BSTNode* node, const std::string& isbn) {
        if (!node) {

            QMessageBox::information(nullptr, "Book not found",
                                     QString::fromStdString("Book not found."));
            return;
        }

        if (isbn < node->book->getISBN()) {
            searchByISBN(node->left, isbn);
        } else if (isbn > node->book->getISBN()) {
            searchByISBN(node->right, isbn);
        } else {
            QMessageBox::information(nullptr, "Book Found",
                                     QString::fromStdString("Book found: "
                                                            + node->book->getTitle()));

        }
    }

    void displayInOrder(BSTNode* node) {
        if (!node) {
            return;
        }

        displayInOrder(node->left);
        std::cout << "Title: " << node->book->getTitle() << ", Author: " << node->book->getAuthor() << ", ISBN: " << node->book->getISBN() << ", Publication Date: " << node->book->getPublicationDate() << std::endl;
        displayInOrder(node->right);
    }

    void clear(BSTNode* node) {
        if (!node) {
            return;
        }

        clear(node->left);
        clear(node->right);
        delete node->book;
        delete node;
    }

    int countBooksByAuthor(BSTNode* node, const std::string& author) {
        if (!node) {
            return 0;
        }

        int count = 0;
        count += countBooksByAuthor(node->left, author);
        if (node->book->getAuthor() == author) {
            count++;
        }
        count += countBooksByAuthor(node->right, author);

        return count;
    }
    void displayByAuthor(BSTNode* node, const std::string& author) {
        if (!node) return;

        displayByAuthor(node->left, author);

        if (node->book->getAuthor() == author) {

            QMessageBox::information(nullptr, "Book by Author",
                                     QString::fromStdString("Title: " + node->book->getTitle() +
                                                            ", Author: " + node->book->getAuthor()));
        }

        displayByAuthor(node->right, author);
    }
};

// Custom main window class for the GUI
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
int selectedRow;
    MainWindow(QWidget* parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Book Library");

        QWidget* centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        bookTable = new QTableWidget(centralWidget);
        bookTable->setColumnCount(4);
        QStringList headers;
        headers << "Title" << "Author" << "ISBN" << "Publication Date";
        bookTable->setHorizontalHeaderLabels(headers);
        bookTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);

        addButton = new QPushButton("Add Book", centralWidget);
        removeButton = new QPushButton("Remove Book", centralWidget);
        searchButton = new QPushButton("Search Book", centralWidget);
        displayButton = new QPushButton("Display All Books", centralWidget);
        authorCountButton = new QPushButton("Count Books by Author", centralWidget);
        displayByAuthorButton = new QPushButton("Display Books by Author", centralWidget);
        editButton = new QPushButton("edit Book", centralWidget);
        titleLineEdit = new QLineEdit(centralWidget);
        authorLineEdit = new QLineEdit(centralWidget);
        isbnLineEdit = new QLineEdit(centralWidget);
        publicationDateLineEdit = new QLineEdit(centralWidget);
        searchLineEdit = new QLineEdit(centralWidget);
        authorCountLineEdit = new QLineEdit(centralWidget);

        searchComboBox = new QComboBox(centralWidget);
        searchComboBox->addItem("Title");
        searchComboBox->addItem("Author");
        searchComboBox->addItem("ISBN");

        QVBoxLayout* layout = new QVBoxLayout(centralWidget);
        layout->addWidget(bookTable);

        QHBoxLayout* buttonLayout = new QHBoxLayout();

        buttonLayout->addWidget(addButton);
        buttonLayout->addWidget(removeButton);
        buttonLayout->addWidget(searchButton);
        buttonLayout->addWidget(displayButton);
        buttonLayout->addWidget(searchComboBox);
        buttonLayout->addWidget(searchLineEdit);
        buttonLayout->addWidget(authorCountButton);
         buttonLayout->addWidget(displayByAuthorButton);
        buttonLayout->addWidget(authorCountLineEdit);
        buttonLayout->addWidget(editButton);
        layout->addWidget(titleLineEdit);
        layout->addWidget(authorLineEdit);
        layout->addWidget(isbnLineEdit);
        layout->addWidget(publicationDateLineEdit);
        layout->addLayout(buttonLayout);
        connect(editButton, SIGNAL(clicked()), this, SLOT(editBook()));
        connect(addButton, SIGNAL(clicked()), this, SLOT(addBook()));
        connect(removeButton, SIGNAL(clicked()), this, SLOT(removeBook()));
        connect(searchButton, SIGNAL(clicked()), this, SLOT(searchBook()));
        connect(displayButton, SIGNAL(clicked()), this, SLOT(displayBooks()));
        connect(authorCountButton, SIGNAL(clicked()), this, SLOT(countBooksByAuthor()));
connect(displayByAuthorButton, SIGNAL(clicked()), this, SLOT(displayBooksByAuthor()));
        bookLibrary = new LinkedList();
        binarySearchTree = new BinarySearchTree();
    }

    ~MainWindow() {
        delete bookLibrary;
        delete binarySearchTree;
    }

public slots:


    void addBook() {
        std::string title = titleLineEdit->text().toStdString();
        std::string author = authorLineEdit->text().toStdString();
        std::string isbn = isbnLineEdit->text().toStdString();
        std::string publicationDate = publicationDateLineEdit->text().toStdString();

        Book* newBook = new Book(title, author, isbn, publicationDate);

        if (bookLibrary->containsBook(newBook)) {
            //std::cout << "Duplicate book found. Book not inserted." << std::endl;
            QMessageBox::warning(this, "Duplicate book found", "The book already exists in the library. Book not inserted.");
            delete newBook;
            clearInputFields();
        } else {
            bookLibrary->addBook(newBook);
            binarySearchTree->insertBook(newBook);
            displayBooks();
            clearInputFields();
        }}


    void removeBook() {
        int selectedRow = bookTable->currentRow();
        if (selectedRow >= 0) {
            QTableWidgetItem* titleItem = bookTable->item(selectedRow, 0);
            if (titleItem) {
                std::string title = titleItem->text().toStdString();

                try {
                    bookLibrary->removeBook(title);
                     binarySearchTree->removeBook(title);
                    displayBooks();
                } catch (std::exception& e) {
                    std::cout << "Error removing book: " << e.what();
                }


        }
    }
    }

    void searchBook() {
        std::string key = searchLineEdit->text().toStdString();
        std::string searchBy = searchComboBox->currentText().toStdString();

        if (!key.empty()) {
            binarySearchTree->searchBooks(key, searchBy);
        } else {
            QMessageBox::information(this, "Search Book", "Please enter a search key.");
        }
    }

    void displayBooks() {
        bookTable->clearContents();
        bookTable->setRowCount(0);

        Node* current = bookLibrary->getHead();
        int row = 0;

        while (current) {
            bookTable->insertRow(row);

            QTableWidgetItem* titleItem = new QTableWidgetItem(QString::fromStdString(current->book->getTitle()));
            QTableWidgetItem* authorItem = new QTableWidgetItem(QString::fromStdString(current->book->getAuthor()));
            QTableWidgetItem* isbnItem = new QTableWidgetItem(QString::fromStdString(current->book->getISBN()));
            QTableWidgetItem* publicationDateItem = new QTableWidgetItem(QString::fromStdString(current->book->getPublicationDate()));

            bookTable->setItem(row, 0, titleItem);
            bookTable->setItem(row, 1, authorItem);
            bookTable->setItem(row, 2, isbnItem);
            bookTable->setItem(row, 3, publicationDateItem);

            current = current->next;
            row++;
        }
    }


    void editBook() {
        selectedRow = bookTable->currentRow();
        std::string title = titleLineEdit->text().toStdString();
        std::string author = authorLineEdit->text().toStdString();
        std::string isbn = isbnLineEdit->text().toStdString();
        std::string publicationDate = publicationDateLineEdit->text().toStdString();

        bookLibrary->edit(selectedRow, title, author, isbn, publicationDate);

        displayBooks();
        clearInputFields();
    }


    void displayBooksByAuthor() {
        std::string author = authorCountLineEdit->text().toStdString();

        binarySearchTree->displayBooksByAuthor(author);
    }
    void countBooksByAuthor() {
        std::string author = authorCountLineEdit->text().toStdString();
        int count = binarySearchTree->countBooksByAuthor(author);

        QMessageBox::information(this, "Count Books by Author", "Number of books by " + QString::fromStdString(author) + ": " + QString::number(count));
    }

private:
    LinkedList list;
    QTableWidget* bookTable;
    QPushButton* addButton;
     QPushButton*  displayByAuthorButton;
    QPushButton* removeButton;
    QPushButton* searchButton;
    QPushButton* displayButton;
    QPushButton* authorCountButton;
    QLineEdit* titleLineEdit;
    QLineEdit* authorLineEdit;
    QLineEdit* isbnLineEdit;
    QLineEdit* publicationDateLineEdit;
    QLineEdit* searchLineEdit;
    QLineEdit* authorCountLineEdit;
    QComboBox* searchComboBox;
    QPushButton *editButton;
    LinkedList* bookLibrary;
    BinarySearchTree* binarySearchTree;

    void clearInputFields() {
        titleLineEdit->clear();
        authorLineEdit->clear();
        isbnLineEdit->clear();
        publicationDateLineEdit->clear();
    }

};

