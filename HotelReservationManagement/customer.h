#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "models.h"

using namespace std;

/*
	pass in the customer id to get the customer obj
*/
Customer* get_customer_by_id(string customerID) {
	for (Customer &customer : customers) {
		if (customer.customerID == customerID)
		{
			return &customer;
		}
	}
	return nullptr;
}