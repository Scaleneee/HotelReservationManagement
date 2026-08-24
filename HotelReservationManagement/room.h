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

        //check whether the user entered a number
        if (cin >> value) {
            string remaining;
            getline(cin, remaining);

            //only accept positive number
            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                value > 0) {
                return value;
            }
        }
        else {
            //clear invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        //display error message and ask user to enter again
        cout << "Invalid input. Please enter a positive whole number."
            << endl;
    }
}

int getValidatedCapacity(const string& prompt) {
    int value;

    while (true) {
        cout << prompt;

        //check whether the user entered a number
        if (cin >> value) {
            string remaining;
            getline(cin, remaining);

            //only accept capacity from 1 to 9 pax
            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                value >= 1 &&
                value <= 9) {
                return value;
            }
        }
        else {
            //clear invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        //display error message and ask user to enter again
        cout << "Invalid capacity. Please enter a whole number from 1 to 9 pax."
            << endl;
    }
}


double getValidatedPositiveDouble(const string& prompt) {
    double value;

    while (true) {
        cout << prompt;

        //check whether the user enter a number
        if (cin >> value) {
            string remaining;
            getline(cin, remaining);

            //only accept the value > 0
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

        //check whether the input is not empty or only spacing
        if (!value.empty() &&
            value.find_first_not_of(" \t") != string::npos) {
            return value;
        }

        cout << "Input cannot be empty. Please try again." << endl;
    }
}

int getValidatedCreateRoomNumber(const string& prompt) {
    int roomNumber;

    while (true) {
        cout << prompt;

        if (cin >> roomNumber) {
            string remaining;
            getline(cin, remaining);

            // 0 to back
            if (roomNumber == 0 &&
                remaining.find_first_not_of("\t\r") == string::npos) {
                return 0;
            }

            //room number must be 3 digit
            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                roomNumber >= 100 &&
                roomNumber <= 999) {
                return roomNumber;
            }
        }
        else {
            //clear invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Invalid input. Please enter a valid room number (3-digits) or 0 to Back."
            << endl;
    }
}

int getValidatedRoomNumberOrBack(const string& prompt) {
    int roomNumber;

    while (true) {
        cout << prompt;

        if (cin >> roomNumber) {
            string remaining;
            getline(cin, remaining);

            //0 to back
            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                roomNumber >= 0) {
                return roomNumber;
            }
        }
        else {
            //clear invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Invalid input. Please enter a valid room number or 0 to Back."
            << endl;
    }
}

string getValidatedRoomType(const string& prompt) {
    string roomType;

    while (true) {
        cout << prompt;
        getline(cin, roomType);

        //convert input to uppercase for easy compare
        string upperType = roomType;

        transform(
            upperType.begin(),
            upperType.end(),
            upperType.begin(),
            ::toupper
        );

        //check whether the room type is valid
        if (upperType == "STANDARD") {
            return "Standard";
        }
        else if (upperType == "DELUXE") {
            return "Deluxe";
        }
        else if (upperType == "FAMILY") {
            return "Family";
        }
        else if (upperType == "SUITE") {
            return "Suite";
        }

        cout << "Invalid room type." << endl;
        cout << "Please enter Standard, Deluxe, Family, or Suite."
            << endl;
    }
}

string getValidatedRoomTypeChoice(const string& prompt) {
    int choice;

    while (true) {
        cout << "Room Type:" << endl;
        cout << " [1] Standard" << endl;
        cout << " [2] Deluxe" << endl;
        cout << " [3] Family" << endl;
        cout << " [4] Suite" << endl;

        cout << endl;
        cout << prompt;

        if (cin >> choice) {
            string remaining;
            getline(cin, remaining);

            //only accept 1-4
            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                choice >= 1 &&
                choice <= 4) {

                switch (choice) {
                case 1:
                    return "Standard";

                case 2:
                    return "Deluxe";

                case 3:
                    return "Family";

                case 4:
                    return "Suite";
                }
            }
        }
        else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Invalid room type. Please enter a number from 1 to 4."
            << endl;
        cout << endl;
    }
}


