#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "models.h"

using namespace std;

/*
	search the list and return the reservation obj pointer
*/
Reservation* get_reservation_by_id(string reservationID) {
	for (Reservation& reservation : reservations)
	{
		if (reservation.reservationID == reservationID) {
			return &reservation;
		}
	}
	return nullptr;
}


// Found Valid Customer
bool isValidCustomer(string customerID) {
	for (int i = 0; i < (int)customers.size(); i++) {
		if (customers[i].customerID == customerID) {
			return true;
		}
	}
	return false;
}

// Room Number
bool isValidRoomNumber(int roomNumber) {
	for (int i = 0; i < (int)rooms.size(); i++) {
		if (rooms[i].roomNumber == roomNumber) {
			return true;
		}
	}
	return false;
}

// generate the reservation id into :"RES001, RES002, RES003".....
string generateReservationID() {
	int id = reservations.size() + 1;		// start from number 001
	string number = to_string(id);

	while (number.length() < 3) {
		number = "0" + number;
	}
	return "RES" + number;
}

// The format of date
bool DateFormat(string date) {
	regex datePattern(R"(^(0[1-9]|[12][0-9]|3[0-1])/(0[1-9]|1[0-2])/[0-9]{4}$)");
	return regex_search(date, datePattern);
}

// Break a date into three seperate part
bool parseDate(string date, int& day, int& month, int& year) {
	stringstream ss(date);
	char slash1, slash2;

	ss >> day >> slash1 >> month >> slash2 >> year;

	if (slash1 != '/' || slash2 != '/') {
		return false;
	}

	if (day < 1 || day > 31 || month < 1 || month > 12 || year < 2000) {
		return false;
	}
	return true;
}

// Stay duration
int dayCount(int day, int month, int year) {
	return (year * 365) + (month * 30) + day;
}

// Room availability
bool roomAvailability(int roomNumber, int checkinDayCount, int checkoutDayCount) {
	for (int i = 0; i < (int)reservations.size(); i++) {
		Reservation existing = reservations[i];

		bool sameRoom = (existing.roomNumber == roomNumber);
		bool activeRoom = (existing.reservationStatus == "Booked" ||
			existing.reservationStatus == "CheckedIn");

		int existCheckinDay, existCheckinMonth, existCheckinYear;
		int existCheckoutDay, existCheckoutMonth, existCheckoutYear;

		if (sameRoom && activeRoom) {
			// check existing reservation's date
			parseDate(existing.checkInDate, existCheckinDay, existCheckinMonth, existCheckinYear);
			parseDate(existing.checkOutDate, existCheckoutDay, existCheckoutMonth, existCheckoutYear);

			// count stay duration
			int existingCheckinDayCount = dayCount(existCheckinDay, existCheckinMonth, existCheckinYear);
			int existingCheckoutDayCount = dayCount(existCheckoutDay, existCheckoutMonth, existCheckoutYear);

			// prevent overlapping
			bool overlap = (checkinDayCount < existingCheckoutDayCount) &&
				(checkoutDayCount > existingCheckinDayCount);

			if (overlap) {
				return false; // means that the rooms is not free
			}
		}
	}
	return true;		// if the date is valid, and there is no overlapping, then it means the rooms is free
}

// Room price per night
double roomPricePerNight(int roomNumber) {
	for (int i = 0; i < (int)rooms.size(); i++) {
		if (rooms[i].roomNumber == roomNumber) {
			return rooms[i].pricePerNight;
		}
	}
	return 0;
}

// Search Reservation ID
int searchingReservationID(string reservationID) {
	for (int i = 0; i < (int)reservations.size(); i++) {
		if (reservations[i].reservationID == reservationID) {
			return i;
		}
	}
	return -1;
}

