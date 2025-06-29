-- Create and use the database
CREATE DATABASE IF NOT EXISTS mydb;
USE mydb;

-- Users Table
CREATE TABLE IF NOT EXISTS Users (
    UserID INT AUTO_INCREMENT PRIMARY KEY,
    Name VARCHAR(100),
    Phone VARCHAR(15),
    Password VARCHAR(100)
);

-- Movies Table
CREATE TABLE IF NOT EXISTS Movies (
    MovieID INT AUTO_INCREMENT PRIMARY KEY,
    Title VARCHAR(100)
);

-- Theatres Table
CREATE TABLE IF NOT EXISTS Theatres (
    TheatreID INT AUTO_INCREMENT PRIMARY KEY,
    Name VARCHAR(100),
    Location VARCHAR(100)
);

-- Shows Table
CREATE TABLE IF NOT EXISTS Shows (
    ShowID INT AUTO_INCREMENT PRIMARY KEY,
    MovieID INT,
    TheatreID INT,
    ShowTime DATETIME,
    FOREIGN KEY (MovieID) REFERENCES Movies(MovieID),
    FOREIGN KEY (TheatreID) REFERENCES Theatres(TheatreID)
);

-- Tickets Table
CREATE TABLE IF NOT EXISTS Tickets (
    TicketID INT AUTO_INCREMENT PRIMARY KEY,
    UserID INT,
    ShowID INT,
    RowNumber INT,
    SeatNumber INT,
    FOREIGN KEY (UserID) REFERENCES Users(UserID),
    FOREIGN KEY (ShowID) REFERENCES Shows(ShowID)
);

-- Insert Sample Movies
INSERT INTO Movies (Title) VALUES 
('Inception'),
('Interstellar'),
('Avengers: Endgame'),
('The Dark Knight'),
('Jawan');

-- Insert Sample Theatres
INSERT INTO Theatres (Name, Location) VALUES 
('INOX', 'Hyderabad'),
('PVR Cinemas', 'Mumbai'),
('GVK One Mall Theatre', 'Hyderabad'),
('AMB Cinemas', 'Bangalore');

-- Insert Sample Shows (Adjust times as needed)
INSERT INTO Shows (MovieID, TheatreID, ShowTime) VALUES
(1, 1, '2025-06-30 18:30:00'),  -- Inception at INOX
(2, 1, '2025-06-30 21:00:00'),  -- Interstellar at INOX
(3, 2, '2025-06-30 20:00:00'),  -- Avengers at PVR
(4, 3, '2025-07-01 17:00:00'),  -- Dark Knight at GVK
(5, 4, '2025-07-01 19:45:00');  -- Jawan at AMB
