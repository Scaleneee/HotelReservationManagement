#pragma once
#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <regex>
#include <ctime>     // needed for getting the current date
#include "models.h"
#include "payment.h"
#include "ui.h"

using namespace std;

/*
	return deposit
*/
double calculate_deposit(double roomFee) {
	return roomFee * DEPOSIT_RATE;
}

void clearScreen() {
	system("cls");
}

void create_unpaid_payment(const Reservation& reservation)
{
	Payment payment{};

	// generate payment ID
	int id = payments.size() + 1;

	string number = to_string(id);

	while (number.length() < 3)
	{
		number = "0" + number;
	}

	payment.paymentID = "PAY" + number;

	// link payment with reservation
	payment.reservationID = reservation.reservationID;

	// unpaid payment information
	payment.paymentDate = "";
	payment.paymentMethod = "";
	payment.paymentStatus = "Unpaid";

	// reservation.roomPrice already contains total room fee
	payment.roomFee = reservation.roomPrice;

	// initially no discount
	payment.membershipDiscount = 0.0;

	// check customer membership
	for (Membership& membership : memberships)
	{
		if (membership.customerID == reservation.customerID &&
			membership.status == "Active")
		{
			payment.membershipDiscount =
				payment.roomFee * membership.discountRate;

			break;
		}
	}

	// calculate deposit
	payment.depositAmount =
		calculate_deposit(payment.roomFee);

	// initially no extra charges
	payment.additionalCharge = 0.0;
	payment.damageCharge = 0.0;

	// calculate total amount
	payment.totalAmount =
		payment.roomFee
		+ payment.depositAmount
		- payment.membershipDiscount;

	// not paid yet
	payment.amountPaid = 0.0;
	payment.change = 0.0;

	// no refund / deposit settlement yet
	payment.refundAmount = 0.0;
	payment.depositReturned = 0.0;
	payment.depositRetained = 0.0;

	// add payment into vector
	payments.push_back(payment);

	// save payment data
	save_payments_to_file();
}

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

// get current date
void getCurrentDate(int& day, int& month, int& year) {
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);   // Windows-safe version of localtime

	day = ltm.tm_mday;
	month = 1 + ltm.tm_mon;    // tm_mon is 0-11, so add 1
	year = 1900 + ltm.tm_year; // tm_year is years since 1900
}

// search the payments vector
Payment* reservation_find_payment_by_id(string reservationID) {
	for (Payment& payment : payments) {
		if (payment.reservationID == reservationID) {
			return &payment;
		}
	}
	return nullptr;
}

// Found Valid Customer
bool isValidCustomer(string name) {
	for (int i = 0; i < (int)customers.size(); i++) {
		if (customers[i].name == name) {
			return true;
		}
	}
	return false;
}

// convert reservation text to uppercase
string reservationToUppercase(string text) {
	for (char& ch : text) {
		ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
	}
	return text;
}

// find customer using name
string findCustomer(string customerName) {
	vector<int> matchIndex;

	for (int i = 0; i < (int)customers.size(); i++) {
		if (reservationToUppercase(customers[i].name) == reservationToUppercase(customerName)) {
			matchIndex.push_back(i);
		}
	}

	if (matchIndex.empty()) {
		return "";
	}

	if (matchIndex.size() == 1) {
		return customers[matchIndex[0]].customerID;
	}

	// if find same name
	cout << "\n" << matchIndex.size() << " customers found with name \"" << customerName << "\":\n";
	for (int j = 0; j < (int)matchIndex.size(); j++) {
		cout << "  " << customers[matchIndex[j]].customerID
			 << " - " << customers[matchIndex[j]].name
			 << "  (" << customers[matchIndex[j]].contact << ")\n";
	}

	string enterID;
	bool matchedID = false;

	do {
		cout << "Enter the Customer ID from the list above : ";
		getline(cin, enterID);

		for (int j = 0; j < (int)matchIndex.size(); j++) {
			if (customers[matchIndex[j]].customerID == enterID) {
				matchedID = true;
				break;
			}
		}

		if (!matchedID) {
			cout << "Invalid Customer ID... Please try again.\n";
		}
	} while (!matchedID);

	return enterID;
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
	print_header("(Booking Confirmation)");
	cout << left << setw(18) << "Reservation ID" << ": " << reservation.reservationID << endl;
	cout << left << setw(18) << "Customer ID" << ": " << reservation.customerID << endl;
	cout << left << setw(18) << "Room Number" << ": " << reservation.roomNumber << endl;
	cout << left << setw(18) << "Check-In Date" << ": " << reservation.checkInDate << endl;
	cout << left << setw(18) << "Check-Out Date" << ": " << reservation.checkOutDate << endl;
	cout << left << setw(18) << "No. of Customer" << ": " << reservation.numberOfGuests << endl;
	cout << left << setw(18) << "Stay Duration" << ": " << reservation.numberOfNights << " night(s)" << endl;
	cout << left << setw(18) << "Room Price" << ": RM " << fixed << setprecision(2) << reservation.roomPrice << endl;
	cout << left << setw(18) << "Room Status" << ": " << reservation.reservationStatus << endl;
	cout << "-------------------------------\n";
}

