#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <regex>
#include <limits> // get the min and max of data type can hold
#include <cctype> // transform the character lowercase and uppercase
#include "models.h"
#include "reservation.h"
#include "file_io.h"
#include "ui.h"

string getNonEmptyInput(const string& prompt);

void customerMembershipMenu();

void registerCustomer(vector<Customer>& customers);

bool searchCustomer(
	const vector<Customer>& customers,
	const vector<Membership>& memberships,
	const string& name
);

void updateCustomer(vector<Customer>& customers);

void deactivateCustomer(
	vector<Customer>& customers,
	const vector<Reservation>& reservation
);

void activateCustomer(vector<Customer>& customers);

void manageMembership(
	vector<Customer>& customers,
	vector<Membership>& membership
);

void viewBookingHistory(
	const string& customerName,
	const vector<Reservation>& reservation
);
/*
	pass in the customer id to get the customer obj
*/
Customer* get_customer_by_id(string customerID) {
	for (Customer& customer : customers) {
		if (customer.customerID == customerID)
		{
			return &customer;
		}
	}
	return nullptr;
}

/*
	pass in the customer id and check whether the customer is membership or not
*/
bool is_membership(string customerID) {

	for (Membership membership : memberships)
	{
		if (membership.customerID == customerID) {
			return true;
		}
	}
	return false;
}

// -------------------------------------- HELPER FUNCTION --------------------------------------
string toUpperText(string text) {
	for (char& ch : text) {
		ch = static_cast<char>(toupper(static_cast<unsigned char>(ch))); // if it are the Upper text return the text
	}
	return text;
}

bool isBlank(const string& text) {
	if (text.empty()) {
		return true;
	}
	for (char ch : text) {
		if (!isspace(static_cast<unsigned char>(ch))) { // if it are not space return false
			return false;
		}
	}
	return true;
}


bool isDigitsOnly(const string& text) {
	if (text.empty()) {
		return false;
	}

	for (char ch : text) {
		if (!isdigit(static_cast<unsigned char>(ch))) { // if it are not the digit retrun false
			return false;
		}
	}
	return true;
}

// In upper case to check gender
bool isValidGender(const string& gender) {
	string upper = toUpperText(gender);
	return upper == "MALE" || upper == "FEMALE" || upper == "M" || upper == "F"; // check wheter the input of user are upper case
}

string normalizeGender(const string& gender) {
	string upper = toUpperText(gender);
	// If M/MALE -> Male
	if (upper == "M" || upper == "MALE") {
		return "Male";
	}
	// If F/FEMALE -> female
	if (upper == "F" || upper == "FEMALE") {
		return "Female";
	}
	return gender;
}

bool isLeapYear(int year) {
	return (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0); // check the yeaar is as the 365days / 366 days
}

int getDaysInMonth(int month, int year) {
	switch (month) {
	case 1: return 31; // 31/1
	case 2: return isLeapYear(year) ? 29 : 28; // check the day of february is have 28/29 days use the LeapYear to check
	case 3: return 31;
	case 4: return 30;
	case 5: return 31;
	case 6: return 30;
	case 7: return 31;
	case 8: return 31;
	case 9: return 30;
	case 10: return 31;
	case 11: return 30;
	case 12: return 31;
	default: return 0;
	}
}

bool isValidDate(const string& date) {
	regex pattern(R"(^\d{2}/\d{2}/\d{4}$)"); // date format -> DD/MM/YYYY

	if (!regex_match(date, pattern)) {
		return false;
	}

	int day = stoi(date.substr(0, 2)); // first 2 char -> day
	int month = stoi(date.substr(3, 2)); // 3 and 4 index -> month
	int year = stoi(date.substr(6, 4)); // 6 - 9 index  -> years

	if (year < 1900 || month < 1 || month > 12) { // validation checking of the registration
		return false;
	}

	int maxDay = getDaysInMonth(month, year); // check the maximum day of each month
	return day >= 1 && day <= maxDay;
}

string getNonEmptyInput(const string& prompt) { // validation check wheter the input is not empty
	string input;

	do {
		cout << prompt;
		getline(cin, input);

		if (isBlank(input)) {
			cout << "Input cannot be empty." << endl;
		}
	} while (isBlank(input));

	return input;
}

bool isValidContact(const string& contact) {
	regex pattern(R"(\d{3}-\d{3}-\d{4})");
	return regex_match(contact, pattern);
}

