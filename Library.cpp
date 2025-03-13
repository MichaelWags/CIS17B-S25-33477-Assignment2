#include <iostream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

//book class
class Book {
    public:
        string title;
        string author;
        int ISBN;
        bool availability;

        Book(string t, string a, int i) : title(t), author(a), ISBN(i), availability(true) {}
};

//user class
class User {
    public:
        string name;
        int userID;
        list<int> borrowedBooks;
        int IDcounter;

        User(string n){
            name = n;
            userID = IDcounter++;
        }
};

//inherited student class
class Student : public User {
    public:
        Student(string n) : User(n){}
};

//inherited faculty class
class Faculty : public User {
    public:
        Faculty(string n) : User(n){}
};

//library class
class Library {
    private:
        static Library* instance;
        list<Book> books;
        list<User*> users;

        Library(){}

    public:
        static Library* getInstance(){
            if(!instance){
                instance = new Library();
            }
            return instance;
        }

        //Functions

        //add a book to library
        void addBook(string title, string author, int ISBN){
            books.push_back(Book(title, author, ISBN));
        }

        //register user
        void registerUser(User* user){
            users.push_back(user);
        }

        //search for a book
        void searchBook(string input) {
            for (auto& book : books) {
                if (book.title.find(input) >= 0 || book.author.find(input) >= 0) {
                    cout << "Found: " << book.title << " by " << book.author << " ISBN: " << book.ISBN << endl;
                }
            }
        }

        //borrow a book
        void borrowBook(int userID, int ISBN){
            for (auto& book : books) {
                if (book.ISBN == ISBN) {
                    if (!book.availability) {
                        cout << "This book was already borrowed.\n";
                        return;
                    }
                    book.availability = false;
                    for (auto& user : users) {
                        if (user->userID == userID) {
                            user->borrowedBooks.push_back(ISBN);
                            cout << "You have borrowed this book.\n";
                            return;
                        }
                    }
                }
            }
            cout << "Book not found!\n";
        }

        //return a book
        void returnBook(int userID, int ISBN){
            for (auto& user : users) {
                if (user->userID == userID) {
                    auto it = find(user->borrowedBooks.begin(), user->borrowedBooks.end(), ISBN);
                    if (it == user->borrowedBooks.end()) {
                        cout << "This book was never borrowed.\n";
                        return;
                    }
                    user->borrowedBooks.erase(it);
                    for (auto& book : books) {
                        if (book.ISBN == ISBN) {
                            book.availability = true;
                            cout << "Book returned.\n";
                            return;
                        }
                    }
                }
            }
        }
};

Library* Library::instance = nullptr;

int main() {
    Library* lib = Library::getInstance(); //get the single instance of the library class

    int choice;
    do {
        cout << "\nLibrary System:\n1. Add Book\n2. Register User\n3. Borrow Book\n4. Return Book\n5. Search Book\n6. Exit\nChoice: ";
        cin >> choice;
        cin.ignore();

        //add book
        if(choice == 1) {
            string title, author; int ISBN;
            cout << "Enter title: "; getline(cin, title);
            cout << "Enter author: "; getline(cin, author);
            cout << "Enter ISBN: "; cin >> ISBN;
            lib->addBook(title, author, ISBN);
        }
        //register user
        else if(choice == 2) {
            string name;
            int type;
            cout << "Enter name: "; getline(cin, name);
            cout << "User type (1 - Student, 2 - Faculty): "; cin >> type;
            cin.ignore();

            User* user;
            
            if (type == 1) {
                user = new Student(name);
            }
            else {
                user = new Faculty(name);
            }
            lib->registerUser(user);
            cout << "User registered with ID: " << user->userID << endl;
        }
        //borrow book
        else if(choice == 3) {
            int userID; int ISBN;
            cout << "Enter User ID: "; cin >> userID;
            cout << "Enter Book ISBN: "; cin >> ISBN;
            lib->borrowBook(userID, ISBN);
        }
        //return book
        else if(choice == 4) {
            int userID; int ISBN;
            cout << "Enter User ID: "; cin >> userID;
            cout << "Enter Book ISBN: "; cin >> ISBN;
            lib->returnBook(userID, ISBN);
        }
        //search book
        else if(choice == 5) {
            string input;
            cout << "Enter book title or author: "; cin.ignore(); getline(cin, input);
            lib->searchBook(input);
        }
        //exit
    } while (choice != 6);

    return 0;
}
