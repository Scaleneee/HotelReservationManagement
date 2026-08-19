#include <iostream>
#include <vector>
#include "models.h"
#include "file_io.h"
#include "payment.h"
#include "customer.h "
#include "room.h"
#include "ui.h"

using namespace std;

// actual global vector list variables
vector<Customer> customers;
vector<Membership> memberships;
vector<Room> rooms;
vector<Reservation> reservations;
vector<Payment> payments;

/*void calculate() {
	for (Payment& payment : payments)
	{
		payment.totalAmount = payment.roomFee + payment.depositAmount + payment.additionalCharge - payment.membershipDiscount;
	}
	save_payments_to_file();
}*/

int main() {
	// set theme
	//set_theme("dark");

	// initialization
	// load all data from file to the list
	load_all_data_from_file();
	//calculate();

	// application entrance
	//payment_menu();
	customerMembershipMenu();
	//room_availability_menu();
	return 0;
}