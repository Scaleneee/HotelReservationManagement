#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <format>
#include "models.h"
#include "reservation.h"
#include "file_io.h"
#include "ui.h"
#include "customer.h"

using namespace std;

// --------------- LOGIC FUNCTIONS ---------------
/*
	return deposit
*/
double calculate_deposit(double roomFee) {
	return roomFee * DEPOSIT_RATE;
}

/*
	return the total amount of the payment
*/
double calculate_total_amount(
	double roomFee,
	double discount,
	double additionalCharge
) {
	return (roomFee + additionalCharge + calculate_deposit(roomFee)) - discount;
}

void create_unpaid_payment(Reservation reservation) {
	// declare a new payment obj 
	Payment payment;

	// initialize
	payment.paymentID = ""; // ltr use payment id generator to generate
	payment.reservationID = reservation.reservationID;
	// set to unpaid first
	payment.paymentStatus = "UNPAID";
	// calculate room fee
	payment.roomFee = reservation.roomPrice * reservation.numberOfNights;
	// calculate total amount
	payment.totalAmount = calculate_total_amount(payment.roomFee, 0.0, 0.0);

	// store into list
	payments.push_back(payment);

	// update to file
	save_payments_to_file();
}

bool check_reservation_id_format(string reservationID) {
	regex pattern(R"(^RES[0-9]{3}$)");
	return regex_match(reservationID, pattern);
}

/*
	pass in the reservationID and get the payment obj belongs to
*/
Payment* get_payment_by_reservation_id(string reservationID) {
	for (Payment& payment : payments) {
		if (payment.reservationID == reservationID)
		{
			return &payment;
		}
	}
	return nullptr;
}

Payment* get_payment_by_id(string paymentID) {
	for (Payment& payment : payments)
	{
		if (payment.paymentID == paymentID)
		{
			return &payment;
		}
	}
	return nullptr;
}


// --------------- UI MENU FUNCTIONS ---------------
void print_invoice(Payment payment) {
	Reservation reservation = *get_reservation_by_id(payment.reservationID);
	Customer customer = *get_customer_by_id(reservation.customerID);

	// print reservation detials
	// info part
	// reservation id
	// reservation id
	print_table_row("|Reservation ID         : " + payment.reservationID);

	// customer name
	print_table_row("|Customer               : " + customer.name);

	// room number
	print_table_row("|Room Number            : " + to_string(reservation.roomNumber));

	// check in and check out
	print_table_row("|Check-In Date          : " + reservation.checkInDate);
	print_table_row("|Check-Out Date         : " + reservation.checkOutDate);

	// number of nights
	print_table_row("|Number of Night        : " + to_string(reservation.numberOfNights));

	print_divider_with_space(true);

	// money part
	// room fee
	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Room Fee",
			payment.roomFee)
	);

	// membership discount
	if (payment.membershipDiscount != 0)
	{
		print_table_row(
			format("{:<23}: RM{:>9.2f} (-)",
				"|Membership Discount",
				payment.membershipDiscount)
		);
	}

	// deposit
	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Security Deposit",
			payment.depositAmount)
	);

	// additional charge
	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Additional Charge",
			payment.additionalCharge)
	);

	empty_line();
	print_divider();

	// total
	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Total Amount",
			payment.totalAmount)
	);
	print_divider();
}

void process_payment_screen(Payment &payment) {
	// clear
	clear_screen();

	// print header
	print_header("Process Payment");
	empty_line();

	// print the invoice
	print_invoice(payment);

	cout << endl;

	// ask user input
	// variable use to store the information
	string payment_method;
	double amount_paid;
	double changes;
		
	// payment method
	cout << "Select Payment Method: " << endl;
	cout << endl;

	cout << "  [1] Cash" << endl;
	cout << "  [2] Credit Card" << endl;
	cout << "  [3] Debit Card" << endl;
	cout << "  [4] E-Wallet" << endl;

	cout << endl;
	cout << "  [0] Back" << endl;
	cout << endl;

	int choice = get_menu_choice(4);

	switch (choice) {
	case 1:
		// cash
		payment_method = "Cash";

		// if the payment method = cash only ask to enter the amount paid
		cout << "Enter the amount paid: ";



		break;
	case 2:
		// credit card
		payment_method = "Credit Card";
		break;
	case 3:
		// debit card
		payment_method = "Debit Card";
		break;
	case 4:
		// eWallet
		payment_method = "E-Wallet";
		break;
	default:
		// 0, back
		return;
	}
}