string getValidatedContact(const string& prompt) { // validation check wheter the contact formart is correct
	string input;

	do {
		cout << prompt;
		getline(cin, input);

		if (!isValidContact(input)) {
			cout << "Invalid contact number. Please enter format of contact xxx-xxx-xxxx." << endl;
		}
	} while (!isValidContact(input));

	return input;
}

bool isValidICNumber(const string& ICNumber) {
	regex pattern(R"(\d{6}-\d{2}-\d{4})");
	return regex_match(ICNumber, pattern);
}

string getValidatedICNumber(const string& prompt) { // validation check wheter the ICNumber formart is correct
	string input;

	do {
		cout << prompt;
		getline(cin, input);

		if (!isValidICNumber(input)) {
			cout << "Invalid IC number. Please enter format of IC Number xxxxxx-xx-xxxx." << endl;
		}
	} while (!isValidICNumber(input));

	return input;
}


string getValidatedGender(const string& prompt) { // validation check the gender input
	string input;

	do {
		cout << prompt;
		getline(cin, input);

		if (!isValidGender(input)) {
			cout << "Invalid gender. Please enter Male/Female or M/F only." << endl;
		}
	} while (!isValidGender(input));

	return normalizeGender(input);
}

string getValidatedDate(const string& prompt) { // validation check the date format
	string input;

	do {
		cout << prompt;
		getline(cin, input);
		if (!isValidDate(input)) {
			cout << "Invalid date. Please use DD/MM/YYYY format." << endl;
		}
	} while (!isValidDate(input));

	return input;
}

string getCurrentDate() {
	time_t now = time(0);
	tm localTime;
	localtime_s(&localTime, &now); // get the date today

	char buffer[11];
	strftime(buffer, sizeof(buffer), "%d/%m/%Y", &localTime);

	return string(buffer);
}

string getEndDate() {
	string date = getCurrentDate();   // example: "12/01/2026"

	string day = date.substr(0, 2);  // "12"
	string month = date.substr(3, 2);  // "01"
	string year = date.substr(6, 4);  // "2026"

	int newYear = stoi(year) + 1;      // 2026 + 1 = 2027

	return day + "/" + month + "/" + to_string(newYear); // result: "12/01/2027"
}

int findCustomerIndexByID(const vector<Customer>& customer, const string& name) { // Find customer using the customerId 
	for (int i = 0; i < static_cast<int>(customers.size()); i++) {
		if (toUpperText(customers[i].name) == toUpperText(name)) {
			return i;
		}
	}
	return -1;
}
int findCustomerIndexByName(const vector<Customer>& customers, const string& name) {
	for (int i = 0; i < static_cast<int>(customers.size()); i++) {
		if (toUpperText(customers[i].name) == toUpperText(name)) {
			return i;
		}
	}
	return -1;
}

int findMembershipIndexByCustomerID(const vector<Membership>& memberships, const string& customerID) { // Find membership using the customerId
	for (int i = 0; i < static_cast<int>(memberships.size()); i++) {
		if (toUpperText(memberships[i].customerID) == toUpperText(customerID)) {
			return i;
		}
	}
	return -1;
}

int selectCustomerByname(const vector<Customer>& customers, const string& customerName) {
	vector<int> matchIndex;

	for (int i = 0; i < static_cast<int>(customers.size()); i++) {
		if (toUpperText(customers[i].name) == toUpperText(customerName)) {
			matchIndex.push_back(i);
		}
	}
	if (matchIndex.empty()) {
		return -1;
	}
	if (matchIndex.size() == 1) {
		return matchIndex[0];
	}
	cout << endl;
	cout << matchIndex.size() << " Customer found with name " << customerName << ":" << endl;

	for (size_t j = 0; j < matchIndex.size(); j++) {
		cout << "  [" << (j + 1) << "] " << customers[matchIndex[j]].customerID
			<< " - " << customers[matchIndex[j]].name << "  # " << customers[matchIndex[j]].contact << endl;
	}
	int choice;
	cout << "Which one do you want? [1 - " << matchIndex.size() << "]: ";

	while (!(cin >> choice) || choice < 1 || choice > static_cast<int>(matchIndex.size())) {
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Invalid input. Please enter a number between 1 and " << matchIndex.size() << ": ";
	}
	cin.ignore(1000, '\n');

	return matchIndex[choice - 1];
}


