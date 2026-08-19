#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <format>
#include <ctime>
#include <sstream>
#include <conio.h>
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

bool check_payment_id_format(string reservationID) {
	regex pattern(R"(^PAY[0-9]{3}$)");
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

double get_amount_paid(double totalAmount)
{
	double amountPaid;

	while (true)
	{
		cout << setw(20) << left << "Enter Amount Paid[0 = Exact]" << ": ";
		cin >> amountPaid;

		// check non-numeric input
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "Invalid input. Please enter a valid amount." << endl;
			continue;
		}

		// if 0 means exact
		if (amountPaid == 0)
		{
			return totalAmount;
		}

		// check negative amount
		if (amountPaid < 0)
		{
			cout << "Amount paid must be greater than RM0.00." << endl;
			continue;
		}

		// check insufficient payment
		if (amountPaid < totalAmount)
		{
			cout << "Insufficient amount. Total amount is RM"
				<< fixed << setprecision(2)
				<< totalAmount << "." << endl;

			continue;
		}

		return amountPaid;
	}
}

/*
	return today date in string format
*/
string get_today_date() {
	time_t now = time(0);

	tm localTime;
	localtime_s(&localTime, &now);

	stringstream ss;

	ss << setfill('0')
		<< setw(2) << localTime.tm_mday << "/"
		<< setw(2) << localTime.tm_mon + 1 << "/"
		<< localTime.tm_year + 1900;

	return ss.str();
}

