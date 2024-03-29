// StegProj_COSC2030.cpp : Defines the entry point for the console application.
//
// As it currently stands the maximum length of a message is 4 characters

#include "stdafx.h"
#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include<bitset>
//#include <stdlib.h> 

using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::string;
using std::to_string;
using std::getline;
using std::hex;
using std::stoll;
using std::bitset;
using std::vector;

void readData(string& output, ifstream& input); //reads pixel data from file, one line at a time
void encrypt(ifstream& input); //encrypts data into a new pixel file
void decrypt(ifstream& input); //decrypts data from pixel file
string char_to_bin(char ch);
char bin_to_char(string group);

int main()
{
	int ready = 0; //Used to ensure correct user unput when choosing whether to encrypt or decrypt
	char input;

	while (!ready) {
		cout << "Are you encrypting? (Y/N)" << endl;
		cin >> input;
		if (input == 'Y' || input == 'N') {
			ready = 1;
		}
		else {
			cout << "Please input either a Y or an N." << endl;
		}
	}
	string inBuf;
	string inputFile; //name of file to be read in either case
	ifstream file;

	cout << "Please enter a filename: ";
	cin >> inputFile;
	file.open(inputFile);

	if (!file) {
		cout << "Unable to find or open input: " << inBuf << endl; //checks if file was input correctly
		file.close();
	}
	else {
		if (input == 'Y') {
			encrypt(file); //runs encryption algorithm below
		}
		else {
			decrypt(file); //runs decryption algorithm
		}
	}
	file.close();
    return 0;
}

void readData(string& output, ifstream& input) {
	string inBuf;
	if (input >> inBuf) {
		output = inBuf;
	}
	else {
		input.clear();
		input.ignore();
	}
}

void encrypt(ifstream& input) {
	string line, message, length, base; //line is each line of the pixel data, message is the coded message, base is used to ensure the pixel is read in hex
	long long int value; //long long int variable to store the converted pixel data, I know its just a long in the example, but it was complaining that a long wasn't big enough
	int msglng = 0, imglng = 0; //used to determine the length of the message and the length of the pixel data file respectively
	ofstream outFile;
	vector<long long int> vector; //vector to store pixel data

	while (!input.eof()) {
		readData(line, input);
		value = stoll(line, nullptr, 16); //convers the hex value of the pixel to a long int
		vector.push_back(value); //stores the inputs in a vector
		imglng++; //incriments image count used later
	}
	if (imglng > 37) {
		imglng = 32;
	}
	else {
		imglng -= 5;
	}

	while (msglng > imglng || msglng < 1)
	{
		cout << "enter the length of the message to be encrypted (between 1 and " << imglng / 8 << "), followed by the message." << endl;
		cin >> msglng;
		msglng *= 8;
		if (msglng > imglng) {
			cout << "length of message is to long for designated image file." << endl;
		}
	}
	cin >> message;

	length = bitset<5>(msglng - 1).to_string(); //converts length of the message to a binary string, the 5 limits it to 32 bits or 4 characters
	cout << length << endl;  //Outputs code for message length, remove after debugging

	int x;
	for (int i = 0; i < 5; i++) { //encodes length of message in first 5 pixels
		x = (int)length.at(i) - 48; //The minus 48 is to account for ascii conversion (look up char to int)
		if (x == 1) {
			//vector[i] = vector[i] | (1 << 0); Not working for unknown reasons
			if (vector[i] % 2 == 0) {
				vector[i] = vector[i] + 1;

			}
		}
		else {
			//vector[i] = vector[i] | ~(1 << 0);
			if (vector[i] % 2 == 1) {
				vector[i] = vector[i] - 1;
			}
		}
	}
string msgdata = "";
for (int i = 0; i < message.length(); i++) { //converts message into binary
	msgdata += char_to_bin(message[i]);
}
cout << msgdata << endl;  //Outputs code for message, remove after debugging
for (int i = 0; i < msglng; i++) { //encodes message in remaining pixels
	x = (int)msgdata[i] - 48;
	if (x == 1) {
		//vector[i] = vector[i] | (1 << 0);
		if (vector[i+5] % 2 == 0) {
			vector[i+5] = vector[i+5] + 1;
		}
	}
	else {
		//vector[i] = vector[i] | ~(1 << 0);
		if (vector[i+5] % 2 == 1) {
			vector[i+5] = vector[i+5] - 1;
		}
	}
}

	outFile.open("codedmsg.txt", ofstream::app);

	for (long long int i : vector) { //prints the encoded file to a new text file called codedmsg.txt
		outFile << hex << i << endl;
	}
	outFile.close();
}

void decrypt(ifstream& input) {
	string line, message;
	long value;

	while (!input.eof()) {
		readData(line, input);
		value = stol(line, nullptr, 16);

	}
}

string char_to_bin(char ch) //used in encryption
{
	bitset<8> temp(ch);
	return temp.to_string();
}

char bin_to_char(string group) //used in decryption
{
	bitset<8> temp(group);
	return temp.to_ulong();
}