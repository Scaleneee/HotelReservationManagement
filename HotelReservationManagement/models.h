#pragma once
#include <string>
#include <vector>

using namespace std;

const double DEPOSIT_RATE = 0.20;
const int POINTS_PER_RM = 50;

struct Customer {
    string customerID;
    string name;
    string contact;
    string gender;
    string icNumber;
    string nationality;
    string birthday;
    string registerDate;
    string accountStatus;
};

struct Membership {
    string membershipID;
    string customerID;
    string level;
    string registerDate;
    string expiryDate;
    int points;
    double discountRate;
    string status;
};

struct Room {
    int roomNumber;
    string roomType;
    string description;
    int capacity;
    double pricePerNight;
    string roomStatus;
};

struct Reservation {
    string reservationID;
    string customerID;
    int roomNumber;
    string checkInDate;
    string checkOutDate;
    string actualCheckInTime;
    string actualCheckOutTime;
    int numberOfGuests;
    int numberOfNights;
    double roomPrice;
    // checked-in, checked-out, cancelled
    string reservationStatus;
    // by default, its will be null
    string cancellationReason;
};

struct Payment {
    string paymentID;
    string reservationID;
    string paymentDate;
    string paymentMethod;
    string paymentStatus;
    double roomFee;
    double membershipDiscount;
    double depositAmount;
    double additionalCharge;
    double damageCharge;
    double totalAmount;
    double amountPaid;
    double change;
    double refundAmount;
    double depositReturned;
    double depositRetained;
};

// extern vector list use to store the record
// actual global variables are declared in the main.cpp
extern vector<Customer> customers;
extern vector<Membership> memberships;
extern vector<Room> rooms;
extern vector<Reservation> reservations;
extern vector<Payment> payments;