// Booking Confirmation
void bookingConfirmation(const Reservation& reservation) {
	cout << "----------Booking Confirmation----------" << endl;
	cout << left << setw(20) << "Reservation ID	: " << reservation.reservationID << endl;
	cout << left << setw(20) << "Customer ID : " << reservation.customerID << endl;
	cout << left << setw(20) << "Room Number : " << reservation.roomNumber << endl;
	cout << left << setw(20) << "Check-In Date : " << reservation.checkInDate << endl;
	cout << left << setw(20) << "Check-Out Date : " << reservation.checkOutDate << endl;
	cout << left << setw(20) << "No. of Customer : " << reservation.numberOfGuests << endl;
	cout << left << setw(20) << "Stay Duration : " << reservation.numberOfNights << endl;
	cout << left << setw(20) << "Room Price	: " << reservation.roomPrice << endl;
	cout << left << setw(20) << "Room Status : " << reservation.reservationStatus << endl;
	cout << "------------------------------- \n";
}

// Create new reservation
void createReservation() {
	Reservation newReservation;

	cout << "\n---------- New Reservation ----------\n";

	// Find Customer using Customer ID
	bool validCustomer;
	do {
		cout << "Enter Customer's ID :   ";
		getline(cin, newReservation.customerID);

		validCustomer = isValidCustomer(newReservation.customerID);
		if (!validCustomer) {
			cout << "Customer ID not found, Please try again." << endl;
		}
	} while (!validCustomer);

	// Room Number of Customer
	bool validRoomNumber;
	do {
		cout << "Enter Room Number :   ";
		cin >> newReservation.roomNumber;
		cin.ignore();

		validRoomNumber = isValidRoomNumber(newReservation.roomNumber);
		if (!validRoomNumber) {
			cout << "Room number not found... Please try again." << endl;
		}


	} while (!validRoomNumber);


	// Check-in Date
	int checkinDay, checkinMonth, checkinYear, checkoutDay, checkoutMonth, checkoutYear;
	bool validCheckinDate;
	do {
		cout << "Enter Check-in Date (DD/MM/YYYY) :   ";
		getline(cin, newReservation.checkInDate);

		validCheckinDate = true;

		if (!DateFormat(newReservation.checkInDate)) {
			cout << "Invalid date format... Please enter the date with valid format (DD/MM/YYYY)" << endl;
			validCheckinDate = false;
		}
		else {
			validCheckinDate = parseDate(newReservation.checkInDate, checkinDay, checkinMonth, checkinYear);

			if (!validCheckinDate) {
				cout << "Invalid day or month value... Please try again..." << endl;
			}

		}
	} while (!validCheckinDate);

	//Check-out Date
	bool validCheckoutDate;
	do {
		cout << "Enter Check-out Date (DD/MM/YYYY) :   ";
		getline(cin, newReservation.checkOutDate);

		if (!DateFormat(newReservation.checkOutDate)) {
			cout << "Invalid date format... Please enter the date with valid format (DD/MM/YYYY)" << endl;
			validCheckoutDate = false;
		}
		else {
			validCheckoutDate = parseDate(newReservation.checkOutDate, checkoutDay, checkoutMonth, checkoutYear);

			if (!validCheckoutDate) {
				cout << "Invalid day or month value... Please try again..." << endl;
			}
		}
	} while (!validCheckoutDate);

	// Make sure the check-out date is after check-in date
	int checkinDayCount = dayCount(checkinDay, checkinMonth, checkinYear);
	int checkoutDayCount = dayCount(checkoutDay, checkoutMonth, checkoutYear);

	if (checkoutDayCount <= checkinDayCount) {
		cout << "Check-out date must be after check-in date. Please try again..." << endl;
		return;
	}

	// Check if the room is available for the reservation dates	
	if (!roomAvailability(newReservation.roomNumber, checkinDayCount, checkoutDayCount)) {
		cout << "Sorry, Room " << newReservation.roomNumber
			<< " is booked for those dates. Reservation cancelled..." << endl;
		return;
	}

	// Number of customer
	cout << "Enter Number of Guests : ";
	cin >> newReservation.numberOfGuests;
	cin.ignore();

	// update the module system
	newReservation.reservationID = generateReservationID();
	newReservation.numberOfNights = checkoutDayCount - checkinDayCount;
	newReservation.reservationStatus = "Booked";
	newReservation.actualCheckInTime = "";
	newReservation.actualCheckOutTime = "";
	newReservation.cancellationReason = "";
	newReservation.roomPrice = roomPricePerNight(newReservation.roomNumber) * newReservation.numberOfNights;

	// add the reservation into the end of the reservation vector
	reservations.push_back(newReservation);

	//create reservation success message
	cout << "\nReservation created successfully!\n";

	// Print Booking Confirmation
	bookingConfirmation(newReservation);
}