bool hasActiveReservation(const string& customerID, const vector<Reservation>& reservations) { // check the customer is active or not
	for (Reservation reservation : reservations) {
		string status = toUpperText(reservation.reservationStatus);

		if (toUpperText(reservation.customerID) == toUpperText(customerID) && status != "CANCELLED" && status != "CHECKED-OUT") {
			return true;
		}
	}
	return false;
}

// generate the customerID
string generateCustomerID(const vector<Customer>& customers) {
	int maxNumber = 0;

	for (Customer customer : customers) {
		string id = customer.customerID;

		if (id.length() >= 6 && id.substr(0, 3) == "CUS") {  // chechking if ID is valid
			string numberPart = id.substr(3); // Extract the number part

			if (isDigitsOnly(numberPart)) { // validation and convert to integer
				int number = stoi(numberPart);
				if (number > maxNumber) {
					maxNumber = number;
				}
			}
		}
	}

	int nextNumber = maxNumber + 1; // generate the next ID
	string newID = "CUS";

	if (nextNumber < 10) {
		newID += "00";
	}
	else if (nextNumber < 100) {
		newID += "0";
	}
	// For 100 and above, no padding needed

	newID += to_string(nextNumber); // Append the number and return
	return newID;
}

// generate the membershipID
string generateMembershipID(const vector<Membership>& memberships) {
	int maxNumber = 0;

	for (Membership membership : memberships) {
		string id = membership.membershipID;

		if (id.length() >= 6 && id.substr(0, 3) == "MEM") {  // chechking if ID is valid
			string numberPart = id.substr(3); // Extract the number part

			if (isDigitsOnly(numberPart)) { // validation and convert to integer
				int number = stoi(numberPart);
				if (number > maxNumber) {
					maxNumber = number;
				}
			}
		}
	}

	int nextNumber = maxNumber + 1; // generate the next ID
	string newID = "MEM";

	if (nextNumber < 10) {
		newID += "00";
	}
	else if (nextNumber < 100) {
		newID += "0";
	}
	// For 100 and above, no padding needed

	newID += to_string(nextNumber); // Append the number and return
	return newID;
}

string getMembershipLevel(int points) { // check the membership level using if else statement
	if (points >= 6000) {
		return "PLATINUM";
	}
	else if (points >= 3000) {
		return "GOLD";
	}
	else if (points >= 1000) {
		return "SILVER";
	}
	else {
		return "BRONZE";
	}
}

double getMembershipDiscount(const string& level) {
	string upper = toUpperText(level);

	if (upper == "PLATINUM") { // if membership are platinum discount 20%
		return 0.20;
	}
	else if (upper == "GOLD") { // if membersip are gold discount 15%
		return 0.15;
	}
	else if (upper == "SILVER") { // if membership are silver discount 10%
		return 0.10;
	}
	else {
		return 0.05; // if customer are just have membership not level discount 5%
	}
}

void updateMembershipTier(Membership& membership) { // update the membership level
	membership.level = getMembershipLevel(membership.points);
	membership.discountRate = getMembershipDiscount(membership.level);
}

void displayCustomerDetails(const Customer& customer) {
	cout << endl;
	print_header("Customer Details");
	cout << " Customer ID    : " << customer.customerID << endl;
	cout << " Name           : " << customer.name << endl;
	cout << " Contact        : " << customer.contact << endl;
	cout << " Gender         : " << customer.gender << endl;
	cout << " IC Number      : " << customer.icNumber << endl;
	cout << " Nationality    : " << customer.nationality << endl;
	cout << " Birthday       : " << customer.birthday << endl;
	cout << " Register Date  : " << customer.registerDate << endl;
	cout << " Account Status : " << customer.accountStatus << endl;
}

void displayMembershipDetails(const Membership& membership) {
	cout << endl;
	print_header("Membership Details");
	cout << " Membership ID  : " << membership.membershipID << endl;
	cout << " Customer ID    : " << membership.customerID << endl;
	cout << " Level          : " << membership.level << endl;
	cout << " Register Date  : " << membership.registerDate << endl;
	cout << " Expiry Date    : " << membership.expiryDate << endl;
	cout << " Points         : " << membership.points << endl;
	cout << " Discount Rate  : " << fixed << setprecision(2) << membership.discountRate * 100 << "%" << endl;
	cout << " Status         : " << membership.status << endl;
}

