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
	//calculate();

	// application entrance
	//room_availability_menu();
	//load_all_data_from_file();
	//payment_menu();
	customerMembershipMenu();

	return 0;
}