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