vector<Payment*> get_payments_by_customer_name(string customerName) {
	vector<Payment*> matchedPayments;

	for (Customer& customer : customers) {
		// find customer with matching name
		if (customer.name == customerName) {

			// find reservations belonging to this customer
			for (Reservation& reservation : reservations) {
				if (reservation.customerID == customer.customerID) {

					// find payment belonging to the reservation
					Payment* payment =
						get_payment_by_reservation_id(
							reservation.reservationID
						);

					if (payment != nullptr) {
						matchedPayments.push_back(payment);
					}
				}
			}
		}
	}

	return matchedPayments;
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

void print_receipt(Payment payment) {
	// header
	print_header("Receipt");

	// print same info as invoice
	print_invoice(payment);

	empty_line();

	print_table_row(format("|{:<23}: {}", "Payment Method", payment.paymentMethod));
	print_table_row(format("|{:<23}: {}", "Amount Paid", payment.amountPaid));
	print_table_row(format("|{:<23}: {}", "Change", payment.change));
	print_table_row(format("|{:<23}: {}", "Payment Date", payment.paymentDate));
	print_table_row(format("|{:<23}: {}", "Payment Status", payment.paymentStatus));

	empty_line();
}

void payment_successful(Payment payment) {
	// clear
	clear_screen();

	// print receipt
	print_receipt(payment);

	cout << endl;

	// footer
	print_header("Thanks For Your Payment");
	cout << "Press any key to continue...";
	_getch();
}

void confirm_payment_screen(Payment& payment, string payment_method) {
	// clear
	clear_screen();

	// display header
	print_header("Process Payment");
	empty_line();

	// display payment method
	print_table_row(format("{:<20}: {}", "|Payment Method", payment_method));

	// display total amount
	print_table_row(format("{:<20}: RM{:>9.2f}", "|Total Amount", payment.totalAmount));

	double amount_paid;
	double changes;

	if (payment_method == "Cash")
	{
		// promt user enter the amount paid
		// if the payment method = cash only ask to enter the amount paid
		// all validation checking inside this function
		amount_paid = get_amount_paid(payment.totalAmount);
		// clear line
		cout << "\033[1A"; // move cursor up 1 line
		cout << "\r\033[2K"; // clear entire line
	}
	else
	{
		amount_paid = payment.totalAmount;
	}

	print_table_row(format("{:<20}: RM{:>9.2f}", "|Amount Paid", amount_paid));

	// calculate and display changes
	changes = amount_paid - payment.totalAmount;
	
	print_table_row(format("{:<20}: RM{:>9.2f}", "|Changes", changes));

	print_divider_with_space(false);

	// confirm payment
	cout << "Confirm Payment? " << endl;
	cout << endl;

	int choice;

	cout << "  [1] Confirm" << endl;
	cout << "  [0] Cancel" << endl;
	cout << endl;

	choice = get_menu_choice(1);
	
	if (choice == 1)
	{
		// confirm payment
		
		// set the payment method
		payment.paymentMethod = payment_method;
		// set the amount paid value
		payment.amountPaid = amount_paid;
		// set the changes value
		payment.change = changes;
		// change the payment status to "Paid"
		payment.paymentStatus = "Paid";
		// set the payment date to today date
		payment.paymentDate = get_today_date();

		// update this all to file
		save_payments_to_file();

		// payment successful
		payment_successful(payment);
	}
	else
	{
		// cancelled
		return;
	}
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
	confirm_payment_screen(payment, payment_method);
}

void print_payment_detail(Payment payment) {
	// clear
	clear_screen();

	// print header
	print_header("Payment Detail");

	empty_line();
	print_invoice(payment);

	cout << endl;
}

void payment_detail_screen(Payment payment) {
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

void print_payment_table(const vector<Payment*>& payments)
{
	// table header
	print_table_row(
		format("|{:<18}{:<16}{:<8}{:<12}{:<8}{:<10}{:<6}",
			"Reservation ID",
			"Customer",
			"Room",
			"Check-In",
			"Nights",
			"Amount",
			"Status")
	);

	print_divider();

	// payment rows
	for (Payment* payment : payments)
	{
		if (payment == nullptr)
		{
			continue;
		}

		// get reservation
		Reservation* reservation =
			get_reservation_by_id(payment->reservationID);

		if (reservation == nullptr)
		{
			continue;
		}

		// get customer
		Customer* customer =
			get_customer_by_id(reservation->customerID);

		string customerName = "-";

		if (customer != nullptr)
		{
			customerName = customer->name;
		}

		// print row
		print_table_row(
			format("|{:<18}{:<16}{:<8}{:<12}{:<8}{:<10.2f}{:<6}",
				reservation->reservationID,
				customerName,
				reservation->roomNumber,
				reservation->checkInDate,
				reservation->numberOfNights,
				payment->totalAmount,
				payment->paymentStatus)
		);
	}

	print_divider();
	cout << endl;
}


void unpaid_payments_screen() {
	// clear screen
	clear_screen();

	// print header
	print_header("Unpaid Payments");

	vector<Payment*> unpaid_payments;

	// only show unpaid payment
	for (Payment& payment : payments) {
		if (payment.paymentStatus == "Unpaid")
		{
			// add the payment obj into the unpaid payment list
			// for future operation
			unpaid_payments.push_back(&payment);
		}
	}
	
	print_payment_table(unpaid_payments);

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
		for (Payment* payment : unpaid_payments)
		{
			// if found
			if (payment->reservationID == reservationID)
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

void payment_search_result_screen(Payment payment) {
	if (payment.paymentStatus == "Unpaid")
	{
		print_payment_detail(payment);

		cout << "Press any key to continue...";
		_getch();
	}
	else
	{
		payment_successful(payment);
	}
}

void payment_search_result_screen(const vector<Payment*>& payments) {
	clear_screen();

	print_header("Search Results");
	empty_line();

	// display table
	for (Payment* payment : payments)
	{
		if (payment == nullptr)
		{
			continue;
		}

		cout << setw(15) << left << payment->paymentID
			<< setw(18) << left << payment->reservationID
			<< setw(12) << fixed << setprecision(2) << payment->totalAmount
			<< setw(12) << left << payment->paymentStatus
			<< endl;
	}

	cout << endl;

	string paymentID;

	while (true)
	{
		cout << "Enter Payment ID to view details [0 to back]: ";
		cin >> paymentID;

		if (paymentID == "0")
		{
			return;
		}

		transform(
			paymentID.begin(),
			paymentID.end(),
			paymentID.begin(),
			::toupper
		);

		// check format
		if (!check_payment_id_format(paymentID))
		{
			cout << "Payment ID format incorrect..." << endl;
			continue;
		}

		bool found = false;

		for (Payment* payment : payments)
		{
			if (payment == nullptr)
			{
				continue;
			}

			if (payment->paymentID == paymentID)
			{
				found = true;

				// dereference pointer and pass Payment object
				payment_search_result_screen(*payment);

				break;
			}
		}

		if (!found)
		{
			cout << "Payment not found in search results..." << endl;
			continue;
		}

		return;
	}
}

void search_payment_screen() {
	// clear
	clear_screen();

	// header
	print_header("Search Payment");

	empty_line();

	// menu selection
	print_table_row("|  [1] Search by Payment ID");
	print_table_row("|  [2] Search by Reservation ID");
	print_table_row("|  [3] Search by Customer Name");
	empty_line();
	print_table_row("|  [0] Back");
	empty_line();
	print_divider_with_space(false);

	cout << endl;

	// get choice
	int choice;
	choice = get_menu_choice(3);

	// back
	if (choice == 0) {
		return;
	}

	// clear
	clear_screen();

	// header 
	print_header("Search Payment");

	string input;

	bool valid = false;
	do {
		cout << endl;

		// prompt user enter payment id, reservation id or customer name and store inside input
		cout << "Enter "
			<< ((choice == 1) ? "Payment ID"
				: (choice == 2 ? "Reservation ID"
					: "Customer Name"))
			<< " [0 to back]: ";

		getline(cin, input);

		// back
		if (input == "0") {
			return;
		}

		// SEARCH BY PAYMENT ID
		if (choice == 1) {

			// convert to uppercase
			transform(
				input.begin(),
				input.end(),
				input.begin(),
				::toupper
			);

			// check format
			if (!check_payment_id_format(input)) {
				cout << "Payment ID format incorrect..." << endl;
				continue;
			}

			// check exists
			Payment* payment = get_payment_by_id(input);

			if (payment == nullptr) {
				cout << "Payment not found..." << endl;
				continue;
			}

			valid = true;

			// show result
			payment_search_result_screen(*payment);
		}

		// SEARCH BY RESERVATION ID
		else if (choice == 2) {

			// convert to uppercase
			transform(
				input.begin(),
				input.end(),
				input.begin(),
				::toupper
			);

			// check format
			if (!check_reservation_id_format(input)) {
				cout << "Reservation ID format incorrect..." << endl;
				continue;
			}

			// check payment exists for reservation
			Payment* payment =
				get_payment_by_reservation_id(input);

			if (payment == nullptr) {
				cout << "Payment not found..." << endl;
				continue;
			}

			valid = true;

			// show result
			payment_search_result_screen(*payment);
		}

		// SEARCH BY CUSTOMER NAME
		else if (choice == 3) {

			// empty input
			if (input.empty()) {
				cout << "Customer name cannot be empty..." << endl;
				continue;
			}

			// check name format
			regex namePattern(R"(^[A-Za-z ]+$)");

			if (!regex_match(input, namePattern)) {
				cout << "Customer name can only contain letters and spaces..."
					<< endl;
				continue;
			}

			// search payments belonging to this customer
			vector<Payment*> matchedPayments =
				get_payments_by_customer_name(input);

			if (matchedPayments.empty()) {
				cout << "No payment found for this customer..." << endl;
				continue;
			}

			valid = true;

			// show multiple results
			payment_search_result_screen(matchedPayments);
		}

	} while (!valid);

}

void refund_successful_screen(Payment payment)
{
	// clear screen
	clear_screen();

	// header
	print_header("Refund Successful");

	empty_line();

	// payment information
	print_table_row(
		format("{:<23}: {}",
			"|Payment ID",
			payment.paymentID)
	);

	print_table_row(
		format("{:<23}: {}",
			"|Reservation ID",
			payment.reservationID)
	);

	empty_line();

	// refund information
	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Refund Amount",
			payment.refundAmount)
	);

	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Deposit Retained",
			payment.depositRetained)
	);

	print_table_row(
		format("{:<23}: {}",
			"|Payment Status",
			payment.paymentStatus)
	);

	empty_line();

	print_divider_with_space(false);

	cout << endl;
	cout << "Refund completed successfully." << endl;

	cout << endl;
	cout << "Press any key to continue...";
	_getch();
}

