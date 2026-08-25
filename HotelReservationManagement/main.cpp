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

void main_menu()
{
	// set the theme to white bg and black fg
	system("color F0");

	int choice;

	do
	{
		clear_screen();

		print_header("Hotel Reservation Management System");

		empty_line();

		print_table_row("|  [1] Customer & Membership Management");
		print_table_row("|  [2] Room Management");
		print_table_row("|  [3] Reservation Management");
		print_table_row("|  [4] Payment & Reporting");

		empty_line();

		print_table_row("|  [0] Exit");

		empty_line();

		print_divider_with_space(false);

		choice = get_menu_choice(4);

		switch (choice)
		{
		case 1:
			clear_screen();
			customerMembershipMenu();
			break;

		case 2:
			clear_screen();
			room_availability_menu();
			break;

		case 3:
			clear_screen();
			reservationMenu();
			break;

		case 4:
			clear_screen();
			payment_menu();
			break;

		case 0:
			return;
		}

	} while (choice != 0);
}

int main()
{
	// load all saved data
	load_all_data_from_file();

	// run system
	main_menu();

	// save everything before exit
	save_customers_to_file();
	save_memberships_to_file();
	save_rooms_to_file();
	save_reservations_to_file();
	save_payments_to_file();

	clear_screen();

	print_header("Hotel Reservation Management System");

	empty_line();

	print_table_row("|Thank you for using the system.");

	empty_line();

	print_divider_with_space(false);

	return 0;
}