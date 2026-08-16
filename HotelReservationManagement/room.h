#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include "models.h"
#include "file_io.h"
#include "ui.h"

using namespace std;

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
        << setw(15) << left << "Status" << endl;

    print_divider();

    vector<Room> availableRooms;

    for (const Room& room : rooms) {
        if (room.roomStatus == "Available") {
            availableRooms.push_back(room);
        }
    }

    sort(availableRooms.begin(), availableRooms.end(),
        [](const Room& a, const Room& b) {
            return a.roomNumber < b.roomNumber;
        });

    if (availableRooms.empty()) {
        cout << "No available rooms." << endl;
    }
    else {
        for (const Room& room : availableRooms) {
            cout << setw(10) << left << room.roomNumber
                << setw(15) << left << room.roomType
                << setw(12) << left << room.capacity
                << setw(15) << left << fixed << setprecision(2)
                << room.pricePerNight
                << setw(15) << left << room.roomStatus << endl;
        }
    }

    print_divider();

    cout << endl;
    cout << "Press Enter to return...";

    string temp;
    getline(cin, temp);
}

void checkRoomStatus() {
    clear_screen();

    print_header("Check Room Status");

    cout << endl;

    int roomNumber;

    cout << "Enter room number: ";
    cin >> roomNumber;
    cin.ignore(1000, '\n');

    Room* room = get_room_by_number(roomNumber);

    if (room == nullptr) {
        cout << endl;
        cout << "Room not found." << endl;
    }
    else {
        cout << endl;
        cout << "Room Number : " << room->roomNumber << endl;
        cout << "Room Type   : " << room->roomType << endl;
        cout << "Room Status : " << room->roomStatus << endl;
    }

    cout << endl;
    cout << "Press Enter to return...";

    string temp;
    getline(cin, temp);
}

void getRoomInfo() {
    clear_screen();

    print_header("Room Information");

    cout << endl;

    int roomNumber;

    cout << "Enter room number: ";
    cin >> roomNumber;
    cin.ignore(1000, '\n');

    Room* room = get_room_by_number(roomNumber);

    if (room == nullptr) {
        cout << endl;
        cout << "Room not found." << endl;
    }
    else {
        cout << endl;

        print_divider();

        cout << "Room Number     : " << room->roomNumber << endl;
        cout << "Room Type       : " << room->roomType << endl;
        cout << "Description     : " << room->description << endl;
        cout << "Capacity        : " << room->capacity << endl;
        cout << "Price Per Night : RM "
            << fixed << setprecision(2)
            << room->pricePerNight << endl;
        cout << "Room Status     : " << room->roomStatus << endl;

        print_divider();
    }

    cout << endl;
    cout << "Press Enter to return...";

    string temp;
    getline(cin, temp);
}

void createRoom() {
    clear_screen();

    print_header("Create Room");

    cout << endl;

    Room newRoom;

    cout << "Enter room number: ";
    cin >> newRoom.roomNumber;
    cin.ignore(1000, '\n');

    Room* existingRoom = get_room_by_number(newRoom.roomNumber);

    if (existingRoom != nullptr) {
        cout << endl;
        cout << "Room number already exists." << endl;

        cout << endl;
        cout << "Press Enter to return...";

        string temp;
        getline(cin, temp);

        return;
    }

    cout << "Enter room type       : ";
    getline(cin, newRoom.roomType);

    cout << "Enter description     : ";
    getline(cin, newRoom.description);

    cout << "Enter capacity        : ";
    cin >> newRoom.capacity;

    cout << "Enter price per night : ";
    cin >> newRoom.pricePerNight;
    cin.ignore(1000, '\n');

    int statusChoice;

    cout << endl;
    cout << "Room Status:" << endl;
    cout << " [1] Available" << endl;
    cout << " [2] Reserved" << endl;
    cout << " [3] Occupied" << endl;
    cout << " [4] Housekeeping" << endl;
    cout << " [5] Maintenance" << endl;

    cout << endl;
    cout << "Enter status [1-5]: ";
    cin >> statusChoice;
    cin.ignore(1000, '\n');

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

    default:
        newRoom.roomStatus = "Available";
        break;
    }

    rooms.push_back(newRoom);

    save_rooms_to_file();

    cout << endl;
    cout << "Room created successfully." << endl;

    cout << endl;
    cout << "Press Enter to return...";

    string temp;
    getline(cin, temp);
}

void updateRoomInfo() {
    clear_screen();

    print_header("Update Room");

    cout << endl;

    int roomNumber;

    cout << "Enter room number to update: ";
    cin >> roomNumber;
    cin.ignore(1000, '\n');

    Room* room = get_room_by_number(roomNumber);

    if (room == nullptr) {
        cout << endl;
        cout << "Room not found." << endl;

        cout << endl;
        cout << "Press Enter to return...";

        string temp;
        getline(cin, temp);

        return;
    }

    cout << endl;

    cout << "Enter new room type         : ";
    getline(cin, room->roomType);

    cout << "Enter new description       : ";
    getline(cin, room->description);

    cout << "Enter new capacity          : ";
    cin >> room->capacity;

    cout << "Enter new price per night   : ";
    cin >> room->pricePerNight;
    cin.ignore(1000, '\n');

    int statusChoice;

    cout << endl;
    cout << "Room Status:" << endl;
    cout << " [1] Available" << endl;
    cout << " [2] Reserved" << endl;
    cout << " [3] Occupied" << endl;
    cout << " [4] Housekeeping" << endl;
    cout << " [5] Maintenance" << endl;

    cout << endl;
    cout << "Enter status [1-5]: ";
    cin >> statusChoice;
    cin.ignore(1000, '\n');

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

    default:
        room->roomStatus = "Available";
        break;
    }

    save_rooms_to_file();

    cout << endl;
    cout << "Room updated successfully!" << endl;

    cout << endl;
    cout << "Press Enter to return...";

    string temp;
    getline(cin, temp);
}

void deleteRoom() {
    clear_screen();

    print_header("Delete Room");

    cout << endl;

    int roomNumber;

    cout << "Enter room number: ";
    cin >> roomNumber;
    cin.ignore(1000, '\n');

    bool found = false;

    for (const Room& room : rooms) {
        if (room.roomNumber == roomNumber) {
            found = true;

            if (room.roomStatus != "Available") {
                cout << endl;

                cout << "Cannot delete Room "
                    << room.roomNumber
                    << " because it is currently "
                    << room.roomStatus
                    << "." << endl;

                cout << endl;
                cout << "Only available rooms can be deleted." << endl;

                cout << endl;
                cout << "Press Enter to return...";

                string temp;
                getline(cin, temp);

                return;
            }
        }
    }

    if (!found) {
        cout << endl;
        cout << "Room not found." << endl;

        cout << endl;
        cout << "Press Enter to return...";

        string temp;
        getline(cin, temp);

        return;
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

    cout << endl;
    cout << "Press Enter to return...";

    string temp;
    getline(cin, temp);
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
            getRoomInfo();
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
            break;

        default:
            break;
        }

    } while (choice != 0);
}