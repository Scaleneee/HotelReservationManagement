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

// for daily report
const int DAILY_REPORT_ROWS = 4;
const int DAILY_REPORT_COLS = 2;

const int ROW_COLLECTED = 0;
const int ROW_REFUND = 1;
const int ROW_DEPOSIT_RETURNED = 2;
const int ROW_DEPOSIT_RETAINED = 3;

const int COL_COUNT = 0;
const int COL_AMOUNT = 1;

// yearly report
const int YEARLY_REPORT_MONTHS = 12;

const int YEAR_COLLECTED = 0;
const int YEAR_REFUND = 1;
const int YEAR_DEPOSIT_RETURNED = 2;
const int YEAR_DEPOSIT_RETAINED = 3;

const int YEARLY_REPORT_COLS = 4;

// --------------- LOGIC FUNCTIONS ---------------
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

bool get_month_year_from_date(
	const string& date,
	int& month,
	int& year
)
{
	// payment date may be empty for unpaid payment
	if (date.empty())
	{
		return false;
	}

	int day;
	char slash1;
	char slash2;

	stringstream ss(date);

	ss >> day >> slash1 >> month >> slash2 >> year;

	if (ss.fail() ||
		slash1 != '/' ||
		slash2 != '/')
	{
		return false;
	}

	return true;
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
						reservation_find_payment_by_id(
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

Membership* get_membership_by_customer_id(string customerID)
{
	for (Membership& membership : memberships)
	{
		if (membership.customerID == customerID &&
			membership.status == "Active")
		{
			return &membership;
		}
	}

	return nullptr;
}

void add_membership_points(Payment& payment)
{
	Reservation* reservation =
		get_reservation_by_id(payment.reservationID);

	if (reservation == nullptr)
	{
		return;
	}

	Membership* membership =
		get_membership_by_customer_id(
			reservation->customerID
		);

	if (membership == nullptr)
	{
		return;
	}

	// RM10 room spending = 1 point
	int earnedPoints =
		static_cast<int>(
			(payment.roomFee - payment.membershipDiscount) / 10
			);

	membership->points += earnedPoints;

	save_memberships_to_file();
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
	print_table_row(format("|{:<23}: RM{:>9.2f}", "Amount Paid", payment.amountPaid));
	print_table_row(format("|{:<23}: RM{:>9.2f}", "Change", payment.change));
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

int get_points_to_redeem(
	Membership& membership,
	double maxDiscount
)
{
	if (membership.points <= 0)
	{
		cout << "No membership points available." << endl;
		return 0;
	}

	cout << endl;
	cout << "Membership Points Available : "
		<< membership.points << endl;

	cout << endl;
	cout << "Use membership points?" << endl;
	cout << "  [1] Yes" << endl;
	cout << "  [0] No" << endl;
	cout << endl;

	int choice = get_menu_choice(1);

	if (choice == 0)
	{
		return 0;
	}

	int pointsToUse;

	while (true)
	{
		cout << "Enter points to redeem [0 to cancel]: ";
		cin >> pointsToUse;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "Invalid input. Please enter a number."
				<< endl;

			continue;
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// cancel point redemption
		if (pointsToUse == 0)
		{
			return 0;
		}

		if (pointsToUse < 0)
		{
			cout << "Points cannot be negative." << endl;
			continue;
		}

		if (pointsToUse > membership.points)
		{
			cout << "Insufficient membership points." << endl;
			continue;
		}

		// calculate point discount
		double pointDiscount =
			static_cast<double>(pointsToUse) / POINTS_PER_RM;

		// don't allow discount to exceed allowed amount
		if (pointDiscount > maxDiscount)
		{
			cout << "Too many points selected." << endl;
			continue;
		}

		return pointsToUse;
	}
}

void confirm_payment_screen(
	Payment& payment,
	string payment_method
)
{
	clear_screen();

	print_header("Process Payment");
	empty_line();

	// find reservation
	Reservation* reservation =
		get_reservation_by_id(payment.reservationID);

	Membership* membership = nullptr;

	if (reservation != nullptr)
	{
		membership =
			get_membership_by_customer_id(
				reservation->customerID
			);
	}

	// -----------------------------
	// POINT REDEMPTION
	// -----------------------------

	int pointsToUse = 0;
	double pointDiscount = 0.0;

	// start with original payment total
	double finalTotalAmount = payment.totalAmount;

	if (membership != nullptr)
	{
		print_table_row(
			format(
				"{:<23}: {}",
				"|Membership Level",
				membership->level
			)
		);

		print_table_row(
			format(
				"{:<23}: {}",
				"|Available Points",
				membership->points
			)
		);

		empty_line();
		print_divider_with_space(false);

		// point discount should not exceed room charge
		double maxDiscount =
			payment.roomFee
			- payment.membershipDiscount;

		pointsToUse =
			get_points_to_redeem(
				*membership,
				maxDiscount
			);

		if (pointsToUse > 0)
		{
			pointDiscount =
				static_cast<double>(pointsToUse)
				/ POINTS_PER_RM;

			finalTotalAmount -= pointDiscount;
		}
	}

	// -----------------------------
	// DISPLAY PAYMENT INFO
	// -----------------------------

	clear_screen();

	print_header("Process Payment");
	empty_line();

	print_table_row(
		format(
			"{:<23}: {}",
			"|Payment Method",
			payment_method
		)
	);

	print_table_row(
		format(
			"{:<23}: RM{:>9.2f}",
			"|Original Amount",
			payment.totalAmount
		)
	);

	if (pointDiscount > 0)
	{
		print_table_row(
			format(
				"{:<23}: {} points",
				"|Points Redeemed",
				pointsToUse
			)
		);

		print_table_row(
			format(
				"{:<23}: RM{:>9.2f} (-)",
				"|Point Discount",
				pointDiscount
			)
		);
	}

	print_divider();

	print_table_row(
		format(
			"{:<23}: RM{:>9.2f}",
			"|Final Amount",
			finalTotalAmount
		)
	);

	empty_line();
	print_divider_with_space(false);

	// -----------------------------
	// GET AMOUNT PAID
	// -----------------------------

	double amount_paid;

	if (payment_method == "Cash")
	{
		amount_paid =
			get_amount_paid(finalTotalAmount);
	}
	else
	{
		amount_paid = finalTotalAmount;
	}

	double changes =
		amount_paid - finalTotalAmount;

	empty_line();

	print_table_row(
		format(
			"{:<23}: RM{:>9.2f}",
			"|Amount Paid",
			amount_paid
		)
	);

	print_table_row(
		format(
			"{:<23}: RM{:>9.2f}",
			"|Change",
			changes
		)
	);

	empty_line();
	print_divider_with_space(false);

	// -----------------------------
	// CONFIRM
	// -----------------------------

	cout << "Confirm Payment?" << endl;
	cout << endl;

	cout << "  [1] Confirm" << endl;
	cout << "  [0] Cancel" << endl;
	cout << endl;

	int choice = get_menu_choice(1);

	if (choice == 0)
	{
		// IMPORTANT:
		// points have not been deducted yet
		return;
	}

	// -----------------------------
	// PAYMENT CONFIRMED
	// -----------------------------

	payment.paymentMethod = payment_method;
	payment.amountPaid = amount_paid;
	payment.change = changes;
	payment.paymentStatus = "Paid";
	payment.paymentDate = get_today_date();

	// add point redemption discount
	payment.membershipDiscount += pointDiscount;

	// update final amount
	payment.totalAmount = finalTotalAmount;

	// NOW deduct points
	if (membership != nullptr &&
		pointsToUse > 0)
	{
		membership->points -= pointsToUse;

		save_memberships_to_file();
	}

	save_payments_to_file();
	save_payments_to_file();

	// award membership points after successful payment
	add_membership_points(payment);

	payment_successful(payment);
}

void process_payment_screen(Payment& payment)
{
	while (true)
	{
		clear_screen();

		print_header("Process Payment");
		empty_line();

		print_invoice(payment);

		empty_line();

		print_table_row("|Select Payment Method:");
		empty_line();

		print_table_row("|  [1] Cash");
		print_table_row("|  [2] Credit Card");
		print_table_row("|  [3] Debit Card");
		print_table_row("|  [4] E-Wallet");

		empty_line();

		print_table_row("|  [0] Back");

		empty_line();
		print_divider_with_space(false);

		int choice = get_menu_choice(4);

		// back to payment detail
		if (choice == 0)
		{
			return;
		}

		string payment_method;

		switch (choice)
		{
		case 1:
			payment_method = "Cash";
			break;

		case 2:
			payment_method = "Credit Card";
			break;

		case 3:
			payment_method = "Debit Card";
			break;

		case 4:
			payment_method = "E-Wallet";
			break;
		}

		confirm_payment_screen(payment, payment_method);

		// successful payment
		if (payment.paymentStatus == "Paid")
		{
			return;
		}
	}
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

void payment_detail_screen(Payment& payment)
{
	while (true)
	{
		// display payment detail
		print_payment_detail(payment);

		print_table_row("|  [1] Proceed Payment");
		print_table_row("|  [0] Back");

		empty_line();
		print_divider_with_space(false);

		int choice = get_menu_choice(1);

		// back to unpaid payments screen
		if (choice == 0)
		{
			return;
		}

		// proceed payment
		process_payment_screen(payment);

		// if payment successfully completed,
		// don't show payment detail again
		if (payment.paymentStatus == "Paid")
		{
			return;
		}
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
				payment_detail_screen(*payment);
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

void refund_detail_screen(Payment& payment) {
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

void process_refund_screen()
{
	while (true)
	{
		clear_screen();

		print_header("Process Refund");

		vector<Payment*> refundable_payments;

		// filter refundable payments
		for (Payment& payment : payments)
		{
			Reservation* reservation =
				get_reservation_by_id(payment.reservationID);

			if (reservation == nullptr)
			{
				continue;
			}

			if (reservation->reservationStatus == "Cancelled" &&
				payment.paymentStatus == "Paid")
			{
				refundable_payments.push_back(&payment);
			}
		}

		// no refundable payment
		if (refundable_payments.empty())
		{
			empty_line();

			print_table_row("|No refundable payments available.");

			empty_line();
			print_divider_with_space(false);

			cout << "Press any key to continue...";
			_getch();

			return;
		}

		print_payment_table(refundable_payments);

		string paymentID;

		cout << "Enter Payment ID to process refund [0 to back]: ";
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

		if (!check_payment_id_format(paymentID))
		{
			cout << "Payment ID format incorrect..." << endl;

			cout << "Press any key to continue...";
			_getch();

			continue;
		}

		Payment* selectedPayment = nullptr;

		for (Payment* payment : refundable_payments)
		{
			if (payment != nullptr &&
				payment->paymentID == paymentID)
			{
				selectedPayment = payment;
				break;
			}
		}

		if (selectedPayment == nullptr)
		{
			cout << "Refundable payment not found..." << endl;

			cout << "Press any key to continue...";
			_getch();

			continue;
		}

		// open refund detail
		refund_detail_screen(*selectedPayment);
	}
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

	clear_screen();

	// display charges
	print_header("Confirm Deposit Settlement");
	empty_line();
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

	cout << endl;
	cout << "  [1] Confirm" << endl;
	cout << "  [0] Back" << endl;
	cout << endl;

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
	while (true)
	{
		clear_screen();

		print_header("Settle Deposit");
		empty_line();

		vector<Payment*> settlement_payments;

		// get all payments eligible for deposit settlement
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

			// only checked-out reservations
			if (reservation->reservationStatus == "Checked-out")
			{
				settlement_payments.push_back(&payment);
			}
		}

		// no eligible payment
		if (settlement_payments.empty())
		{
			print_table_row(
				"|No payment available for deposit settlement."
			);

			empty_line();
			print_divider_with_space(false);

			cout << "Press any key to continue...";
			_getch();

			return;
		}

		// display eligible payments
		print_payment_table(settlement_payments);

		string reservationID;

		cout << "Enter Reservation ID to settle deposit [0 to back]: ";
		cin >> reservationID;

		// back
		if (reservationID == "0")
		{
			return;
		}

		// convert to uppercase
		transform(
			reservationID.begin(),
			reservationID.end(),
			reservationID.begin(),
			::toupper
		);

		// validate reservation ID format
		if (!check_reservation_id_format(reservationID))
		{
			cout << "Reservation ID format incorrect..." << endl;

			cout << "Press any key to continue...";
			_getch();

			continue;
		}

		Payment* selectedPayment = nullptr;

		// search only inside eligible settlement payments
		for (Payment* payment : settlement_payments)
		{
			if (payment != nullptr &&
				payment->reservationID == reservationID)
			{
				selectedPayment = payment;
				break;
			}
		}

		// reservation does not have an eligible payment
		if (selectedPayment == nullptr)
		{
			cout << "Reservation not available for deposit settlement..."
				<< endl;

			cout << "Press any key to continue...";
			_getch();

			continue;
		}

		// proceed to deposit settlement
		settle_deposit_screen(*selectedPayment);
	}
}

void generate_yearly_report()
{
	clear_screen();

	print_header("Yearly Payment Report");
	empty_line();

	int reportYear;

	 
	// GET YEAR
	 

	while (true)
	{
		cout << "Enter Year [0 to back]: ";
		cin >> reportYear;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "Invalid year." << endl;

			continue;
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// back
		if (reportYear == 0)
		{
			return;
		}

		if (reportYear < 2000)
		{
			cout << "Invalid year." << endl;
			continue;
		}

		break;
	}

	 
	// 2D ARRAY
	 

	double yearlyReport
		[YEARLY_REPORT_MONTHS]
		[YEARLY_REPORT_COLS] = {};

	// process every payment
	for (Payment& payment : payments)
	{
		int paymentMonth;
		int paymentYear;

		if (!get_month_year_from_date(
			payment.paymentDate,
			paymentMonth,
			paymentYear))
		{
			continue;
		}

		// only selected year
		if (paymentYear != reportYear)
		{
			continue;
		}

		// array starts from 0
		// January = 0
		// February = 1
		// ...
		int monthIndex = paymentMonth - 1;

		 
		// COLLECTED
		 

		if (payment.paymentStatus == "Paid" ||
			payment.paymentStatus == "Completed" ||
			payment.paymentStatus == "Refunded")
		{
			yearlyReport
				[monthIndex]
				[YEAR_COLLECTED]
				+= payment.amountPaid;
		}

		 
		// REFUND
		 

		if (payment.paymentStatus == "Refunded")
		{
			yearlyReport
				[monthIndex]
				[YEAR_REFUND]
				+= payment.refundAmount;
		}

		 
		// DEPOSIT SETTLEMENT
		 

		if (payment.paymentStatus == "Completed")
		{
			yearlyReport
				[monthIndex]
				[YEAR_DEPOSIT_RETURNED]
				+= payment.depositReturned;

			yearlyReport
				[monthIndex]
				[YEAR_DEPOSIT_RETAINED]
				+= payment.depositRetained;
		}
	}

	string monthNames[12] =
	{
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"
	};

	double yearlyCollected = 0.0;
	double yearlyRefund = 0.0;
	double yearlyDepositReturned = 0.0;
	double yearlyDepositRetained = 0.0;

	clear_screen();

	print_header(
		"Yearly Payment Report - " +
		to_string(reportYear)
	);

	empty_line();

	// table header
	print_table_row(
		format(
			"|{:<12}{:<15}{:<13}{:<15}{:<15}",
			"Month",
			"Collected",
			"Refund",
			"Returned",
			"Retained"
		)
	);

	print_divider();

	 
	// DISPLAY 12 MONTHS
	 

	for (int month = 0;
		month < YEARLY_REPORT_MONTHS;
		month++)
	{
		double collected =
			yearlyReport[month][YEAR_COLLECTED];

		double refund =
			yearlyReport[month][YEAR_REFUND];

		double returned =
			yearlyReport[month][YEAR_DEPOSIT_RETURNED];

		double retained =
			yearlyReport[month][YEAR_DEPOSIT_RETAINED];

		print_table_row(
			format(
				"|{:<12}RM{:<13.2f}RM{:<11.2f}RM{:<13.2f}RM{:<13.2f}",
				monthNames[month],
				collected,
				refund,
				returned,
				retained
			)
		);

		// yearly totals
		yearlyCollected += collected;
		yearlyRefund += refund;
		yearlyDepositReturned += returned;
		yearlyDepositRetained += retained;
	}

	print_divider();
	empty_line();

	double yearlyNet =
		yearlyCollected
		- yearlyRefund
		- yearlyDepositReturned;

	print_table_row(
		format(
			"{:<27}: RM{:>10.2f}",
			"|Total Collected",
			yearlyCollected
		)
	);

	print_table_row(
		format(
			"{:<27}: RM{:>10.2f}",
			"|Total Refund",
			yearlyRefund
		)
	);

	print_table_row(
		format(
			"{:<27}: RM{:>10.2f}",
			"|Deposit Returned",
			yearlyDepositReturned
		)
	);

	print_table_row(
		format(
			"{:<27}: RM{:>10.2f}",
			"|Deposit Retained",
			yearlyDepositRetained
		)
	);

	print_divider();

	print_table_row(
		format(
			"{:<27}: RM{:>10.2f}",
			"|Net Amount",
			yearlyNet
		)
	);

	empty_line();

	print_divider_with_space(false);

	cout << "Press any key to continue...";
	_getch();
}

void generate_monthly_report()
{
	clear_screen();

	print_header("Monthly Payment Report");
	empty_line();

	int reportMonth;
	int reportYear;

	// GET MONTH
	while (true)
	{
		cout << "Enter Month [1 - 12] [0 to back]: ";
		cin >> reportMonth;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "Invalid input. Please enter a number."
				<< endl;

			continue;
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		// back
		if (reportMonth == 0)
		{
			return;
		}

		if (reportMonth < 1 || reportMonth > 12)
		{
			cout << "Month must be between 1 and 12."
				<< endl;

			continue;
		}

		break;
	}

	// GET YEAR
	while (true)
	{
		cout << "Enter Year: ";
		cin >> reportYear;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			cout << "Invalid year." << endl;
			continue;
		}

		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		if (reportYear < 2000)
		{
			cout << "Invalid year." << endl;
			continue;
		}

		break;
	}

	// 2D ARRAY

	double monthlyReport
		[DAILY_REPORT_ROWS]
		[DAILY_REPORT_COLS] = {};

	// process payment records
	for (Payment& payment : payments)
	{
		int paymentMonth;
		int paymentYear;

		// skip payment without valid payment date
		if (!get_month_year_from_date(
			payment.paymentDate,
			paymentMonth,
			paymentYear))
		{
			continue;
		}

		// only selected month and year
		if (paymentMonth != reportMonth ||
			paymentYear != reportYear)
		{
			continue;
		}

		// PAYMENT COLLECTED
		if (payment.paymentStatus == "Paid" ||
			payment.paymentStatus == "Completed" ||
			payment.paymentStatus == "Refunded")
		{
			monthlyReport
				[ROW_COLLECTED]
				[COL_COUNT]++;

			monthlyReport
				[ROW_COLLECTED]
				[COL_AMOUNT]
				+= payment.amountPaid;
		}

		// REFUND
		if (payment.paymentStatus == "Refunded")
		{
			monthlyReport
				[ROW_REFUND]
				[COL_COUNT]++;

			monthlyReport
				[ROW_REFUND]
				[COL_AMOUNT]
				+= payment.refundAmount;
		}

		// DEPOSIT SETTLEMENT
		if (payment.paymentStatus == "Completed")
		{
			if (payment.depositReturned > 0)
			{
				monthlyReport
					[ROW_DEPOSIT_RETURNED]
					[COL_COUNT]++;

				monthlyReport
					[ROW_DEPOSIT_RETURNED]
					[COL_AMOUNT]
					+= payment.depositReturned;
			}

			if (payment.depositRetained > 0)
			{
				monthlyReport
					[ROW_DEPOSIT_RETAINED]
					[COL_COUNT]++;

				monthlyReport
					[ROW_DEPOSIT_RETAINED]
					[COL_AMOUNT]
					+= payment.depositRetained;
			}
		}
	}

	// TOTAL
	double totalCollected =
		monthlyReport[ROW_COLLECTED][COL_AMOUNT];

	double totalRefund =
		monthlyReport[ROW_REFUND][COL_AMOUNT];

	double totalDepositReturned =
		monthlyReport[ROW_DEPOSIT_RETURNED][COL_AMOUNT];

	double totalDepositRetained =
		monthlyReport[ROW_DEPOSIT_RETAINED][COL_AMOUNT];

	double netAmount =
		totalCollected
		- totalRefund
		- totalDepositReturned;

	// DISPLAY
	clear_screen();

	print_header("Monthly Payment Report");
	empty_line();

	print_table_row(
		format(
			"{:<23}: {:02}/{}",
			"|Report Month",
			reportMonth,
			reportYear
		)
	);

	empty_line();
	print_divider();

	print_table_row(
		format(
			"|{:<27}{:<15}{:<20}",
			"Category",
			"Transactions",
			"Amount"
		)
	);

	print_divider();

	print_table_row(
		format(
			"|{:<27}{:<15.0f}RM{:>10.2f}",
			"Payment Collected",
			monthlyReport[ROW_COLLECTED][COL_COUNT],
			monthlyReport[ROW_COLLECTED][COL_AMOUNT]
		)
	);

	print_table_row(
		format(
			"|{:<27}{:<15.0f}RM{:>10.2f}",
			"Refund",
			monthlyReport[ROW_REFUND][COL_COUNT],
			monthlyReport[ROW_REFUND][COL_AMOUNT]
		)
	);

	print_table_row(
		format(
			"|{:<27}{:<15.0f}RM{:>10.2f}",
			"Deposit Returned",
			monthlyReport[ROW_DEPOSIT_RETURNED][COL_COUNT],
			monthlyReport[ROW_DEPOSIT_RETURNED][COL_AMOUNT]
		)
	);

	print_table_row(
		format(
			"|{:<27}{:<15.0f}RM{:>10.2f}",
			"Deposit Retained",
			monthlyReport[ROW_DEPOSIT_RETAINED][COL_COUNT],
			monthlyReport[ROW_DEPOSIT_RETAINED][COL_AMOUNT]
		)
	);

	print_divider();
	empty_line();

	print_table_row(
		format(
			"{:<27}: RM{:>10.2f}",
			"|Total Collected",
			totalCollected
		)
	);

	print_table_row(
		format(
			"{:<27}: RM{:>10.2f} (-)",
			"|Total Refund",
			totalRefund
		)
	);

	print_table_row(
		format(
			"{:<27}: RM{:>10.2f} (-)",
			"|Deposit Returned",
			totalDepositReturned
		)
	);

	print_table_row(
		format(
			"{:<27}: RM{:>10.2f}",
			"|Deposit Retained",
			totalDepositRetained
		)
	);

	print_divider();

	print_table_row(
		format(
			"{:<27}: RM{:>10.2f}",
			"|Net Amount",
			netAmount
		)
	);

	empty_line();

	print_divider_with_space(false);

	cout << "Press any key to continue...";
	_getch();
}

void generate_daily_report()
{
	clear_screen();

	print_header("Daily Payment Report");
	empty_line();

	string reportDate;

	// ask report date
	while (true)
	{
		cout << "Enter Report Date (DD/MM/YYYY) [0 to back]: ";
		getline(cin, reportDate);

		// back
		if (reportDate == "0")
		{
			return;
		}

		// validate date format
		if (!DateFormat(reportDate))
		{
			cout << "Invalid date format. Please use DD/MM/YYYY."
				<< endl;

			continue;
		}

		break;
	}

	// 2D ARRAY
	//
	// Column 0 = number of transactions
	// Column 1 = total amount
	//
	// Row 0 = collected payment
	// Row 1 = refund
	// Row 2 = deposit returned
	// Row 3 = deposit retained

	double dailyReport[DAILY_REPORT_ROWS][DAILY_REPORT_COLS] = {};

	// process all payment records
	for (Payment& payment : payments)
	{
		// only process payment records belonging to selected date
		if (payment.paymentDate != reportDate)
		{
			continue;
		}

		// --------------------------
		// PAYMENT COLLECTED
		// --------------------------

		if (payment.paymentStatus == "Paid" ||
			payment.paymentStatus == "Completed" ||
			payment.paymentStatus == "Refunded")
		{
			// increase transaction count
			dailyReport[ROW_COLLECTED][COL_COUNT]++;

			// add amount collected
			dailyReport[ROW_COLLECTED][COL_AMOUNT]
				+= payment.amountPaid;
		}

		// --------------------------
		// REFUND
		// --------------------------

		if (payment.paymentStatus == "Refunded")
		{
			dailyReport[ROW_REFUND][COL_COUNT]++;

			dailyReport[ROW_REFUND][COL_AMOUNT]
				+= payment.refundAmount;
		}

		// --------------------------
		// DEPOSIT SETTLEMENT
		// --------------------------

		if (payment.paymentStatus == "Completed")
		{
			if (payment.depositReturned > 0)
			{
				dailyReport[ROW_DEPOSIT_RETURNED][COL_COUNT]++;

				dailyReport[ROW_DEPOSIT_RETURNED][COL_AMOUNT]
					+= payment.depositReturned;
			}

			if (payment.depositRetained > 0)
			{
				dailyReport[ROW_DEPOSIT_RETAINED][COL_COUNT]++;

				dailyReport[ROW_DEPOSIT_RETAINED][COL_AMOUNT]
					+= payment.depositRetained;
			}
		}
	}

	// calculate totals
	double totalCollected =
		dailyReport[ROW_COLLECTED][COL_AMOUNT];

	double totalRefund =
		dailyReport[ROW_REFUND][COL_AMOUNT];

	double depositReturned =
		dailyReport[ROW_DEPOSIT_RETURNED][COL_AMOUNT];

	double depositRetained =
		dailyReport[ROW_DEPOSIT_RETAINED][COL_AMOUNT];

	double netAmount =
		totalCollected
		- totalRefund
		- depositReturned;

	// display report
	clear_screen();

	print_header("Daily Payment Report");

	empty_line();

	print_table_row(
		format("{:<23}: {}",
			"|Report Date",
			reportDate)
	);

	empty_line();

	print_divider();

	// table header
	print_table_row(
		format("|{:<27}{:<15}{:<20}",
			"Category",
			"Transactions",
			"Amount")
	);

	print_divider();

	// payment collected
	print_table_row(
		format("|{:<27}{:<15.0f}RM{:>10.2f}",
			"Payment Collected",
			dailyReport[ROW_COLLECTED][COL_COUNT],
			dailyReport[ROW_COLLECTED][COL_AMOUNT])
	);

	// refund
	print_table_row(
		format("|{:<27}{:<15.0f}RM{:>10.2f}",
			"Refund",
			dailyReport[ROW_REFUND][COL_COUNT],
			dailyReport[ROW_REFUND][COL_AMOUNT])
	);

	// deposit returned
	print_table_row(
		format("|{:<27}{:<15.0f}RM{:>10.2f}",
			"Deposit Returned",
			dailyReport[ROW_DEPOSIT_RETURNED][COL_COUNT],
			dailyReport[ROW_DEPOSIT_RETURNED][COL_AMOUNT])
	);

	// deposit retained
	print_table_row(
		format("|{:<27}{:<15.0f}RM{:>10.2f}",
			"Deposit Retained",
			dailyReport[ROW_DEPOSIT_RETAINED][COL_COUNT],
			dailyReport[ROW_DEPOSIT_RETAINED][COL_AMOUNT])
	);

	print_divider();

	empty_line();

	// totals
	print_table_row(
		format("{:<27}: RM{:>10.2f}",
			"|Total Collected",
			totalCollected)
	);

	print_table_row(
		format("{:<27}: RM{:>10.2f} (-)",
			"|Total Refund",
			totalRefund)
	);

	print_table_row(
		format("{:<27}: RM{:>10.2f} (-)",
			"|Deposit Returned",
			depositReturned)
	);

	print_divider();

	print_table_row(
		format("{:<27}: RM{:>10.2f}",
			"|Net Amount",
			netAmount)
	);

	empty_line();

	print_divider_with_space(false);

	cout << "Press any key to continue...";
	_getch();
}

void generate_report_menu()
{
	int choice;

	do
	{
		clear_screen();

		print_header("Generate Report");

		empty_line();

		print_table_row("|  [1] Daily Report");
		print_table_row("|  [2] Monthly Report");
		print_table_row("|  [3] Yearly Report");

		empty_line();

		print_table_row("|  [0] Back");

		empty_line();

		print_divider_with_space(false);

		choice = get_menu_choice(3);

		switch (choice)
		{
		case 1:
			generate_daily_report();
			break;

		case 2:
			 generate_monthly_report();
			break;

		case 3:
			 generate_yearly_report();
			break;

		default:
			return;
		}

	} while (choice != 0);
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
			generate_report_menu();
			break;
		default:
			// means the choice = 0, back
			return;
		}
	} while (choice != 0);
}