void refund_detail_screen(Payment payment) {
	// clear
	clear_screen();

	// show receipt
	print_receipt(payment);

	cout << endl;

	// calculate refund information
	double refundAmount =
		payment.roomFee - payment.membershipDiscount;

	double depositRetained =
		payment.depositAmount;

	// display refund summary
	print_divider();

	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Refund Amount",
			refundAmount)
	);

	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Deposit Retained",
			depositRetained)
	);

	print_divider();

	cout << endl;

	// ask confirm refund
	cout << "Confirm Refund?" << endl;
	cout << endl;

	cout << "  [1] Confirm Refund" << endl;
	cout << "  [0] Back" << endl;
	cout << endl;

	int choice = get_menu_choice(1);

	// back
	if (choice == 0)
	{
		return;
	}

	// process refund
	payment.refundAmount = refundAmount;
	payment.depositRetained = depositRetained;
	payment.depositReturned = 0.0;
	payment.paymentStatus = "Refunded";

	// save changes
	save_payments_to_file();

	// show successful refund screen
	refund_successful_screen(payment);

}

void process_refund_screen() {
	// clear screen
	clear_screen();

	// header
	print_header("Process Refund");

	// get the paid, cancelled payments list
	vector<Payment*> refundable_payments;

	for (Payment& payment : payments)
	{
		// get the reservation that belongs to this payment
		Reservation* reservation =
			get_reservation_by_id(payment.reservationID);

		// reservation not found
		if (reservation == nullptr)
		{
			continue;
		}

		// only cancelled reservation + paid payment can be refunded
		if (reservation->reservationStatus == "Cancelled" &&
			payment.paymentStatus == "Paid")
		{
			refundable_payments.push_back(&payment);
		}
	}

	// display
	print_payment_table(refundable_payments);

	string paymentID;
	bool valid = false;

	// prompt user input
	do
	{
		cout << "Enter Payment ID to process refund [0 to back]: ";
		cin >> paymentID;

		// back
		if (paymentID == "0")
		{
			return;
		}

		// convert to uppercase
		transform(
			paymentID.begin(),
			paymentID.end(),
			paymentID.begin(),
			::toupper
		);

		// check payment ID format
		if (!check_payment_id_format(paymentID))
		{
			cout << "Payment ID format incorrect..." << endl;
			continue;
		}

		// check whether payment exists in refundable list
		for (Payment* payment : refundable_payments)
		{
			if (payment != nullptr &&
				payment->paymentID == paymentID)
			{
				valid = true;

				// proceed refund
				refund_detail_screen(*payment);

				break;
			}
		}

		if (!valid)
		{
			cout << "Refundable payment not found..." << endl;
		}

	} while (!valid);
}