// Create new reservation
void createReservation() {
	Reservation newReservation;
	clearScreen();
	print_header("New Reservation");

	// Find Customer using Customer ID
	bool validCustomer;
	do {
		cout << "Enter Customer's Name :   ";
		getline(cin, newReservation.name);

		validCustomer = isValidCustomer(newReservation.name);
		if (!validCustomer) {
			cout << "Customer not found, Please try again." << endl;
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

	save_reservations_to_file();

	// create unpaid payment
	create_unpaid_payment(newReservation);

	//create reservation success message
	cout << "\nReservation created successfully!\n";
	clearScreen();

	cout << endl;
	// Print Booking Confirmation
	bookingConfirmation(newReservation);
	cout << "\nPress [ENTER] to continue..." << endl;
	cin.get();

	clearScreen();
}

// Reservation List
void printReservationList(string statusFilter1, string statusFilter2 = "") {
	// header
	string header = "Reservation " + statusFilter1;
	if (statusFilter2 != "") {
		header += " / " + statusFilter2;
	}

	print_header(header);

	cout << left << setw(10) << "ID" << setw(12) << "CustomerID"
		<< setw(8) << "Room" << setw(14) << "CheckIn"
		<< setw(14) << "CheckOut" << setw(12) << "Status" << "\n";

	bool found = false;
	for (int i = 0; i < (int)reservations.size(); i++) {
		Reservation record = reservations[i];
		// che reservation status
		bool match = (record.reservationStatus == statusFilter1) ||
			(statusFilter2 != "" && record.reservationStatus == statusFilter2);

		if (match) {
			found = true;
			cout << left << setw(10) << record.reservationID << setw(12) << record.customerID
				<< setw(8) << record.roomNumber << setw(14) << record.checkInDate
				<< setw(14) << record.checkOutDate << setw(12) << record.reservationStatus << "\n";
		}
	}
	if (!found) {
		cout << "(No matching reservation found)\n\n";
	}
}

// Customer check in
void customerCheckin() {
	clearScreen();
	string reservationID;
	print_header("Customer Check-In");
	printReservationList("Booked");

	cout << "Enter Reservation  : ";
	getline(cin, reservationID);

	reservationID = reservationToUppercase(reservationID);

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
	save_reservations_to_file();


	// customer check in success message
	cout << "Customer checked in successfully for reservation " << reservationID << "\n";
	cout << "\nPress [ENTER] to continue..." << endl;
	cin.get();

	clearScreen();
}

// Customer check out
void customerCheckout() {
	clearScreen();

	string reservationID;
	print_header("Customer Check-Out");
	printReservationList("CheckedIn");

	cout << "Enter Reservation ID : ";
	getline(cin, reservationID);

	reservationID = reservationToUppercase(reservationID);

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
	save_reservations_to_file();

	// automatic late check-out charge
	int checkoutDay, checkoutMonth, checkoutYear;
	parseDate(reservations[searchingID].checkOutDate, checkoutDay, checkoutMonth, checkoutYear);
	int scheduledCheckoutDayCount = dayCount(checkoutDay, checkoutMonth, checkoutYear);

	int todayDay, todayMonth, todayYear;
	getCurrentDate(todayDay, todayMonth, todayYear);
	int todayDayCount = dayCount(todayDay, todayMonth, todayYear);

	if (todayDayCount > scheduledCheckoutDayCount) {
		Payment* payment = reservation_find_payment_by_id(reservationID);

		if (payment != nullptr) {
			payment->additionalCharge += 50;
			payment->totalAmount += 50;
			save_payments_to_file();

			cout << "\nNote: Late check-out detected." << endl;
			cout << "An additional charge of RM 50.00 has been added to this reservation's payment.\n";
		}
	}

	// customer check out success message
	cout << "Customer checked out successfully for reservation " << reservationID << "\n";
	cout << "\nPress [ENTER] to continue..." << endl;
	cin.get();

	clearScreen();
}

// Cancellation of reservation
void cancelReservation() {
	clearScreen();

	string reservationID;
	print_header("Reservation Cancellation");
	printReservationList("Booked", "CheckedIn");
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
		clearScreen();

		print_header("Select an Cancellation Reason");
		empty_line();
		cout << "|  [1] Customer Requested Cancellation" << setw(80 - 38) << right << "|" << endl;
		cout << "|  [2] Changes of Reservation" << setw(80 - 29) << right << "|" << endl;
		cout << "|  [3] Maintenance Issue" << setw(80 - 24) << right << "|" << endl;
		cout << "|  [4] Emergency" << setw(80 - 16) << right << "|" << endl;
		empty_line();
		cout << "|  [5] Other" << setw(80 - 12) << right << "|" << endl;
		
		print_divider_with_space(false);
		reasonChoice = get_menu_choice(4);

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
		confirm = toupper(confirm);

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
	save_reservations_to_file();

	// cancellation success message
	cout << "Reservation " << reservationID << " has been cancelled..." << endl;
	cout << "\nPress [ENTER] to continue..." << endl;
	cin.get();

	clearScreen();
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
		print_header("RESERVATION MENU");
		empty_line();
		cout << "|  [1] Create Reservation" << setw(80 - 25) << right << "|" << endl;
		cout << "|  [2] Customer Check-In" << setw(80 - 24) << right << "|" << endl;
		cout << "|  [3] Customer Check-Out" << setw(80 - 25) << right << "|" << endl;
		cout << "|  [4] Reservation Cancellation" << setw(80 - 31) << right << "|" << endl;
		
		empty_line();
		cout << "|  [0] Back" << setw(80 - 11) << right << "|" << endl;
		print_divider_with_space(false);
		choice = get_menu_choice(4);

		switch (choice) {
		case 1:
			createReservation();
			clear_screen();
			break;
		case 2:
			customerCheckin();
			clear_screen();
			break;
		case 3:
			customerCheckout();
			clear_screen();
			break;
		case 4:
			cancelReservation();
			clear_screen();
			break;
		case 0:
			cout << "Exiting Reservation Menu...\n";
			clear_screen();
			return;
		default:
			cout << "Invalid choice. Please enter a valid input (1-6).\n";
		}
	} while (choice != 6);

}
