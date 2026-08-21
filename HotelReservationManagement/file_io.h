#pragma once
#include <fstream>
#include "models.h"

using namespace std;

/*
	Customer File Operation
*/
// load all data from customer file to customer list
void load_customers_from_file() {
	ifstream inFile("customers.txt");

    // if the file can't be opened
    if (!inFile) {
        cout << "Error opening customers.txt." << endl;
        return;
    }

    // clear the list
    customers.clear();

    // customer variable to store the data read from the file
    Customer customer;

    while (getline(inFile, customer.customerID, '|')) {
        getline(inFile, customer.name, '|');
        getline(inFile, customer.contact, '|');
        getline(inFile, customer.gender, '|');
        getline(inFile, customer.icNumber, '|');
        getline(inFile, customer.nationality, '|');
        getline(inFile, customer.birthday, '|');
        getline(inFile, customer.registerDate, '|');
        getline(inFile, customer.accountStatus);

        customers.push_back(customer);
    }

    inFile.close();
}


// save all the customer data from list to file
void save_customers_to_file() {
    ofstream outFile("customers.txt");

    if (!outFile) {
        cout << "Error opening customers.txt." << endl;
        return;
    }

    for (Customer customer : customers) {
        outFile << customer.customerID << "|"
            << customer.name << "|"
            << customer.contact << "|"
            << customer.gender << "|"
            << customer.icNumber << "|"
            << customer.nationality << "|"
            << customer.birthday << "|"
            << customer.registerDate << "|"
            << customer.accountStatus << endl;
    }

    outFile.close();
}

/*
	Memberships File Operation
*/
// load all data from membership file to membership list
void load_memberships_from_file() {
    ifstream inFile("memberships.txt");

    if (!inFile) {
        cout << "Error opening memberships.txt." << endl;
        return;
    }

    memberships.clear();

    Membership membership;

    string points;
    string discountRate;

    while (getline(inFile, membership.membershipID, '|')) {

        // skip empty record
        if (membership.membershipID.empty()) {
            continue;
        }

        getline(inFile, membership.customerID, '|');
        getline(inFile, membership.level, '|');
        getline(inFile, membership.registerDate, '|');
        getline(inFile, membership.expiryDate, '|');
        getline(inFile, points, '|');
        getline(inFile, discountRate, '|');
        getline(inFile, membership.status);

        try {
            membership.points = stoi(points);
            membership.discountRate = stod(discountRate);

            memberships.push_back(membership);
        }
        catch (const invalid_argument&) {
            cout << "Invalid numeric data in membership: "
                << membership.membershipID << endl;
        }
        catch (const out_of_range&) {
            cout << "Numeric value out of range in membership: "
                << membership.membershipID << endl;
        }
    }

    inFile.close();
}

// save all the membership data from list to file
void save_memberships_to_file() {
    ofstream outFile("memberships.txt");

    if (!outFile) {
        cout << "Error opening memberships.txt." << endl;
        return;
    }

    for (Membership membership : memberships) {
        outFile << membership.membershipID << "|"
            << membership.customerID << "|"
            << membership.level << "|"
            << membership.registerDate << "|"
            << membership.expiryDate << "|"
            << membership.points << "|"
            << membership.discountRate << "|"
            << membership.status << endl;
    }

    outFile.close();
}

/*
	Payments File Operation
*/
// load data
void load_payments_from_file() {
    ifstream inFile("payments.txt");

    if (!inFile) {
        cout << "Error opening payments.txt." << endl;
        return;
    }

    payments.clear();

    Payment payment;

    string roomFee;
    string membershipDiscount;
    string depositAmount;
    string additionalCharge;
    string damageCharge;
    string totalAmount;
    string amountPaid;
    string change;
    string refundAmount;
    string depositReturned;
    string depositRetained;

    while (getline(inFile, payment.paymentID, '|')) {

        // skip empty record
        if (payment.paymentID.empty()) {
            continue;
        }

        getline(inFile, payment.reservationID, '|');
        getline(inFile, payment.paymentDate, '|');
        getline(inFile, payment.paymentMethod, '|');
        getline(inFile, payment.paymentStatus, '|');
        getline(inFile, roomFee, '|');
        getline(inFile, membershipDiscount, '|');
        getline(inFile, depositAmount, '|');
        getline(inFile, additionalCharge, '|');
        getline(inFile, damageCharge, '|');
        getline(inFile, totalAmount, '|');
        getline(inFile, amountPaid, '|');
        getline(inFile, change, '|');
        getline(inFile, refundAmount, '|');
        getline(inFile, depositReturned, '|');
        getline(inFile, depositRetained);

        try {
            payment.roomFee = stod(roomFee);
            payment.membershipDiscount = stod(membershipDiscount);
            payment.depositAmount = stod(depositAmount);
            payment.additionalCharge = stod(additionalCharge);
            payment.damageCharge = stod(damageCharge);
            payment.totalAmount = stod(totalAmount);
            payment.amountPaid = stod(amountPaid);
            payment.change = stod(change);
            payment.refundAmount = stod(refundAmount);
            payment.depositReturned = stod(depositReturned);
            payment.depositRetained = stod(depositRetained);

            payments.push_back(payment);
        }
        catch (const invalid_argument&) {
            cout << "Invalid numeric data in payment: "
                << payment.paymentID << endl;
        }
        catch (const out_of_range&) {
            cout << "Numeric value out of range in payment: "
                << payment.paymentID << endl;
        }
    }

    inFile.close();
}

