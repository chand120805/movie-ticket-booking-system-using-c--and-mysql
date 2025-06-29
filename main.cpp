// Movie Ticket Booking System with User Login, Movie & Theatre Selection, Seat Management, Ticket Generation

#include <iostream>
#include <mysql.h>
#include <windows.h>
#include <sstream>
using namespace std;

const char* HOST = "localhost";
const char* USER = "root";
const char* PW = "Chand@131095";
const char* DB = "mydb";

int loggedInUserID = -1;
string loggedInUserName = "";

void pause() {
    cout << "\nPress any key to continue...";
    cin.ignore();
    cin.get();
}

void userLoginOrRegister(MYSQL* conn) {
    while (true) {
        system("cls");
        cout << "1. Register\n2. Login\nEnter your choice: ";
        int choice; cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string name, phone, password;
            cout << "\nEnter Name: "; getline(cin, name);
            cout << "Enter Phone: "; getline(cin, phone);
            cout << "Enter Password: "; getline(cin, password);

            stringstream ss;
            ss << "INSERT INTO Users (Name, Phone, Password) VALUES ('" << name << "', '" << phone << "', '" << password << "')";
            if (mysql_query(conn, ss.str().c_str())) {
                cout << "\nError: " << mysql_error(conn) << endl;
            } else {
                loggedInUserName = name;
                loggedInUserID = (int)mysql_insert_id(conn);
                cout << "\nRegistration successful! Logged in as " << name << endl;
                pause();
                break;
            }
        } else if (choice == 2) {
            string phone, password;
            cout << "\nEnter Phone: "; getline(cin, phone);
            cout << "Enter Password: "; getline(cin, password);

            stringstream ss;
            ss << "SELECT UserID, Name FROM Users WHERE Phone='" << phone << "' AND Password='" << password << "'";
            if (mysql_query(conn, ss.str().c_str())) {
                cout << "\nError: " << mysql_error(conn) << endl;
                pause();
            } else {
                MYSQL_RES* res = mysql_store_result(conn);
                MYSQL_ROW row = mysql_fetch_row(res);
                if (row) {
                    loggedInUserID = atoi(row[0]);
                    loggedInUserName = row[1];
                    cout << "\nWelcome back, " << loggedInUserName << "!\n";
                    mysql_free_result(res);
                    pause();
                    break;
                } else {
                    cout << "\nInvalid credentials. Try again." << endl;
                    mysql_free_result(res);
                    pause();
                }
            }
        } else {
            cout << "Invalid choice!" << endl;
            pause();
        }
    }
}

void displaySeatMap(MYSQL* conn, int showID) {
    int Seat[5][10];
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 10; ++j)
            Seat[i][j] = 1;

    stringstream ss;
    ss << "SELECT RowNumber, SeatNumber FROM Tickets WHERE ShowID = " << showID;
    if (mysql_query(conn, ss.str().c_str())) {
        cout << "Error fetching seats: " << mysql_error(conn) << endl;
        return;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        int r = atoi(row[0]) - 1;
        int c = atoi(row[1]) - 1;
        Seat[r][c] = 0;
    }
    mysql_free_result(res);

    cout << "\n       SCREEN THIS WAY\n\n   ";
    for (int i = 1; i <= 10; i++) cout << i << " ";
    cout << "\n";
    for (int i = 0; i < 5; ++i) {
        cout << "R" << (i + 1) << "  ";
        for (int j = 0; j < 10; ++j) {
            cout << (Seat[i][j] ? "-" : "X") << " ";
        }
        cout << "\n";
    }
}

void showSelection(MYSQL* conn, int& showID, string& movieTitle, string& theatreName, string& showTimeStr) {
    string movieQuery = "SELECT * FROM Movies";
    mysql_query(conn, movieQuery.c_str());
    MYSQL_RES* movies = mysql_store_result(conn);
    MYSQL_ROW row;
    int i = 1;
    cout << "\nAvailable Movies:\n";
    while ((row = mysql_fetch_row(movies))) {
        cout << i++ << ". " << row[1] << endl;
    }
    cout << "\nSelect movie number: ";
    int movieChoice; cin >> movieChoice;
    mysql_data_seek(movies, movieChoice - 1);
    row = mysql_fetch_row(movies);
    int movieID = atoi(row[0]);
    movieTitle = row[1];
    mysql_free_result(movies);

    string theatreQuery = "SELECT * FROM Theatres";
    mysql_query(conn, theatreQuery.c_str());
    MYSQL_RES* theatres = mysql_store_result(conn);
    i = 1;
    cout << "\nAvailable Theatres:\n";
    while ((row = mysql_fetch_row(theatres))) {
        cout << i++ << ". " << row[1] << " - " << row[2] << endl;
    }
    cout << "\nSelect theatre number: ";
    int theatreChoice; cin >> theatreChoice;
    mysql_data_seek(theatres, theatreChoice - 1);
    row = mysql_fetch_row(theatres);
    int theatreID = atoi(row[0]);
    theatreName = row[1];
    mysql_free_result(theatres);

    stringstream ss;
    ss << "SELECT * FROM Shows WHERE MovieID=" << movieID << " AND TheatreID=" << theatreID;
    mysql_query(conn, ss.str().c_str());
    MYSQL_RES* shows = mysql_store_result(conn);

    if (!shows || mysql_num_rows(shows) == 0) {
        cout << "\n? No shows available for this movie in the selected theatre." << endl;
        mysql_free_result(shows);
        showID = -1;
        pause();
        return;
    }

    cout << "\nAvailable Shows:\n";
    i = 1;
    while ((row = mysql_fetch_row(shows))) {
        cout << i++ << ". Show at " << row[3] << endl;
    }

    cout << "\nSelect show number: ";
    int showChoice;
    cin >> showChoice;

    mysql_data_seek(shows, showChoice - 1);
    row = mysql_fetch_row(shows);
    showID = atoi(row[0]);
    showTimeStr = row[3];
    mysql_free_result(shows);
}