int getValidatedStatusChoice(const string& prompt) {
    int choice;

    while (true) {
        cout << prompt;

        if (cin >> choice) {
            string remaining;
            getline(cin, remaining);

            //only accept 1-5
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

            //allow 0 return to previous page
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

    //search through all room records
    for (Room& room : rooms) {

        //check whether the room number is match
        if (room.roomNumber == roomNumber) {

            //return the address of matched room
            return &room;
        }
    }

    //return nullptr if the room is not found
    return nullptr;
}

void displayAvailableRooms() {
    clear_screen();

    print_header("Available Rooms");

    cout << endl;

    //display the table headings
    cout << setw(10) << left << "Room No."
        << setw(15) << left << "Type"
        << setw(12) << left << "Capacity"
        << setw(15) << left << "Price"
        << setw(15) << left << "Status"
        << endl;

    print_divider();

    //store only rooms that are currently available
    vector<Room> availableRooms;

    //search through all room records
    for (const Room& room : rooms) {
        if (room.roomStatus == "Available") {
            availableRooms.push_back(room);
        }
    }

    //sort available rooms by room number in ascending
    sort(
        availableRooms.begin(),
        availableRooms.end(),
        [](const Room& a, const Room& b) {
            return a.roomNumber < b.roomNumber;
        }
    );

    //check whether there are any available room
    if (availableRooms.empty()) {
        cout << "No available rooms." << endl;
    }
    else {
        //display all available room information
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

    //return to previous page
    system("pause");
    clear_screen();
}

void checkRoomStatus() {

    // Keep the search page active until user chooses back
    while (true) {
        clear_screen();

        print_header("Check Room Status by Room Type");

        empty_line();

        // Display room type options
        cout << "|  Room Type:" << setw(80 - 13) << right << "|" << endl;
        cout << "|  [1] Standard" << setw(80 - 16) << right << "|" << endl;
        cout << "|  [2] Deluxe" << setw(80 - 14) << right << "|" << endl;
        cout << "|  [3] Family" << setw(80 - 14) << right << "|" << endl;
        cout << "|  [4] Suite" << setw(80 - 13) << right << "|" << endl;

        empty_line();

        cout << "|  [0] Back" << setw(80 - 11) << right << "|" << endl;

        print_divider_with_space(false);

        int typeChoice = get_menu_choice(4);

        // Return to previous menu
        if (typeChoice == 0) {
            return;
        }

        string selectedType;

        // Convert user's choice into corresponding room type
        switch (typeChoice) {
        case 1:
            selectedType = "Standard";
            break;

        case 2:
            selectedType = "Deluxe";
            break;

        case 3:
            selectedType = "Family";
            break;

        case 4:
            selectedType = "Suite";
            break;
        }

        clear_screen();

        print_header("Room Status - " + selectedType);

        cout << endl;

        // Display table
        cout << setw(10) << left << "Room No."
            << setw(15) << left << "Type"
            << setw(12) << left << "Capacity"
            << setw(15) << left << "Price"
            << setw(15) << left << "Status"
            << endl;

        print_divider();

        bool found = false;

        // Store rooms that match the selected room type
        vector<Room> matchedRooms;

        // Search through all room records
        for (const Room& room : rooms) {

            if (room.roomType == selectedType) {
                matchedRooms.push_back(room);
            }
        }

        // Sort matched rooms by room number in ascending order
        sort(
            matchedRooms.begin(),
            matchedRooms.end(),
            [](const Room& a, const Room& b) {
                return a.roomNumber < b.roomNumber;
            }
        );

        // Display all rooms that match the selected room type
        for (const Room& room : matchedRooms) {

            found = true;

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

        // Display message if no matching room is found
        if (!found) {
            cout << "No rooms found with room type: "
                << selectedType << endl;
        }

        print_divider();

        system("pause");
    }
}

void searchRoom() {
    clear_screen();

    print_header("Search Room");

    cout << endl;

    cout << "Current" << endl;

    print_divider();

    //display table
    cout << setw(10) << left << "Room No."
        << setw(15) << left << "Type"
        //<< setw(12) << left << "Capacity"
        //<< setw(15) << left << "Price"
        << setw(15) << left << "Status"
        << endl;

    print_divider();

    //check whether there are any room records
    if (rooms.empty()) {
        cout << "No room records found." << endl;
    }
    else {
        //copy room records for sorting
        vector<Room> sortedRooms = rooms;

        //sort rooms by room number in ascending
        sort(
            sortedRooms.begin(),
            sortedRooms.end(),
            [](const Room& a, const Room& b) {
                return a.roomNumber < b.roomNumber;
            }
        );

        //display all current room records
        for (const Room& room : sortedRooms) {
            cout << setw(10) << left << room.roomNumber
                << setw(15) << left << room.roomType
                //<< setw(12) << left << room.capacity
                //<< setw(15) << left
                //<< fixed << setprecision(2)
                //<< room.pricePerNight
                << setw(15) << left
                << room.roomStatus
                << endl;
        }
    }

    print_divider();

    cout << endl;

    //allow user search for multiple rooms
    while (true) {

        //get the room number to search
        int roomNumber = getValidatedRoomNumberOrBack(
            "Enter room number to search [0 to Back]: "
        );

        //return previous page
        if (roomNumber == 0) {
            return;
        }

        Room* room = get_room_by_number(roomNumber);

        //ask again if the room does not exist
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

        cout << endl;
        cout << "[1] Continue Search" << endl;
        cout << "[0] Back" << endl;

        int choice;

        //validate choice
        while (true) {
            cout << "Enter choice: ";

            if (cin >> choice) {
                string remaining;
                getline(cin, remaining);

                if (remaining.find_first_not_of("\t\r") == string::npos
                    && (choice == 0 || choice == 1)) {
                    break;
                }
            }
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Invalid choice. Please enter 0 or 1." << endl;
        }
        if (choice == 0) {
            return;
        }if (choice == 1) {
            return searchRoom();
        }
        //1 continue to another loop
        cout << endl;
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

    //validate the room number and prevent duplicate
    while (true) {
        newRoom.roomNumber = getValidatedCreateRoomNumber(
            "Enter room number [0 to Back]: "
        );

        //return to previous menu
        if (newRoom.roomNumber == 0) {
            return;
        }

        //check whether the room number already exist
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


    //get and validate the room type
    newRoom.roomType = getValidatedRoomTypeChoice(
        "Enter room type [1-4]: "
    );


    //get description (no need validate)
    newRoom.description = getValidatedText(
        "Enter description     : "
    );

    //get and validate capacity
    newRoom.capacity = getValidatedCapacity(
        "Enter capacity [1-9 pax] : "
    );
    //get and validate the price
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

    //get and validate the selected room status
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

    //add the new room record 
    rooms.push_back(newRoom);

    //save the updated room records
    save_rooms_to_file();

    cout << endl;
    cout << "Room created successfully!!" << endl;

    print_header("Room Information");

    cout << "Room Number     : "
        << newRoom.roomNumber << endl;

    cout << "Room Type       : "
        << newRoom.roomType << endl;

    cout << "Description     : "
        << newRoom.description << endl;

    cout << "Capacity        : "
        << newRoom.capacity << endl;

    cout << "Price Per Night : RM "
        << fixed << setprecision(2)
        << newRoom.pricePerNight << endl;

    cout << "Room Status     : "
        << newRoom.roomStatus << endl;

    print_divider();
    system("pause");
}

void updateRoomInfo() {
    clear_screen();

    print_header("Update Room");

    cout << endl;

    Room* room = nullptr;

    //keep asking until the valid room is found
    while (room == nullptr) {
        int roomNumber = getValidatedRoomNumberOrBack(
            "Enter room number to update [0 to Back]: "
        );

        //return to previous menu
        if (roomNumber == 0) {
            return;
        }

        //search for the room by room number
        room = get_room_by_number(roomNumber);

        if (room == nullptr) {
            cout << endl;
            cout << "Room not found. Please try again." << endl;
            cout << endl;
        }
    }

    while (true) {
        clear_screen();

        print_header("Update Room");

        cout << endl;

        //display current room information
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

        print_header("Select Item to Update");

        cout << endl;

        cout << " [1] Room Type" << endl;
        cout << " [2] Description" << endl;
        cout << " [3] Capacity" << endl;
        cout << " [4] Price Per Night" << endl;
        cout << " [5] Room Status" << endl;

        cout << endl;

        cout << " [0] Back" << endl;

        cout << endl;

        int updateChoice = get_menu_choice(5);

        //return to previous menu
        if (updateChoice == 0) {
            return;
        }

        cout << endl;

        switch (updateChoice) {
        case 1:
            //display current room type & get the new room type
            cout << "Current Room Type : "
                << room->roomType << endl;

            cout << endl;

            room->roomType = getValidatedRoomTypeChoice(
                "Enter new room type [1-4]: "
            );
            break;

        case 2:
            //display current description & get the new description
            cout << "Current Description : "
                << room->description << endl;

            room->description = getValidatedText(
                "Enter new description : "
            );
            break;

        case 3:
            //display current capacity & get the new capacity
            cout << "Current Capacity : "
                << room->capacity << endl;

            room->capacity = getValidatedCapacity(
                "Enter new capacity [1-9 pax] : "
            );
            break;

        case 4:
            //display current price and get the new price
            cout << "Current Price Per Night : RM "
                << fixed << setprecision(2)
                << room->pricePerNight << endl;

            room->pricePerNight = getValidatedPositiveDouble(
                "Enter new price per night : RM "
            );
            break;

        case 5:
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

            //get and validate the new room status
            {
                int statusChoice = getValidatedStatusChoice(
                    "Enter new status [1-5]: "
                );

                //update the room status based on the user's choice
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
            }
            break;
        }

        save_rooms_to_file();

        cout << endl;
        cout << "Room updated successfully!" << endl;

        waitForBack();

        return;
    }
}

void deleteRoom() {
    clear_screen();

    print_header("Delete Room");

    cout << endl;

    //keep asking until the valid room is selected or choose back
    while (true) {
        int roomNumber = getValidatedRoomNumberOrBack(
            "Enter room number to delete [0 to Back]: "
        );

        if (roomNumber == 0) {
            return;
        }

        Room* room = get_room_by_number(roomNumber);

        //ask again if the room does not exist
        if (room == nullptr) {
            cout << endl;
            cout << "Room not found. Please try again." << endl;
            cout << endl;

            continue;
        }

        //prevent deletion if the room is not available
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

        cout << endl;

        //display current room information before delete
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

        cout << endl;

        cout << "Are you sure you want to delete Room "
            << room->roomNumber << "?" << endl;

        cout << endl;

        cout << " [1] Confirm Delete" << endl;
        cout << " [0] Cancel" << endl;

        cout << endl;

        int confirmChoice;

        //validate delete confirmation
        while (true) {
            cout << "Enter choice: ";

            if (cin >> confirmChoice) {
                string remaining;
                getline(cin, remaining);

                if (remaining.find_first_not_of(" \t\r") == string::npos &&
                    (confirmChoice == 0 || confirmChoice == 1)) {
                    break;
                }
            }
            else {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "Invalid choice. Please enter 0 or 1." << endl;
        }

        //cancel delete and return to previous page
        if (confirmChoice == 0) {
            cout << endl;
            cout << "Delete cancelled." << endl;

            waitForBack();

            return;
        }

        //remove the selected room from the room vector
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

        //save the updated room records into the file
        save_rooms_to_file();

        cout << endl;
        cout << "Room deleted successfully." << endl;

        waitForBack();

        return;
    }
}

void room_availability_menu() {
    int choice;

    //keep displaying the menu until the user choose back
    do {
        clear_screen();

        print_header("Room Availability Management");
        empty_line();

        //display all room availability management option
        cout << "|  [1] View Available Rooms" << setw(80 - 27) << right << "|" << endl;
        cout << "|  [2] Search Room" << setw(80 - 18) << right << "|" << endl;
        cout << "|  [3] Check Room Status" << setw(80 - 24) << right << "|" << endl;
        cout << "|  [4] Create New Room" << setw(80 - 22) << right << "|" << endl;
        cout << "|  [5] Update Room" << setw(80 - 18) << right << "|" << endl;
        cout << "|  [6] Delete Room" << setw(80 - 18) << right << "|" << endl;
        empty_line();
        cout << "|  [0] Back" << setw(80 - 11) << right << "|" << endl;
        print_divider_with_space(false);

        //get and validate the user's menu choice
        choice = get_menu_choice(6);

        //call the selected room management function
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

            //return to previous page
        case 0:
            return;
        }

    } while (choice != 0);
}