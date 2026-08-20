#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include "models.h"
#include "file_io.h"
#include "ui.h"

using namespace std;

int getValidatedPositiveInt(const string& prompt) {
    int value;

    while (true) {
        cout << prompt;

        if (cin >> value) {
            string remaining;
            getline(cin, remaining);

            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                value > 0) {
                return value;
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Invalid input. Please enter a positive whole number."
            << endl;
    }
}

double getValidatedPositiveDouble(const string& prompt) {
    double value;

    while (true) {
        cout << prompt;

        if (cin >> value) {
            string remaining;
            getline(cin, remaining);

            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                value > 0) {
                return value;
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Invalid input. Please enter a value greater than 0."
            << endl;
    }
}


string getValidatedText(const string& prompt) {
    string value;

    while (true) {
        cout << prompt;
        getline(cin, value);

        if (!value.empty() &&
            value.find_first_not_of(" \t") != string::npos) {
            return value;
        }

        cout << "Input cannot be empty. Please try again." << endl;
    }
}

int getValidatedRoomNumberOrBack(const string& prompt) {
    int roomNumber;

    while (true) {
        cout << prompt;

        if (cin >> roomNumber) {
            string remaining;
            getline(cin, remaining);

            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                roomNumber >= 0) {
                return roomNumber;
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Invalid input. Please enter a valid room number or 0 to Back."
            << endl;
    }
}

int getValidatedStatusChoice(const string& prompt) {
    int choice;

    while (true) {
        cout << prompt;

        if (cin >> choice) {
            string remaining;
            getline(cin, remaining);

            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                choice >= 1 &&
                choice <= 5) {
                return choice;
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Invalid status. Please enter a number from 1 to 5."
            << endl;
    }
}

void waitForBack() {
    int choice;

    cout << endl;
    cout << "[0] Back" << endl;

    while (true) {
        cout << "Enter choice: ";

        if (cin >> choice) {
            string remaining;
            getline(cin, remaining);

            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                choice == 0) {
                return;
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Invalid choice. Please enter 0 to Back." << endl;
    }
}

Room* get_room_by_number(int roomNumber) {
    for (Room& room : rooms) {
        if (room.roomNumber == roomNumber) {
            return &room;
        }
    }

    return nullptr;
}

void displayAvailableRooms() {
    clear_screen();

    print_header("Available Rooms");

    cout << endl;

    cout << setw(10) << left << "Room No."
        << setw(15) << left << "Type"
        << setw(12) << left << "Capacity"
        << setw(15) << left << "Price"
        << setw(15) << left << "Status"
        << endl;

    print_divider();

    vector<Room> availableRooms;

    for (const Room& room : rooms) {
        if (room.roomStatus == "Available") {
            availableRooms.push_back(room);
        }
    }

    sort(
        availableRooms.begin(),
        availableRooms.end(),
        [](const Room& a, const Room& b) {
            return a.roomNumber < b.roomNumber;
        }
    );

    if (availableRooms.empty()) {
        cout << "No available rooms." << endl;
    }
    else {
        for (const Room& room : availableRooms) {
            cout << setw(10) << left << room.roomNumber
                << setw(15) << left << room.roomType
                << setw(12) << left << room.capacity
                << setw(15) << left
                << fixed << setprecision(2)
                << room.pricePerNight
                << setw(15) << left
                << room.roomStatus
                << endl;
        }
    }

    print_divider();

    waitForBack();
}

void checkRoomStatus() {
    clear_screen();

    print_header("Check Room Status");

    cout << endl;

    while (true) {
        int roomNumber = getValidatedRoomNumberOrBack(
            "Enter room number [0 to Back]: "
        );

        if (roomNumber == 0) {
            return;
        }

        Room* room = get_room_by_number(roomNumber);

        if (room == nullptr) {
            cout << endl;
            cout << "Room not found. Please try again." << endl;
            cout << endl;

            continue;
        }

        cout << endl;

        print_divider();

        cout << "Room Number : "
            << room->roomNumber << endl;

        cout << "Room Type   : "
            << room->roomType << endl;

        cout << "Room Status : "
            << room->roomStatus << endl;

        print_divider();

        waitForBack();

        return;
    }
}

void searchRoom() {
    clear_screen();

    print_header("Search Room");

    cout << endl;

    cout << "Current Room Information" << endl;

    print_divider();

    cout << setw(10) << left << "Room No."
        << setw(15) << left << "Type"
        << setw(12) << left << "Capacity"
        << setw(15) << left << "Price"
        << setw(15) << left << "Status"
        << endl;

    print_divider();

    if (rooms.empty()) {
        cout << "No room records found." << endl;
    }
    else {
        vector<Room> sortedRooms = rooms;

        sort(
            sortedRooms.begin(),
            sortedRooms.end(),
            [](const Room& a, const Room& b) {
                return a.roomNumber < b.roomNumber;
            }
        );

        for (const Room& room : sortedRooms) {
            cout << setw(10) << left << room.roomNumber
                << setw(15) << left << room.roomType
                << setw(12) << left << room.capacity
                << setw(15) << left
                << fixed << setprecision(2)
                << room.pricePerNight
                << setw(15) << left
                << room.roomStatus
                << endl;
        }
    }

    print_divider();

    cout << endl;

    while (true) {
        int roomNumber = getValidatedRoomNumberOrBack(
            "Enter room number to search [0 to Back]: "
        );

        if (roomNumber == 0) {
            return;
        }

        Room* room = get_room_by_number(roomNumber);

        if (room == nullptr) {
            cout << endl;
            cout << "Room not found. Please try again." << endl;
            cout << endl;

            continue;
        }

        cout << endl;

        print_header("Room Information");

        cout << endl;

        print_divider();

        cout << "Room Number     : "
            << room->roomNumber << endl;

        cout << "Room Type       : "
            << room->roomType << endl;

        cout << "Description     : "
            << room->description << endl;

        cout << "Capacity        : "
            << room->capacity << endl;

        cout << "Price Per Night : RM "
            << fixed << setprecision(2)
            << room->pricePerNight << endl;

        cout << "Room Status     : "
            << room->roomStatus << endl;

        print_divider();

        waitForBack();

        return;
    }
}

void getRoomInfo() {
    searchRoom();
}

void createRoom() {
    clear_screen();

    print_header("Create Room");

    cout << endl;

    Room newRoom;

    while (true) {
        newRoom.roomNumber = getValidatedRoomNumberOrBack(
            "Enter room number [0 to Back]: "
        );

        if (newRoom.roomNumber == 0) {
            return;
        }

        Room* existingRoom =
            get_room_by_number(newRoom.roomNumber);

        if (existingRoom == nullptr) {
            break;
        }

        cout << endl;
        cout << "Room number already exists." << endl;
        cout << "Please enter another room number." << endl;
        cout << endl;
    }

    newRoom.roomType = getValidatedText(
        "Enter room type       : "
    );

    newRoom.description = getValidatedText(
        "Enter description     : "
    );

    newRoom.capacity = getValidatedPositiveInt(
        "Enter capacity        : "
    );

    newRoom.pricePerNight = getValidatedPositiveDouble(
        "Enter price per night : RM "
    );

    cout << endl;

    cout << "Room Status:" << endl;
    cout << " [1] Available" << endl;
    cout << " [2] Reserved" << endl;
    cout << " [3] Occupied" << endl;
    cout << " [4] Housekeeping" << endl;
    cout << " [5] Maintenance" << endl;

    cout << endl;

    int statusChoice = getValidatedStatusChoice(
        "Enter status [1-5]: "
    );

    switch (statusChoice) {
    case 1:
        newRoom.roomStatus = "Available";
        break;

    case 2:
        newRoom.roomStatus = "Reserved";
        break;

    case 3:
        newRoom.roomStatus = "Occupied";
        break;

    case 4:
        newRoom.roomStatus = "Housekeeping";
        break;

    case 5:
        newRoom.roomStatus = "Maintenance";
        break;
    }

    rooms.push_back(newRoom);

    save_rooms_to_file();

    cout << endl;
    cout << "Room created successfully." << endl;

    waitForBack();
}

void updateRoomInfo() {
    clear_screen();

    print_header("Update Room");

    cout << endl;

    Room* room = nullptr;

    while (room == nullptr) {
        int roomNumber = getValidatedRoomNumberOrBack(
            "Enter room number to update [0 to Back]: "
        );

        if (roomNumber == 0) {
            return;
        }

        room = get_room_by_number(roomNumber);

        if (room == nullptr) {
            cout << endl;
            cout << "Room not found. Please try again." << endl;
            cout << endl;
        }
    }

    cout << endl;

    print_header("Current Room Information");

    cout << endl;

    print_divider();

    cout << "Room Number     : "
        << room->roomNumber << endl;

    cout << "Room Type       : "
        << room->roomType << endl;

    cout << "Description     : "
        << room->description << endl;

    cout << "Capacity        : "
        << room->capacity << endl;

    cout << "Price Per Night : RM "
        << fixed << setprecision(2)
        << room->pricePerNight << endl;

    cout << "Room Status     : "
        << room->roomStatus << endl;

    print_divider();

    cout << endl;

    print_header("Enter New Room Information");

    cout << endl;

    cout << "Current Room Type : "
        << room->roomType << endl;

    room->roomType = getValidatedText(
        "Enter new room type   : "
    );

    cout << endl;

    cout << "Current Description : "
        << room->description << endl;

    room->description = getValidatedText(
        "Enter new description : "
    );

    cout << endl;

    cout << "Current Capacity : "
        << room->capacity << endl;

    room->capacity = getValidatedPositiveInt(
        "Enter new capacity    : "
    );

    cout << endl;

    cout << "Current Price Per Night : RM "
        << fixed << setprecision(2)
        << room->pricePerNight << endl;

    room->pricePerNight = getValidatedPositiveDouble(
        "Enter new price per night : RM "
    );

    cout << endl;

    cout << "Current Room Status : "
        << room->roomStatus << endl;

    cout << endl;

    cout << "Room Status:" << endl;
    cout << " [1] Available" << endl;
    cout << " [2] Reserved" << endl;
    cout << " [3] Occupied" << endl;
    cout << " [4] Housekeeping" << endl;
    cout << " [5] Maintenance" << endl;

    cout << endl;

    int statusChoice = getValidatedStatusChoice(
        "Enter new status [1-5]: "
    );

    switch (statusChoice) {
    case 1:
        room->roomStatus = "Available";
        break;

    case 2:
        room->roomStatus = "Reserved";
        break;

    case 3:
        room->roomStatus = "Occupied";
        break;

    case 4:
        room->roomStatus = "Housekeeping";
        break;

    case 5:
        room->roomStatus = "Maintenance";
        break;
    }

    save_rooms_to_file();

    cout << endl;
    cout << "Room updated successfully!" << endl;

    waitForBack();
}

void deleteRoom() {
    clear_screen();

    print_header("Delete Room");

    cout << endl;

    while (true) {
        int roomNumber = getValidatedRoomNumberOrBack(
            "Enter room number to delete [0 to Back]: "
        );

        if (roomNumber == 0) {
            return;
        }

        Room* room = get_room_by_number(roomNumber);

        if (room == nullptr) {
            cout << endl;
            cout << "Room not found. Please try again." << endl;
            cout << endl;

            continue;
        }

        if (room->roomStatus != "Available") {
            cout << endl;

            cout << "Cannot delete Room "
                << room->roomNumber
                << " because it is currently "
                << room->roomStatus
                << "." << endl;

            cout << "Only available rooms can be deleted."
                << endl;

            cout << endl;

            cout << "Please enter another room number "
                << "or enter 0 to Back." << endl;

            cout << endl;

            continue;
        }

        rooms.erase(
            remove_if(
                rooms.begin(),
                rooms.end(),
                [roomNumber](const Room& room) {
                    return room.roomNumber == roomNumber;
                }
            ),
            rooms.end()
        );

        save_rooms_to_file();

        cout << endl;
        cout << "Room deleted successfully." << endl;

        waitForBack();

        return;
    }
}

void room_availability_menu() {
    int choice;

    do {
        clear_screen();

        print_header("Room Availability Management");

        cout << endl;

        cout << " [1] View Available Rooms" << endl;
        cout << " [2] Search / View Room Information" << endl;
        cout << " [3] Check Room Status" << endl;
        cout << " [4] Create New Room" << endl;
        cout << " [5] Update Room" << endl;
        cout << " [6] Delete Room" << endl;

        cout << endl;

        cout << " [0] Back" << endl;

        cout << endl;

        print_divider();

        cout << endl;

        choice = get_menu_choice(6);

        switch (choice) {
        case 1:
            displayAvailableRooms();
            break;

        case 2:
            searchRoom();
            break;

        case 3:
            checkRoomStatus();
            break;

        case 4:
            createRoom();
            break;

        case 5:
            updateRoomInfo();
            break;

        case 6:
            deleteRoom();
            break;

        case 0:
            return;
        }

    } while (choice != 0);
}