#pragma once
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <algorithm>

using namespace std;

/*
	ALL REUSABLE UI
*/

const int HEADER_WIDTH = 80;
const int CONTENT_WIDTH = HEADER_WIDTH - 2; // - two '|'

// set light or dark theme
void set_theme(string theme) {
	transform(theme.begin(), theme.end(), theme.begin(), ::toupper);
	if (theme == "LIGHT")
	{
		system("color F0");
	}
}

void empty_line() {
	cout << "|" << setw(79) << right << "|" << endl;
}

void print_divider() {
	for (int i = 0; i < HEADER_WIDTH; i++)
	{
		cout << '-';
	}
	cout << endl;
}

void print_divider_with_space(bool bottom_line) {
	empty_line();
	print_divider();
	if (bottom_line)
	{
		empty_line();
	}
	else
	{
		cout << endl;
	}
}

void print_line() {
	for (int i = 0; i < HEADER_WIDTH; i++)
	{
		cout << '=';
	}
	// next line
	cout << endl;
}

void print_header(string header) {
	// calculate padding, to make it align center
	int left_padding = (CONTENT_WIDTH - header.length()) / 2;
	int right_padding = CONTENT_WIDTH - header.length() - left_padding;

	print_line();
	// |       |
	cout << setw(HEADER_WIDTH - 1) << left << '|' 
		<< '|' << endl;

	// | header |
	cout << '|' 
		<< string(left_padding, ' ') 
		<< header
		<< string(right_padding, ' ')
		<< '|' << endl;

	// |       |
	cout << setw(HEADER_WIDTH - 1) << left << '|' 
		<< '|' << endl;
	print_line();
}

int get_menu_choice(int max) {
	// store the input
	int choice;

	while (true) {
		// get input
		cout << "Enter your choice[0 - " << max << "]: ";
		cin >> choice;

		// if not a number
		if (cin.fail())
		{
			// clear cin
			cin.clear();
			cin.ignore(1000, '\n');

			// error msg
			cout << "Invalid input. Please enter a number. " << endl;
			continue;
		}

		// clear cin
		cin.ignore(1000, '\n');

		// check range
		// min range must be 0
		if (choice < 0 || choice > max)
		{
			cout << "Invalid input. Please enter " << 0 << " - " << max << "." << endl;
			continue;
		}

		// valid choice
		return choice;
	}
}

void clear_screen() {
	system("cls");
}

