#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
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
	clear_screen;

	print_header("Available Rooms");

	cout << endl;

	cout << setw(10) << left << "Room No."
		<< setw(15) << left << "Type"
		<< setw(12) << left << "Capacity"
		<< setw(15) << left << "Price"
		<< setw(15) << left << "Status" << endl;

	print_divider();

	bool found = false;

	for (Room room : rooms) {
		if (room.roomStatus == "Available") {
			cout << setw(10) << left << room.roomNumber
				<< setw(15) << left << room.roomType
				<< setw(12) << left << room.capacity
				<< setw(15) << left << fixed << setprecision(2) << room.pricePerNight
				<< setw(15) << left << room.roomStatus << endl;

			found = true;
		}
	}

	if (!found) {
		cout << "No available rooms." << endl;
	}

	print_divider();

	cout << endl;
	cout << "Press Enter to return...";
	cin.get();
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
		cout << "Room not found..." << endl;
	}
	else {
		cout << endl;
		cout << "Room Number : " << room->roomNumber << endl;
		cout << "Room Type   : " << room->roomType << endl;
		cout << "Room Status : " << room->roomStatus << endl;
	}
	cout << endl;
	cout << "Press Enter to return...";
	cin.get();
}