// Customer check in
void customerCheckin() {
	string reservationID;
	cout << "\n---------- Customer Check-In----------\n";
	cout << "Enter Reservation ID : ";
	getline(cin, reservationID);

	// searching reservation id
	int searchingID = searchingReservationID(reservationID);

	if (searchingID == -1) {
		cout << "Reservation ID not found.\n";
		return;
	}

	if (reservations[searchingID].reservationStatus != "Booked") {
		cout << "This reservation cannot be checked in." << endl;
		cout << "(current status : " << reservations[searchingID].reservationStatus << ")\n";
		return;
	}

	cout << "Enter actual check-in time (e.g. : 23:59) : ";
	getline(cin, reservations[searchingID].actualCheckInTime);

	// update reservation status
	reservations[searchingID].reservationStatus = "CheckedIn";

	// customer check in success message
	cout << "Customer checked in successfully for reservation " << reservationID << "\n";
}

// Customer check out
void customerCheckout() {
	string reservationID;
	cout << "\n---------- Customer Check-Out----------\n";
	cout << "Enter Reservation ID : ";
	getline(cin, reservationID);

	// searching reservation id
	int searchingID = searchingReservationID(reservationID);

	if (searchingID == -1) {
		cout << "Reservation ID not found.\n";
		return;
	}

	if (reservations[searchingID].reservationStatus != "CheckedIn") {
		cout << "This reservation cannot be checked out." << endl;
		cout << "(current status : " << reservations[searchingID].reservationStatus << ")\n";
		return;
	}

	cout << "Enter actual check-in time (e.g. : 23:59) : ";
	getline(cin, reservations[searchingID].actualCheckOutTime);

	// update reservation status
	reservations[searchingID].reservationStatus = "CheckedOut";

	// customer check out success message
	cout << "Customer checked out successfully for reservation " << reservationID << "\n";
}

// Cancellation of reservation
void cancelReservation() {
	string reservationID;
	cout << "\n---------- Cancel Reservation ----------\n";
	cout << "Enter Reservation ID : ";
	getline(cin, reservationID);

	// searching reservation id
	int searchingID = searchingReservationID(reservationID);

	if (searchingID == -1) {
		cout << "Reservation ID not found.\n";
		return;
	}

	if (reservations[searchingID].reservationStatus == "Cancelled" ||
		reservations[searchingID].reservationStatus == "CheckedOut") {
		cout << "This reservation cannot be cancel. " << endl;
		cout << "(current status : " << reservations[searchingID].reservationStatus << ")\n";
		return;
	}

	// user input cancel reason
	string reason;
	int reasonChoice;
	bool validReasonChoice;
	do {
		cout << "\nSelect an Cancellation Reason\n";
		cout << "1. Customer Requested Cancellation\n";
		cout << "2. Changes of Reservation\n";
		cout << "3. Maintenance Issue\n";
		cout << "4. Emergency\n";
		cout << "5. Other\n";
		cin >> reasonChoice;
		cin.ignore();

		validReasonChoice = (reasonChoice >= 1 && reasonChoice <= 5);
		if (!validReasonChoice) {
			cout << "Invalid choice. Please enter a valid choice (between 1 to 5)\n";
		}
	} while (!validReasonChoice);

	switch (reasonChoice) {
	case 1:
		reason = "Customer requested cancellation";
		break;
	case 2:
		reason = "Changes of Reservation";
		break;
	case 3:
		reason = "Maintenance Issue";
		break;
	case 4:
		reason = "Emergency";
		break;
	case 5:
		cout << "Please enter the reason : ";
		getline(cin, reason);
		break;
	}

	// Cancellation Confirmation
	char confirm;

	cout << "\n----- Cancellation Confirmation -----\n";
	do {
		cout << "Confirm Cancellation ? (Y/N) : ";
		cin >> confirm;
		cin.ignore();

		if (confirm != 'Y' && confirm != 'N') {
			cout << "Invalid input. Please enter the valid input (Y/N).\n";
		}
	} while (confirm != 'Y' && confirm != 'N');

	if (confirm == 'N') {
		cout << "Cancellation aborted..." << endl;;
		return;
	}

	// update reservation status
	reservations[searchingID].reservationStatus = "Cancelled";
	reservations[searchingID].cancellationReason = reason;

	// cancellation success message
	cout << "Reservation " << reservationID << " has been cancelled..." << endl;
}