// -------------------------------------- CUSTOMER FUNCTION --------------------------------------
// Register new customer
void registerCustomer(vector<Customer>& customers) {
	clear_screen();
	print_header("Register Customer");

	Customer customer;
	customer.customerID = generateCustomerID(customers);
	customer.registerDate = getCurrentDate();
	empty_line();

	cout << left << setw(53) << ("| Customer ID: " + customer.customerID)
		<< "Register Date: " << customer.registerDate << setw(80 - 78) << right << "|" << endl;

	print_divider_with_space(false);

	customer.name = getNonEmptyInput("Enter Customer Name  : ");
	customer.contact = getValidatedContact("Enter Contact Number : ");
	customer.gender = getValidatedGender("Enter Gender         : ");
	customer.icNumber = getValidatedICNumber("Enter IC Number      : ");
	customer.nationality = getNonEmptyInput("Enter Nationality    : ");
	customer.birthday = getValidatedDate("Enter Birthday       : ");
	customer.accountStatus = "ACTIVE";

	customers.push_back(customer);
	save_customers_to_file(); // save the data into the customer file

	cout << "Customer registered sucessfully!!" << endl;
	clear_screen();
	displayCustomerDetails(customer);
	system("pause");
	clear_screen();
}

// Search Customer
bool searchCustomer(const vector<Customer>& customers, const vector<Membership>& memberships, const string& name) {
	clear_screen();
	print_header("Search Customer");
	cout << endl;

	bool found = false;

	for (int i = 0; i < static_cast<int>(customers.size()); i++) {
		if (toUpperText(customers[i].name) == toUpperText(name)) {
			found = true;
			displayCustomerDetails(customers[i]);

			int membershipIndex = findMembershipIndexByCustomerID(memberships, customers[i].customerID);
			if (membershipIndex != -1) {
				displayMembershipDetails(memberships[membershipIndex]);
			}
			else {
				cout << "No membership record found." << endl;
			}
			cout << endl;
		}
	}
	if (!found) {
		cout << "Customer not found." << endl;
	}
	system("pause");
	clear_screen();
	return found;
}

// Update Customer
void updateCustomer(vector<Customer>& customers) {
	clear_screen();
	print_header("Update Customer");
	cout << endl;

	string customerName = getNonEmptyInput("Enter Customer Name to Update [0 to back] : ");
	if (toUpperText(customerName) == "0") {
		return;
	}

	int index = selectCustomerByname(customers, customerName);

	if (index == -1) {
		cout << "Customer not found." << endl;
		system("pause");
		clear_screen();
		return;
	}
	if (toUpperText(customers[index].accountStatus) != "ACTIVE") {
		cout << "Only ACTIVE Customer can be updated." << endl;
		system("pause");
		clear_screen();
		return;
	}
	int choice;

	do {
		clear_screen();
		print_header("Update Customer");
		displayCustomerDetails(customers[index]);

		cout << "--------------------------------------------------------------------------------" << endl;
		cout << "|  [1] Update Name" << setw(80 - 18) << right << "|" << endl;
		cout << "|  [2] Update Contact" << setw(80 - 21) << right << "|" << endl;
		cout << "|  [3] Update Gender" << setw(80 - 20) << right << "|" << endl;
		cout << "|  [4] Update IC Number" << setw(80 - 23) << right << "|" << endl;
		cout << "|  [5] Update Nationality" << setw(80 - 25) << right << "|" << endl;
		cout << "|  [6] Update Birthday" << setw(80 - 22) << right << "|" << endl;
		cout << "|  [7] update Register Date" << setw(80 - 27) << right << "|" << endl;
		empty_line();
		cout << "|  [0] Back" << setw(80 - 11) << right << "|" << endl;
		print_divider();

		choice = get_menu_choice(7);

		switch (choice) {
		case 1:
			customers[index].name = getNonEmptyInput("Enter New Name          : ");
			save_customers_to_file();
			cout << "Name updated successfully!!" << endl;
			system("pause");
			break;

		case 2:
			customers[index].contact = getValidatedContact("Enter New Contact       : ");
			save_customers_to_file();
			cout << "Contact updated successfully!!" << endl;
			system("pause");
			break;

		case 3:
			customers[index].gender = getValidatedGender("Enter New Gender        : ");
			save_customers_to_file();
			cout << "Gender updated successfully!!" << endl;
			system("pause");
			break;

		case 4:
			customers[index].icNumber = getNonEmptyInput("Enter New IC Number     : ");
			save_customers_to_file();
			cout << "IC Number updated successfully!!" << endl;
			system("pause");
			break;

		case 5:
			customers[index].nationality = getNonEmptyInput("Enter New Nationality   : ");
			save_customers_to_file();
			cout << "Nationality updated successfully!!" << endl;
			system("pause");
			break;

		case 6:
			customers[index].birthday = getValidatedDate("Enter New Birthday      : ");
			save_customers_to_file();
			cout << "Birthday updated successfully!!" << endl;
			system("pause");
			break;

		case 7:
			customers[index].registerDate = getValidatedDate("Enter New Register Date : ");
			save_customers_to_file();
			cout << "Register Date updated successfully!!" << endl;
			system("pause");
			break;

		default:
			clear_screen();
			break;
		}
	} while (choice != 0);
}
// Activate Customer
void activateCustomer(vector<Customer>& customers) {
	clear_screen();
	print_header("Activate Customer");
	cout << endl;

	string customerName= getNonEmptyInput("Enter Customer Name to activate [0 to back] : ");
	if (toUpperText(customerName) == "0") {
		return;
	}

	int index = selectCustomerByname(customers, customerName);

	if (index == -1) {
		cout << "Customer not found." << endl;
		system("pause");
		clear_screen();
		return;
	}

	if (toUpperText(customers[index].accountStatus) == "ACTIVE") {
		cout << "Customer is already active." << endl;
		system("pause");
		clear_screen();
		return;
	}

	string confirm;
	do {
		cout << "Confirm activate customer? (Y/N) : ";
		getline(cin, confirm);
		confirm = toUpperText(confirm);

		if (confirm != "Y" && confirm != "N") {
			cout << "Invalid input. Please enter Y or N only." << endl;
		}
	} while (confirm != "Y" && confirm != "N");

	if (confirm == "Y") {
		customers[index].accountStatus = "ACTIVE";
		save_customers_to_file();
		cout << "Customer activated successfully!!" << endl;
	}
	else {
		cout << "Activation cancelled." << endl;
	}
	displayCustomerDetails(customers[index]);
	system("pause");
	clear_screen();
}

