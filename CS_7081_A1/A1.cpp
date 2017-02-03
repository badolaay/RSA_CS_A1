#include <iostream>
#include <string>
#include <math.h>

using namespace std;

char textArray[] = { ' ', 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };

/*Method to trim white space from string -- Copied from StackOverflow*/
string trim(string const& str)
{
	if (str.empty())
		return str;

	std::size_t firstScan = str.find_first_not_of(' ');
	std::size_t first = firstScan == std::string::npos ? str.length() : firstScan;
	std::size_t last = str.find_last_not_of(' ');
	return str.substr(first, last - first + 1);
}

string getOriginalMessage(string msgInBase27) {
	string message = "";
	msgInBase27 = trim(msgInBase27);
	string delimiter = " ";
	size_t pos = 0;
	string token;

	while ((pos = msgInBase27.find(delimiter)) != std::string::npos) {
		token = msgInBase27.substr(0, pos);
		message += textArray[stoi(token)];
		msgInBase27.erase(0, pos + delimiter.length());
	}

	return 	message += textArray[stoi(msgInBase27)];;
}

/*Method to convert decimal to another base*/
string convertBases(unsigned long number, unsigned long base) {
	if (number == 0) {

		return "";
	}
	else {
		return 	convertBases(number / base, base) + " " + to_string(number%base);
	}
}

bool checkIfPrime(unsigned long int p) {
	for (int i = 2; i <= sqrt(p); i++) {
		if (p%i == 0) {
			return false;
		}
	}
	return true;
}
unsigned long long powersMod(unsigned long long number, unsigned long int power, unsigned long modulus) {
	if (power == 0) { return 1; }
	if (power == 1)
	{
		return number%modulus;
	}
	else
	{
		if (power % 2 == 0)
		{
			return	powersMod((number%modulus)*(number%modulus), power / 2, modulus) % modulus;
		}

		return ((number%modulus)* powersMod((number%modulus)*(number%modulus), (power - 1) / 2, modulus)) % modulus;
	}
}
unsigned long long powers(unsigned long long num1, unsigned long int num2) {
	if (num2 == 0) { return 1; }
	if (num2 == 1)
	{
		return num1;
	}
	else
	{
		if (num2 % 2 == 0)
		{
			return	powers(num1*num1, num2 / 2);
		}

		return (num1*powers(num1*num1, (num2 - 1) / 2));
	}
}


unsigned long int gcdEuclid(unsigned long int num1, unsigned long int num2) {

	if (num2 == 0)
	{
		return num1;
	}
	else
	{
		unsigned long int r = num1 % num2;
		gcdEuclid(num2, r);
	}
}


long int gcdEuclidExtended(unsigned long int a, unsigned long int b, long int *d, long int *y)
{
	// Base Case
	if (a == 0)
	{
		*d = 0;
		*y = 1;
		return b;
	}

	long int x1, y1; // To store results of recursive call
	long int gcd1 = gcdEuclidExtended(b%a, a, &x1, &y1);

	*d = y1 - (b / a) * x1;
	*y = x1;
	return gcd1;
}


int main()
{

	//Initialising a text array for message conversion to decimal
	string message;
	cout << "Enter message M in Upper Case Only. Example: TEST or MESSAGE \n";
	cout << "The lowercase letters will be converted to uppercase and everything else will be treated as space\n";
	cin >> message;
	//Breakdown each part of message for changing bases
	int msgLength = message.size();

	for (char& c : message) {
		if (c > 96 && c < 123) { c = toupper(c); }
		else if (c < 65 || c>90) { c = ' '; }
	}
	cout << "Message to encrypt :" << message << "\n";

	//Initialise array to hold the letter to digit conversion of message

	/*This contains values in BEARCATII*/
	int * msgDecimal = new int[msgLength];
	unsigned long msgDecimalVal = 0;
	for (int i = 0; i < msgLength; i++) {
		for (int j = 0; j < 27; j++) {
			if (textArray[j] == message[i])
				msgDecimal[i] = j;
		}

	}

	/*Conversion from BEARCATII to Decimal*/
	for (int i = 0; i < msgLength; i++) {
		msgDecimalVal += powers(27, msgLength - i - 1) * msgDecimal[i];
	}

	delete[] msgDecimal;

	cout << "Your Message in Decimal is " << msgDecimalVal << "\n";
	unsigned long p, q, totn, n, e;
	long int d, y;
	n = 0;
	while (n <= msgDecimalVal) {
		cout << "Enter first prime number: p \n";
		cin >> p;
		while (!checkIfPrime(p)) { cout << "p is not prime Please enter again \n"; cin >> p; }
		cout << "Enter second prime number: q \n";
		cin >> q;
		while (!checkIfPrime(q)) { cout << "q is not prime Please enter again \n"; cin >> q; }
		n = p * q;
		if (n <= msgDecimalVal) {
			cout << "n = p*q is not greater than the mesage in decimal!! \n";
		}
	}
	totn = (p - 1)*(q - 1);
	cout << "Product (n=p*q) = " << n;
	cout << "\nTotient(n) = " << totn;
	cout << "\nEnter E (realtively prime to Totient(n)) \n";
	cin >> e;
	if (gcdEuclid(e, totn) != 1) {
		cout << "\n E is not realtively prime to Totient(n) \n";
		cout << "\n Please enter E again \n";
		cin >> e;
	}
	else {
		cout << "Great job finding e";
	}


	unsigned long int g = gcdEuclidExtended(totn, e, &d, &y);
	printf("gcdEuclid(%lu, %lu) = %lu, d = %ld, y = %ld \n",
		totn, e, g, d, y);

	unsigned long long cipher;
	long long t = powers(msgDecimalVal, e);
	cipher = powersMod(msgDecimalVal, e, n);

	printf("Cipher text is %lu \n", cipher);

	unsigned long int decrypt;

	while (y < 0) { y = totn + y; }
	decrypt = powersMod(cipher, y, n);

	printf("Decrypted decimal value of text is %lu", decrypt);

	string msgInBase27 = convertBases(decrypt, 27);
	cout << "\nMessage in base 27 : " << msgInBase27 << "\n";
	cout << "The original message was :" << getOriginalMessage(msgInBase27);

	getchar();	getchar();
}