// save the reservation vector into reservations.txt
void saveReservation(string filename = "reservations.txt") {
	ofstream outFile(filename);

	for (int i = 0; i < (int)reservations.size(); i++) {
		Reservation record = reservations[i];
		// the pattern while the data saved inside the reservation.txt
		outFile << record.reservationID << "|"
			<< record.customerID << "|"
			<< record.roomNumber << "|"
			<< record.checkInDate << "|"
			<< record.checkOutDate << "|"
			<< record.actualCheckInTime << "|"
			<< record.actualCheckOutTime << "|"
			<< record.numberOfGuests << "|"
			<< record.numberOfNights << "|"
			<< record.roomPrice << "|"
			<< record.reservationStatus << "|"
			<< record.cancellationReason << "\n";
	}
	outFile.close();

	// success message
	cout << "Saved " << reservations.size() << " reservation(s) to file..." << endl;
}

// read the records in reservations.txt
void loadReservation(string filename = "reservations.txt") {
	ifstream inFile(filename);

	if (!inFile) {
		cout << "No existing reservation file found...\n";
		return;
	}

	//empty out the reservation vector
	reservations.clear();

	string line;

	while (getline(inFile, line)) {		// getline = to read the whole line the file
		stringstream ss(line);
		string field;
		Reservation record;

		getline(ss, record.reservationID, '|');
		getline(ss, record.customerID, '|');
		getline(ss, field, '|'); record.roomNumber = stoi(field);
		getline(ss, record.checkInDate, '|');
		getline(ss, record.checkOutDate, '|');
		getline(ss, record.actualCheckInTime, '|');
		getline(ss, record.actualCheckOutTime, '|');
		getline(ss, field, '|'); record.numberOfGuests = stoi(field);
		getline(ss, field, '|'); record.numberOfNights = stoi(field);
		getline(ss, field, '|'); record.roomPrice = stod(field);
		getline(ss, record.reservationStatus, '|');
		getline(ss, record.cancellationReason, '|');

		reservations.push_back(record);
	}
	inFile.close();
}

// Reservation module menu
void reservationMenu() {
	int choice;

	do {
		cout << "\n----------Reservation Menu ----------\n";
		cout << "1. Create Reservation\n";
		cout << "2. Customer Check-In\n";
		cout << "3. Customer Check-Out\n";
		cout << "4. Reservation Cancellation\n";
		cout << "5. Save Reservations to File\n";
		cout << "6. Exit\n";
		cout << "Enter choice : ";
		cin >> choice;
		cin.ignore();

		switch (choice) {
		case 1:
			createReservation();
			break;
		case 2:
			customerCheckin();
			break;
		case 3:
			customerCheckout();
			break;
		case 4:
			cancelReservation();
			break;
		case 5:
			saveReservation();
			break;
		case 6:
			cout << "Exiting Reservation Menu...\n";
			break;
		default:
			cout << "Invalid choice. Please enter a valid input (1-6).\n";
		}
	} while (choice != 6);
}