// Deactivate Customer
void deactivateCustomer(vector<Customer>& customers, const vector<Reservation>& reservation) {
	clear_screen();
	print_header("Deactivate Customer");
	cout << endl;
	string customerName = getNonEmptyInput("Enter Customer Name to deactivate [0 to back] : ");
	if (toUpperText(customerName) == "0") {
		return;
	}

	int index = selectCustomerByname(customers, customerName);

	if (index == -1) {
		cout << "Customer not found." << endl;
		system("pause");
		clear_screen();
		return;
	}
	if (toUpperText(customers[index].accountStatus) == "INACTIVE") {
		cout << "Customer is already inactive." << endl;
		system("pause");
		clear_screen();
		return;
	}
	if (hasActiveReservation(customers[index].customerID, reservations)) {	
		cout << "Customer cannot be deactiveted because there is an active reservation." << endl;
		system("pause");
		clear_screen();
		return;
	}

	string confirm;
	do {
		cout << "Confirm deactivate customer? (Y/N) : ";
		getline(cin, confirm);
		confirm = toUpperText(confirm);

		if (confirm != "Y" && confirm != "N") {
			cout << "Invalid input. Please enter Y or N only." << endl;
		}
	} while (confirm != "Y" && confirm != "N");

	if (confirm == "Y") {
		customers[index].accountStatus = "INACTIVE";
		save_customers_to_file();
		cout << "Customer deactivated successfully!!" << endl;
	}
	else {
		cout << "Deactivation cancelled." << endl;
	}

	displayCustomerDetails(customers[index]);
	system("pause");
	clear_screen();

}

// View Customer Booking History
void viewBookingHistory(const string& customerName, const vector<Reservation>& reservation) {
	clear_screen();
	print_header("View Booking History");
	cout << endl;


	int customerIndex = selectCustomerByname(customers, customerName);

	if (customerIndex == -1) {
		cout << "Customer not found." << endl;
		system("pause");
		return;
	}

	string customerID = customers[customerIndex].customerID;
	bool found = false;

	cout << endl;
	cout << "Customer ID   : " << customerID << endl;
	cout << "Customer Name : " << customers[customerIndex].name << endl;

	cout << setw(15) << left << "Reserve ID"
		<< setw(12) << left << "Room No"
		<< setw(15) << left << "Check In"
		<< setw(15) << left << "Check Out"
		<< setw(10) << left << "Nights"
		<< setw(15) << left << "Status"
		<< setw(12) << left << "Amount" << endl;

	print_divider();

	for (Reservation reservation : reservations) {
		if (toUpperText(reservation.customerID) == toUpperText(customerID)) {
			double amount = reservation.roomPrice * reservation.numberOfNights;

			cout << setw(15) << left << reservation.reservationID // format of the view booking history
				<< setw(12) << left << reservation.roomNumber
				<< setw(15) << left << reservation.checkInDate
				<< setw(15) << left << reservation.checkOutDate
				<< setw(10) << left << reservation.numberOfNights
				<< setw(15) << left << reservation.reservationStatus
				<< setw(12) << left << fixed << setprecision(2) << amount << endl;

			found = true;
		}
	}
	if (!found) {
		cout << "No booking history found for this customer." << endl;
	}

	cout << endl;
	system("pause");
	clear_screen();
}

