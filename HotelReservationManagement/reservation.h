#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "models.h"

using namespace std;

/*
	search the list and return the reservation obj pointer
*/
Reservation* get_reservation_by_id(string reservationID) {
	for (Reservation& reservation : reservations)
	{
		if (reservation.reservationID == reservationID) {
			return &reservation;
		}
	}
	return nullptr;
}