void payment_detail_screen(Payment payment) {
	// clear
	clear_screen();
	
	// print header
	print_header("Payment Detail");

	empty_line();
	print_invoice(payment);

	cout << endl;
	
	// print choices
	cout << "  [1] Proceed Payment" << endl;
	cout << "  [0] Back" << endl;

	cout << endl;

	// ask user input
	int choice = get_menu_choice(1);

	if (choice == 1)
	{
		// process payment
		process_payment_screen(*get_payment_by_id(payment.paymentID));
	}
}

void unpaid_payments_screen() {
	// clear screen
	clear_screen();

	// print header
	print_header("Unpaid Payments");

	cout << endl;

	// show the information in table form
	// table header row
	cout << setw(18) << left << "Reservation ID"
		<< setw(12) << left << "Customer"
		<< setw(8) << left << "Room"
		<< setw(12) << left <<"Check-In"
		<< setw(10) << "Nights"
		<< setw(11) << left << "Amount" 
		<< setw(6) << left << "Status" << endl;

	print_divider();

	vector<Payment> unpaid_payments;

	Reservation reservation;
	// show the unpaid payment information
	for (Payment payment : payments)
	{
		// only show unpaid payment
		if (payment.paymentStatus == "Unpaid")
		{
			// add the payment obj into the unpaid payment list
			// for future operation
			unpaid_payments.push_back(payment);
			
			reservation = *get_reservation_by_id(payment.reservationID);

			cout << setw(18) << left << reservation.reservationID
				<< setw(12) << left << reservation.customerID // change it to name ltr
				<< setw(8) << left << reservation.roomNumber
				<< setw(12) << left << reservation.checkInDate
				<< setw(10) << reservation.numberOfNights
				<< setw(11) << left << payment.totalAmount
				<< setw(6) << left << payment.paymentStatus << endl;
		}
	}

	print_divider();
	cout << endl;

	bool valid = false;

	string reservationID;
	do
	{
		// ask user to enter reservation id to continue
		cout << "Enter the Reservation ID to proceed payment[0 to back]: ";
		cin >> reservationID;

		// process it to all upper case
		if (reservationID == "0")
		{
			 // back
			return;
		}

		transform(reservationID.begin(), reservationID.end(), reservationID.begin(), ::toupper);

		// reservation id validation checking
		// check format
		valid = check_reservation_id_format(reservationID);

		// format wrong
		if (!valid)
		{
			cout << "Reservation ID format incorrect..." << endl;
			continue;
		}

		// valid = true already
		// now format correct
		// change valid variable back to false
		valid = false;

		// check whether exists or not
		for (Payment payment : unpaid_payments)
		{
			// if found
			if (payment.reservationID == reservationID)
			{
				valid = true;
				// call the function to show the payment detail
				payment_detail_screen(*get_payment_by_reservation_id(reservationID));
				break;
			}
		}

		// if not found
		if (!valid)
		{
			cout << "Reservation not found..." << endl;
		}

	} while (!valid);

}

void payment_menu() {

	int choice;

	do {
		// clear screen
		clear_screen();

		// display menu header
		print_header("Payment and Reporting");

		// payment menu
		empty_line();
		cout << "|  [1] View Unpaid Payments" << setw(80-27) << right << "|" << endl;
		cout << "|  [2] Search Payment" << setw(80 - 21) << right << "|" << endl;
		cout << "|  [3] Process Refund" << setw(80 - 21) << right << "|" << endl;
		cout << "|  [4] Settle Deposit" << setw(80 - 21) << right << "|" << endl;
		cout << "|  [5] Generate Report" << setw(80 - 22) << right << "|" << endl;

		empty_line();
		cout << "|  [0] Back" << setw(80 - 11) << right << "|" << endl;
		print_divider_with_space(false);

		// ask user to input a choice
		choice = get_menu_choice(5);

		switch (choice) {
		case 1:
			// show unpaid payments list
			unpaid_payments_screen();
			break;
		case 2:
			// search payment
			break;
		case 3:
			// process refund
			break;
		case 4:
			// settle deposit
			break;
		case 5:
			// generate report
			break;
		default:
			// means the choice = 0, back
			return;
		}
	} while (choice != 0);
}