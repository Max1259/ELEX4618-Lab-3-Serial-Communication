#include "stdafx.h"

#include <string>
#include <iostream>
#include <thread>

#include "Client.h"
#include "Server.h"
#include "Serial.h" // Must include Windows.h after Winsock2.h, so Serial must include after Client/Server
#include "CControl.h"
// OpenCV Include
#include "opencv.hpp"

// OpenCV Library
#pragma comment(lib,".\\opencv\\lib\\opencv_world310d.lib")

using namespace std;

enum { DIGITAL = 0, ANALOG, SERVO };

#define MIN_SERVO 1
#define MAX_SERVO 180

void print_menu();
void servo();

int main() {

	CControl ctrl;
	char command = 0;

	do {

		ctrl.mycom();
		print_menu();

		cin >> command;

		switch (toupper(command)) {

			case 'A':
				ctrl.get_analog();
				break;

			case 'B':
				ctrl.get_button();
				break;

			case 'D':
				ctrl.set_digital();
				break;

			case 'S':
				servo();
				break;

			default:
				cout << "Command unknown. Try again." << endl;
		}
		
	} while (command != 'Q' && command != 'q');

	return 0;
}

void print_menu()
{
	cout << "****************************************" << endl << "ELEX 4618 Lab 3: Embedded Control" << endl << "Max Horie, A01010912" << endl << "****************************************" << endl;
	cout << "(A)nalog test" << endl << "(B)utton debounce test" << endl << "(D)igital IO test" << endl << "(S)ervo Test" << endl << "(Q)uit" << endl;
}

void servo()
{
	int servo_channel;

	cout << "Enter servo channel: ";
	cin >> servo_channel;

	while (1) {

		set_data(SERVO, servo_channel, MIN_SERVO);
		cout << "SERVO TEST: CH " << servo_channel << " 0 DEGREES" << endl;
		Sleep(800);
		set_data(SERVO, servo_channel, MAX_SERVO);
		cout << "SERVO TEST: CH " << servo_channel << " 180 DEGREES" << endl;
		Sleep(800);
	}
}