// save data
void save_payments_to_file() {
    ofstream outFile("payments.txt");

    if (!outFile) {
        cout << "Error opening payments.txt." << endl;
        return;
    }

    for (Payment payment : payments) {
        outFile << payment.paymentID << "|"
            << payment.reservationID << "|"
            << payment.paymentDate << "|"
            << payment.paymentMethod << "|"
            << payment.paymentStatus << "|"
            << payment.roomFee << "|"
            << payment.membershipDiscount << "|"
            << payment.depositAmount << "|"
            << payment.additionalCharge << "|"
            << payment.damageCharge << "|"
            << payment.totalAmount << "|"
            << payment.amountPaid << "|"
            << payment.change << "|"
            << payment.refundAmount << "|"
            << payment.depositReturned << "|"
            << payment.depositRetained << endl;
    }

    outFile.close();
}

/*
	Reservation File Operation
*/
// load data
void load_reservations_from_file() {
    ifstream inFile("reservations.txt");

    if (!inFile) {
        cout << "Error opening reservations.txt." << endl;
        return;
    }

    reservations.clear();

    Reservation reservation;

    string roomNumber;
    string numberOfGuests;
    string numberOfNights;
    string roomPrice;

    while (getline(inFile, reservation.reservationID, '|')) {

        if (reservation.reservationID.empty()) {
            continue;
        }

        getline(inFile, reservation.customerID, '|');
        getline(inFile, roomNumber, '|');
        getline(inFile, reservation.checkInDate, '|');
        getline(inFile, reservation.checkOutDate, '|');
        getline(inFile, reservation.actualCheckInTime, '|');
        getline(inFile, reservation.actualCheckOutTime, '|');
        getline(inFile, numberOfGuests, '|');
        getline(inFile, numberOfNights, '|');
        getline(inFile, roomPrice, '|');
        getline(inFile, reservation.reservationStatus, '|');
        getline(inFile, reservation.cancellationReason);

        try {
            reservation.roomNumber = stoi(roomNumber);
            reservation.numberOfGuests = stoi(numberOfGuests);
            reservation.numberOfNights = stoi(numberOfNights);
            reservation.roomPrice = stod(roomPrice);

            reservations.push_back(reservation);
        }
        catch (const invalid_argument&) {
            cout << "Invalid numeric data in reservation: "
                << reservation.reservationID << endl;
        }
        catch (const out_of_range&) {
            cout << "Numeric value out of range in reservation: "
                << reservation.reservationID << endl;
        }
    }

    inFile.close();
}

// save data
void save_reservations_to_file() {
    ofstream outFile("reservations.txt");

    if (!outFile) {
        cout << "Error opening reservations.txt." << endl;
        return;
    }

    for (Reservation reservation : reservations) {
        outFile << reservation.reservationID << "|"
            << reservation.customerID << "|"
            << reservation.roomNumber << "|"
            << reservation.checkInDate << "|"
            << reservation.checkOutDate << "|"
            << reservation.actualCheckInTime << "|"
            << reservation.actualCheckOutTime << "|"
            << reservation.numberOfGuests << "|"
            << reservation.numberOfNights << "|"
            << reservation.roomPrice << "|"
            << reservation.reservationStatus << "|"
            << reservation.cancellationReason << endl;
    }

    outFile.close();
}

/*
	Rooms File Operation
*/
// load data
void load_rooms_from_file() {
    ifstream inFile("rooms.txt");

    if (!inFile) {
        cout << "Error opening rooms.txt." << endl;
        return;
    }

    rooms.clear();

    Room room;

    string roomNumber;
    string capacity;
    string pricePerNight;

    while (getline(inFile, roomNumber, '|')) {

        if (roomNumber.empty()) {
            continue;
        }

        getline(inFile, room.roomType, '|');
        getline(inFile, room.description, '|');
        getline(inFile, capacity, '|');
        getline(inFile, pricePerNight, '|');
        getline(inFile, room.roomStatus);

        try {
            room.roomNumber = stoi(roomNumber);
            room.capacity = stoi(capacity);
            room.pricePerNight = stod(pricePerNight);

            rooms.push_back(room);
        }
        catch (const invalid_argument&) {
            cout << "Invalid numeric data in room record." << endl;
        }
        catch (const out_of_range&) {
            cout << "Numeric value out of range in room record." << endl;
        }
    }

    inFile.close();
}

// save data
void save_rooms_to_file() {
    ofstream outFile("rooms.txt");

    if (!outFile) {
        cout << "Error opening rooms.txt." << endl;
        return;
    }

    for (Room room : rooms) {
        outFile << room.roomNumber << "|"
            << room.roomType << "|"
            << room.description << "|"
            << room.capacity << "|"
            << room.pricePerNight << "|"
            << room.roomStatus << endl;
    }

    outFile.close();
}

/*
    Load all data from the file to the list
*/
void load_all_data_from_file() {
    try {
        load_customers_from_file();
        load_memberships_from_file();
        load_payments_from_file();
        load_reservations_from_file();
        load_rooms_from_file();
    }
    catch (const exception& e) {
        cout << "Unexpected error while loading data: "
            << e.what() << endl;
    }
}