void bookTicket(MYSQL* conn, int showID, const string& movieTitle, const string& theatreName, const string& showTimeStr) {
    displaySeatMap(conn, showID);
    int row, seat;
    cout << "\nSelect Row (1-5): "; cin >> row;
    cout << "Select Seat (1-10): "; cin >> seat;
    if (row < 1 || row > 5 || seat < 1 || seat > 10) {
        cout << "Invalid seat selection!" << endl;
        pause();
        return;
    }

    stringstream check;
    check << "SELECT * FROM Tickets WHERE ShowID=" << showID << " AND RowNumber=" << row << " AND SeatNumber=" << seat;
    mysql_query(conn, check.str().c_str());
    MYSQL_RES* res = mysql_store_result(conn);
    if (mysql_num_rows(res) > 0) {
        cout << "\n? Seat already booked!" << endl;
        mysql_free_result(res);
        pause();
        return;
    }
    mysql_free_result(res);

    stringstream book;
    book << "INSERT INTO Tickets (UserID, ShowID, RowNumber, SeatNumber) VALUES ("
         << loggedInUserID << ", " << showID << ", " << row << ", " << seat << ")";
    if (mysql_query(conn, book.str().c_str())) {
        cout << "Error: " << mysql_error(conn) << endl;
    } else {
        cout << "\n? --- Ticket Confirmed ---\n";
        cout << "Name     : " << loggedInUserName << endl;
        cout << "Movie    : " << movieTitle << endl;
        cout << "Theatre  : " << theatreName << endl;
        cout << "ShowTime : " << showTimeStr << endl;
        cout << "Seat     : Row " << row << " Seat " << seat << endl;
    }
    pause();
}

int main() {
    MYSQL* conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {
        cout << "Error: " << mysql_error(conn) << endl;
        return 1;
    }

    mysql_query(conn, "CREATE TABLE IF NOT EXISTS Users (UserID INT AUTO_INCREMENT PRIMARY KEY, Name VARCHAR(100), Phone VARCHAR(15), Password VARCHAR(100))");
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS Movies (MovieID INT AUTO_INCREMENT PRIMARY KEY, Title VARCHAR(100))");
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS Theatres (TheatreID INT AUTO_INCREMENT PRIMARY KEY, Name VARCHAR(100), Location VARCHAR(100))");
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS Shows (ShowID INT AUTO_INCREMENT PRIMARY KEY, MovieID INT, TheatreID INT, ShowTime DATETIME, FOREIGN KEY(MovieID) REFERENCES Movies(MovieID), FOREIGN KEY(TheatreID) REFERENCES Theatres(TheatreID))");
    mysql_query(conn, "CREATE TABLE IF NOT EXISTS Tickets (TicketID INT AUTO_INCREMENT PRIMARY KEY, UserID INT, ShowID INT, RowNumber INT, SeatNumber INT, FOREIGN KEY(UserID) REFERENCES Users(UserID), FOREIGN KEY(ShowID) REFERENCES Shows(ShowID))");

    userLoginOrRegister(conn);

    while (true) {
        system("cls");
        cout << "Welcome to Movie Booking System, " << loggedInUserName << "!\n";
        cout << "1. Book Ticket\n2. Exit\nEnter choice: ";
        int ch; cin >> ch;

        if (ch == 1) {
            int showID;
            string movie, theatre, time;
            showSelection(conn, showID, movie, theatre, time);
            if (showID > 0)
                bookTicket(conn, showID, movie, theatre, time);
        } else if (ch == 2) {
            break;
        } else {
            cout << "Invalid choice!" << endl;
            pause();
        }
    }

    mysql_close(conn);
    return 0;
}
