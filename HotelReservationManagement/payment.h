#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
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
double calculateDeposit(double roomFee) {
	return roomFee * DEPOSIT_RATE;
}

/*
	return the total amount of the payment
*/
double calculateTotalAmount(
	double roomFee,
	double discount,
	double additionalCharge
) {
	return (roomFee + additionalCharge + calculateDeposit(roomFee)) - discount;
}

void createUnpaidPayment(Reservation reservation) {
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
	payment.totalAmount = calculateTotalAmount(payment.roomFee, 0.0, 0.0);

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


// --------------- UI MENU FUNCTIONS ---------------
void payment_detail_screen(Payment payment) {
	// clear
	clear_screen();

	Reservation reservation = *get_reservation_by_id(payment.reservationID);
	Customer customer = *get_customer_by_id(reservation.customerID);

	// print header
	print_header("Payment Detail");

	cout << endl;

	// print reservation detials
	// info part
	// reservation id
	cout << setw(23) << left << "Reservation ID" << ": " << payment.reservationID << endl;

	// customer name
	cout << setw(23) << left << "Customer" << ": " << customer.name << endl;

	// room number
	cout << setw(23) << left << "Room Number" << ": " << reservation.roomNumber << endl;

	// check in and check out
	cout << setw(23) << left << "Check-In Date" << ": " << reservation.checkInDate << endl;
	cout << setw(23) << left << "Check-Out Date" << ": " << reservation.checkOutDate << endl;

	// num of night
	cout << setw(23) << left << "Number of Night" << ": " << reservation.numberOfNights << endl;

	print_divider_with_space();

	// money part
	// room fee
	cout << setw(23) << left << "Room Fee" << ": RM" << payment.roomFee << endl;

	// membership discount
	if (payment.membershipDiscount != 0)
	{
		// if the customer is membership only print this
		cout << setw(23) << left << "Membership Discount" << ": RM" << payment.roomFee << endl;
	}

	// deposit
	cout << setw(23) << left << "Security Deposit" << ": RM" << payment.depositAmount << endl;

	// additional charge
	cout << setw(23) << left << "Additional Charge" << ": RM" << payment.additionalCharge << endl;

	print_divider_with_space();
	
	// print choices
	cout << "  [1] Proceed Payment" << endl;
	cout << "  [0] Back" << endl;

	cout << endl;

	// ask user input
	int choice = get_menu_choice(1);
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
		cout << endl;
		cout << "  [1] View Unpaid Payments" << endl;
		cout << "  [2] Search Payment" << endl;
		cout << "  [3] Process Refund" << endl;
		cout << "  [4] Settle Deposit" << endl;
		cout << "  [5] Generate Report" << endl;

		cout << endl;
		cout << "  [0] Back" << endl;
		cout << endl;
		print_divider();
		cout << endl;

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