void deposit_settlement_successful_screen(Payment payment)
{
	clear_screen();

	print_header("Deposit Settlement Successful");

	empty_line();

	print_table_row(
		format("{:<23}: {}",
			"|Payment ID",
			payment.paymentID)
	);

	print_table_row(
		format("{:<23}: {}",
			"|Reservation ID",
			payment.reservationID)
	);

	empty_line();

	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Damage Charge",
			payment.damageCharge)
	);

	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Deposit Retained",
			payment.depositRetained)
	);

	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Deposit Returned",
			payment.depositReturned)
	);

	print_table_row(
		format("{:<23}: {}",
			"|Payment Status",
			payment.paymentStatus)
	);

	empty_line();

	print_table_row("|Deposit settlement completed successfully.");

	empty_line();

	print_divider_with_space(false);

	cout << "Press any key to continue...";
	_getch();
}

void settle_deposit_screen(Payment& payment)
{
	// clear screen
	clear_screen();

	// header
	print_header("Settle Deposit");

	empty_line();

	// payment information
	print_table_row(
		format("{:<23}: {}",
			"|Payment ID",
			payment.paymentID)
	);

	print_table_row(
		format("{:<23}: {}",
			"|Reservation ID",
			payment.reservationID)
	);

	empty_line();

	// deposit amount
	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Security Deposit",
			payment.depositAmount)
	);

	empty_line();
	print_divider_with_space(false);

	// ask charges
	double damageCharge =
		get_non_negative_amount("Enter Damage Charge     : RM ");

	double additionalCharge =
		get_non_negative_amount("Enter Additional Charge : RM ");

	// calculate total deduction
	double totalDeduction =
		damageCharge + additionalCharge;

	double depositReturned;
	double depositRetained;

	// deduction exceeds deposit
	if (totalDeduction >= payment.depositAmount)
	{
		depositReturned = 0.0;
		depositRetained = payment.depositAmount;
	}
	else
	{
		depositRetained = totalDeduction;

		depositReturned =
			payment.depositAmount - totalDeduction;
	}

	empty_line();

	// display charges
	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Damage Charge",
			damageCharge)
	);

	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Additional Charge",
			additionalCharge)
	);

	empty_line();
	print_divider();

	// settlement result
	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Deposit Retained",
			depositRetained)
	);

	print_table_row(
		format("{:<23}: RM{:>9.2f}",
			"|Deposit Returned",
			depositReturned)
	);

	empty_line();
	print_divider_with_space(false);

	// confirm settlement
	cout << "Confirm Deposit Settlement?" << endl;

	empty_line();

	print_table_row("|  [1] Confirm");
	print_table_row("|  [0] Back");

	empty_line();
	print_divider_with_space(false);

	int choice = get_menu_choice(1);

	// back
	if (choice == 0)
	{
		return;
	}

	// update payment
	payment.damageCharge = damageCharge;
	payment.additionalCharge += additionalCharge;
	payment.depositRetained = depositRetained;
	payment.depositReturned = depositReturned;

	// payment lifecycle completed
	payment.paymentStatus = "Completed";

	// save changes
	save_payments_to_file();

	// successful screen
	deposit_settlement_successful_screen(payment);
}

