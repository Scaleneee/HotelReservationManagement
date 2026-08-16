#include <iostream>
#include <vector>
#include "models.h"
#include "file_io.h"
#include "payment.h"
#include "ui.h"

using namespace std;

// actual global vector list variables
vector<Customer> customers;
vector<Membership> memberships;
vector<Room> rooms;
vector<Reservation> reservations;
vector<Payment> payments;

int main() {

	// initialization
	// load all data from file to the list
	load_all_data_from_file();

	// application entrance
	payment_menu();

	return 0;
}