// -------------------------------------- MEMBERSHIP FUNCTION --------------------------------------
// Manage the Membership
void manageMembership(vector<Customer>& customers, vector<Membership>& membership) {
	int choice;

	do {
		clear_screen();
		print_header("Manage Membership");
		empty_line();
		cout << "|  [1] Register Membership" << setw(80 - 26) << right << "|" << endl;
		cout << "|  [2] View Membership Details" << setw(80 - 30) << right << "|" << endl;
		cout << "|  [3] Add Membership Points" << setw(80 - 28) << right << "|" << endl;
		cout << "|  [4] Redeem Membership Points" << setw(80 - 31) << right << "|" << endl;
		cout << "|  [5] Activate Membership" << setw(80 - 26) << right << "|" << endl;
		cout << "|  [6] Deactivate Membership" << setw(80 - 28) << right << "|" << endl;
		empty_line();
		cout << "|  [0] Back" << setw(80 - 11) << right << "|" << endl;
		print_divider_with_space(false);

		choice = get_menu_choice(6);

		switch (choice) {
			// Register Membership
		case 1: {
			clear_screen();
			print_header("Register Membership");

			Membership membership;
			membership.membershipID = generateMembershipID(memberships);
			membership.registerDate = getCurrentDate();
			membership.expiryDate = getEndDate();
			membership.level = "BRONZE";
			membership.points = 0;
			membership.discountRate = 0.05;
			membership.status = "ACTIVE";

			empty_line();

			// ✅ Display Membership ID and Register Date aligned like Customer ID
			cout << left << setw(53) << ("| Membership ID: " + membership.membershipID)
				<< "Register Date: " << membership.registerDate
				<< setw(80 - 78) << right << "|" << endl;
			print_divider_with_space(false);

			string customerName = getNonEmptyInput("Enter Customer Name : ");
			int customerIndex = selectCustomerByname(customers, customerName);
			
			print_divider_with_space(false);

			customerName = getNonEmptyInput("Enter Customer Name : ");

			if (customerIndex == -1) {
				cout << "Customer not found." << endl;
				system("pause");
				break;
			}

			if (toUpperText(customers[customerIndex].accountStatus) != "ACTIVE") { // If the customer is INACTIVe cannot be register
				cout << "Only ACTIVE customers can register membership." << endl;
				system("pause");
				break;
			}

			int membershipIndex = findMembershipIndexByCustomerID(memberships, customerName);

			if (membershipIndex != -1 && toUpperText(memberships[membershipIndex].status) == "ACTIVE") { // If the customer is the membersihp cannot be register
				cout << "This customer already has an active membership." << endl;
				system("pause");
				break;
			}

			if (membershipIndex == -1) {
				Membership membership;
				membership.membershipID = generateMembershipID(memberships);
				membership.customerID = customers[customerIndex].customerID;
				membership.level = "BRONZE"; // The initial membership level
				membership.registerDate = getCurrentDate();
				cout << "Register Date     : " << membership.registerDate << endl;
				membership.expiryDate = getEndDate();
				cout << "Expiry Date       : " << membership.expiryDate << endl;
				membership.points = 0;
				membership.discountRate = 0.05;
				membership.status = "ACTIVE"; // The status of customer at membership will be active

				memberships.push_back(membership);
				save_memberships_to_file();

				cout << "Membership registered successfully!!" << endl;
				displayMembershipDetails(membership);
			}
			else {
				memberships[membershipIndex].registerDate = getValidatedDate("Enter Membership Register Date : ");
				memberships[membershipIndex].expiryDate = getValidatedDate("Enter Membership Expiry Date   : ");
				memberships[membershipIndex].status = "ACTIVE";
				updateMembershipTier(memberships[membershipIndex]);
				save_memberships_to_file();

				cout << "Membership activated successfully!!" << endl;
				displayMembershipDetails(memberships[membershipIndex]);
			}
			system("pause");
			clear_screen();
			break;
		}

			  // View Membership Details
		case 2: {
			clear_screen();
			print_header("View Membership Details");
			cout << endl;

			string customerName= getNonEmptyInput("Enter Customer Name : ");
			int customerIndex = selectCustomerByname(customers, customerName);

			if (customerIndex == -1) {
				cout << "Customer not found." << endl;
				system("pause");
				break;
			}
			int index = findMembershipIndexByCustomerID(memberships, customers[customerIndex].customerID);
			if (index == -1) {
				cout << "Membership record not found." << endl;
			}
			else {
				displayMembershipDetails(memberships[index]);
			}

			system("pause");
			clear_screen();
			break;
		}

		// Add Membership points
		case 3: {
			clear_screen();
			print_header("Add Membership Points");
			cout << endl;

			string customerName= getNonEmptyInput("Enter Customer Name [0 to back] : ");
			if (toUpperText(customerName) == "0") {
				break;    
			}
			int customerIndex = selectCustomerByname(customers, customerName);

			if (customerIndex == -1) {
				cout << "Customer not found." << endl;
				system("pause");
				break;
			}
			int index = findMembershipIndexByCustomerID(memberships, customers[customerIndex].customerID);

			if (index == -1) {
				cout << "Membership record not found." << endl;
				system("pause");
				clear_screen();
				break;
			}

			if (toUpperText(memberships[index].status) != "ACTIVE") {
				cout << "Membership is not active." << endl;
				system("pause");
				clear_screen();
				break;
			}

			int pointsToAdd;
			cout << "Enter Points to Add : ";

			while (!(cin >> pointsToAdd) || pointsToAdd <= 0) {
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "Invalid input. Please enter a positive number : ";
			}
			cin.ignore(1000, '\n');

			memberships[index].points += pointsToAdd;
			updateMembershipTier(memberships[index]);
			save_memberships_to_file();

			cout << "Points added successfully!!" << endl;
			displayMembershipDetails(memberships[index]);
			system("pause");
			clear_screen();
			break;
		}

		// Redeem Membership Point
		case 4: {
			clear_screen();
			print_header("Redeem Membership Points");
			cout << endl;

			string customerName= getNonEmptyInput("Enter Customer Name [0 to back] : ");
			if (toUpperText(customerName) == "0") {
				break;
			}
			int customerIndex = selectCustomerByname(customers, customerName);

			if (customerIndex == -1) {
				cout << "Customer not found." << endl;
				system("pause");
				break;
			}

			int index = findMembershipIndexByCustomerID(memberships, customers[customerIndex].customerID);
			if (index == -1) {
				cout << "Membership record not found." << endl;
				system("pause");
				clear_screen();
				break;
			}

			if (toUpperText(memberships[index].status) != "ACTIVE") {
				cout << "Membership is not active." << endl;
				system("pause");
				clear_screen();
				break;
			}

			int pointsToRedeem;
			cout << "Enter Points to Redeem : ";

			while (!(cin >> pointsToRedeem) || pointsToRedeem <= 0) {
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "Invalid input. Please enter a positive number : ";
			}
			cin.ignore(1000, '\n');

			if (pointsToRedeem > memberships[index].points) {
				cout << "Not enough points to redeem." << endl;
				system("pause");
				clear_screen();
				break;
			}

			memberships[index].points -= pointsToRedeem;
			updateMembershipTier(memberships[index]);
			save_memberships_to_file();

			cout << "Points redeemed successfully!!" << endl;
			displayMembershipDetails(memberships[index]);
			system("pause");
			break;
		}

		// Activate Membership
		case 5: {
			clear_screen();
			print_header("Activate Membership");
			cout << endl;

			string customerName = getNonEmptyInput("Enter customer Name [0 to back] : ");
			if (toUpperText(customerName) == "0") {
				break;
			}
			int customerIndex = selectCustomerByname(customers, customerName);

			if (customerIndex == -1) {
				cout << "Customer not found." << endl;
				system("pause");
				break;
			}

			int index = findMembershipIndexByCustomerID(memberships, customers[customerIndex].customerID);

			if (index == -1) {
				cout << "Membership record not found." << endl;
				system("pause");
				clear_screen();
				break;
			}

			if (toUpperText(memberships[index].status) == "ACTIVE") {
				cout << "Membership is already active." << endl;
				system("pause");
				clear_screen();
				break;
			}

			string confirm;
			do {
				cout << "Confirm activate membership? (Y/N) : ";
				getline(cin, confirm);
				confirm = toUpperText(confirm);

				if (confirm != "Y" && confirm != "N") {
					cout << "Invalid input. Please enter Y or N only." << endl;
				}
			} while (confirm != "Y" && confirm != "N");

			if (confirm == "Y") {
				memberships[index].status = "ACTIVE";
				save_memberships_to_file();
				cout << "Membership activated successfully!!" << endl;
			}
			else {
				cout << "activation cancelled." << endl;
			}
			displayMembershipDetails(memberships[index]);
			system("pause");
			clear_screen();
			break;
		}

		// Deactivate Membership
		case 6: {
			clear_screen();
			print_header("Deactivate Membership");
			cout << endl;

			string customerName = getNonEmptyInput("Enter Customer Name [0 to back] : ");
			if (toUpperText(customerName) == "0") {
				break;
			}
			int customerIndex = selectCustomerByname(customers, customerName);

			if (customerIndex == -1) {
				cout << "Customer not found." << endl;
				system("pause");
				break;
			}

			int index = findMembershipIndexByCustomerID(memberships, customers[customerIndex].customerID);

			if (index == -1) {
				cout << "Membership record not found." << endl;
				system("pause");
				clear_screen();
				break;
			}

			if (toUpperText(memberships[index].status) == "INACTIVE") {
				cout << "Membership is already inactive." << endl;
				system("pause");
				clear_screen();
				break;
			}

			string confirm;
			do {
				cout << "Confirm deactivate membership? (Y/N) : ";
				getline(cin, confirm);
				confirm = toUpperText(confirm);

				if (confirm != "Y" && confirm != "N") {
					cout << "Invalid input. Please enter Y or N only." << endl;
				}
			} 
			while (confirm != "Y" && confirm != "N");

			if (confirm == "Y") {
				memberships[index].status = "INACTIVE";
				save_memberships_to_file();
				cout << "Membership deactivated successfully!!" << endl;
			}

			else {
				cout << "Deactivation cancelled." << endl;
			}

			displayMembershipDetails(memberships[index]);
			system("pause");
			clear_screen();
			break;
		}

		default:
			clear_screen();
			break;

		}
	} while (choice != 0);
}