void settle_deposit_payments_screen()
{
	// clear
	clear_screen();

	// header
	print_header("Settle Deposit");

	empty_line();

	// store payments that are allowed to settle deposit
	vector<Payment*> settlement_payments;

	for (Payment& payment : payments)
	{
		// only paid payments
		if (payment.paymentStatus != "Paid")
		{
			continue;
		}

		// get reservation
		Reservation* reservation =
			get_reservation_by_id(payment.reservationID);

		if (reservation == nullptr)
		{
			continue;
		}

		// only reservation ready for checkout/deposit settlement
		if (reservation->reservationStatus == "Checked-out")
		{
			settlement_payments.push_back(&payment);
		}
	}

	// no payment available
	if (settlement_payments.empty())
	{
		print_table_row("|No payment available for deposit settlement.");

		empty_line();
		print_divider_with_space(false);

		cout << "Press any key to continue...";
		_getch();

		return;
	}

	// display payments
	print_payment_table(settlement_payments);

	// ask staff to select payment
	string paymentID;

	while (true)
	{
		cout << "Enter Payment ID to settle deposit [0 to back]: ";
		cin >> paymentID;

		// back
		if (paymentID == "0")
		{
			return;
		}

		// uppercase
		transform(
			paymentID.begin(),
			paymentID.end(),
			paymentID.begin(),
			::toupper
		);

		// check format
		if (!check_payment_id_format(paymentID))
		{
			cout << "Payment ID format incorrect..." << endl;
			continue;
		}

		// check whether payment is inside settlement list
		bool found = false;

		for (Payment* payment : settlement_payments)
		{
			if (payment != nullptr &&
				payment->paymentID == paymentID)
			{
				found = true;

				// proceed to settlement screen
				settle_deposit_screen(*payment);

				break;
			}
		}

		if (!found)
		{
			cout << "Payment not available for deposit settlement..." << endl;
			continue;
		}

		return;
	}
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
			search_payment_screen();
			break;
		case 3:
			// process refund
			process_refund_screen();
			break;
		case 4:
			// settle deposit
			settle_deposit_payments_screen();
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