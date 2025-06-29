# 🎬 Movie Ticket Booking System

A **C++ console-based movie ticket booking system** that allows users to log in or register, select movies, choose theatres and shows, view available seats, and book tickets with MySQL database integration.

---

## 🚀 Project Overview

- **Language**: C++
- **Database**: MySQL
- **IDE**: Embarcadero Dev-C++
- **Platform**: Windows  
- **Description**:  
  This project uses MySQL C API to connect a C++ console app to a MySQL database for managing movies, theatres, shows, seats, and users.

---

## ✨ Features

- ✅ User Registration & Login  
- 🎥 Movie → 🎭 Theatre → 🕒 Show selection  
- 💺 Seat Availability Display (5×10 layout)  
- 💾 MySQL Integration using C API  
- 🎟️ Ticket Confirmation & Storage  
- 🛠️ Auto-Creates Required Tables on First Run  

---

## 🧰 Technologies Used

| Tool                  | Version       |
|-----------------------|---------------|
| C++                   | ISO C++11+    |
| MySQL Server          | 8.0           |
| MySQL C Connector     | 8.0           |
| Embarcadero Dev-C++   | 6.3           |
| Windows               | 10+           |

---

## 🛠️ Setup & Run Instructions

### 🧩 Step 1: Install Required Software

- MySQL Server & MySQL Workbench  
- Embarcadero Dev-C++  
- MySQL Connector/C++  

### 🗃️ Step 2: Database Setup

You have two options:

- ✅ Let `main.cpp` auto-create all required tables  
**OR**
- 📝 Run `create_tables.sql` manually in MySQL Workbench

### ⚙️ Step 3: Configure Dev-C++

- Add **MySQL `include` directory** to Compiler Include Paths  
- Add **MySQL `lib` directory** to Linker Library Paths  
- Link against `libmysql.dll` or place it beside `Project1.exe`

### ▶️ Step 4: Build & Run

- Open `main.cpp` in Dev-C++
- Press `F11` to compile and run
- Use console to register/login, select movie/theatre/show, and book your ticket!

---

## 🗄️ Database Schema

### 📋 `Users` Table
- `UserID` INT, AUTO_INCREMENT, PRIMARY KEY  
- `Name` VARCHAR(100)  
- `Phone` VARCHAR(15)  
- `Password` VARCHAR(100)  

### 🎥 `Movies` Table
- `MovieID` INT, AUTO_INCREMENT, PRIMARY KEY  
- `Title` VARCHAR(100)  

### 🎭 `Theatres` Table
- `TheatreID` INT, AUTO_INCREMENT, PRIMARY KEY  
- `Name` VARCHAR(100)  
- `Location` VARCHAR(100)  

### 📅 `Shows` Table
- `ShowID` INT, AUTO_INCREMENT, PRIMARY KEY  
- `MovieID` INT (FK)  
- `TheatreID` INT (FK)  
- `ShowTime` DATETIME  

### 🎟️ `Tickets` Table
- `TicketID` INT, AUTO_INCREMENT, PRIMARY KEY  
- `UserID` INT (FK)  
- `ShowID` INT (FK)  
- `RowNumber` INT  
- `SeatNumber` INT  

---

## 🗂 Repository Structure
📁 movie-ticket-booking-system/
├── main.cpp
├── create_tables.sql
├── .gitignore
├── README.md
├── Makefile.win
├── Project1.dev
├── libmysql.dll