// -------------------------------------- MAIN MENU --------------------------------------
void customerMembershipMenu() {
	int choice;

	do {
		print_header(" CUSTOMER & MEMBERSHIP MANAGEMENT MENU");
		empty_line();
		cout << "|  [1] Register Customer" << setw(80 - 24) << right << "|" << endl;
		cout << "|  [2] Search Customer" << setw(80 - 22) << right << "|" << endl;
		cout << "|  [3] Update Customer" << setw(80 - 22) << right << "|" << endl;
		cout << "|  [4] Activate Customer" << setw(80 - 24) << right << "|" << endl;
		cout << "|  [5] Deactivate Customer" << setw(80 - 26) << right << "|" << endl;
		cout << "|  [6] Manage Membership" << setw(80 - 24) << right << "|" << endl;
		cout << "|  [7] View Booking History" << setw(80 - 27) << right << "|" << endl;
		empty_line();
		cout << "|  [0] Back" << setw(80 - 11) << right << "|" << endl;
		print_divider_with_space(false);
		choice = get_menu_choice(7);


		switch (choice) {
		case 1:
			registerCustomer(customers);
			break;

		case 2: {
			string customerName = getNonEmptyInput("Enter Customer Name [0 to back] : ");
			if (toUpperText(customerName) == "0") {
				break;
			}

			searchCustomer(customers, memberships, customerName);
			break;
		}

		case 3:
			updateCustomer(customers);
			break;

		case 4:
			activateCustomer(customers);
			break;

		case 5:
			deactivateCustomer(customers, reservations);
			break;

		case 6:
			manageMembership(customers, memberships);
			break;

		case 7: {
			string customerName= getNonEmptyInput("Enter Customer Name to view the booking history [0 to back] : ");
			if (toUpperText(customerName) == "0") {
				break;
			}
			
			viewBookingHistory(customerName, reservations);
			break;
		}

		case 0:
			cout << "Returning to main menu...";
			break;

		default:
			cout << "Invalid Choice.";
		}

	} while (choice != 0);
}