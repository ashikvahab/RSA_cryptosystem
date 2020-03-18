#include <iostream>
#include <string>
#include<algorithm>
#include<cmath>

using namespace std;

//function declarations
bool isProbablePrime(int, int);
bool millerRabin(int, int);
unsigned long long gcd(unsigned long long, unsigned long long);
unsigned long long modInverse(unsigned long long, unsigned long long);
unsigned long long encrypt(string, unsigned long long, int, unsigned long long);
unsigned long long modPower(unsigned long long, unsigned long long, unsigned long long);
string decrypt(unsigned long long, unsigned long long, int, unsigned long long);
unsigned long long bearcatii_to_num(string, int);
string num_to_bearcatii(unsigned long long, int);
//end of funtion declaration

int main() {
	unsigned int p, q;
	bool check = false; 
	while (!check) {
		p = rand() % 9000 + 1000; // randomly choose a number (p) between 1000 to 9999
		check = isProbablePrime(p, 10); //calling miller rabin to check if the number is prime or not
	}
	check = false;
	while (!check) {
		q = rand() % 9000 + 1000; // randomly choose a number (q) between 1000 to 9999
		check = isProbablePrime(q, 10); //calling miller rabin to check if the number is prime or not
	}
	cout << "p is " << p << endl << "q is " << q << endl;

	unsigned long long n, e;
	unsigned long long phi = (p - 1) * (q - 1);
	n = p * q;
	cout << "n is " << n << endl << "phi is " << phi << endl;
	
	/* In order to find the decrypt key 
	Instead of using extended gcd we have implemented euclid gcd followed by modInverse function*/
	while (true) { // asking the user to enter the public key e until gcd(e,phi) = 1
		cout << "Enter a public key e ( sample prime number = 45971 )" << endl;
		cin >> e;
		if (gcd(e, phi) == 1) {  //calling euclid gcd
			cout << "GCD is 1" << endl;
			break;
		}
		else {
			cout << "Try again" << endl;
		}
	}
	unsigned long long d = modInverse(e, phi); //(d = (e ^ -1) mod(phi)).d is the decrypt key.
	cout << "d is " << d << endl << "e is " << e << endl;
	
	cout << "Enter the string to be encrypted in capital letter " << endl;
	string message;
	cin >> message;
	unsigned long long encrypted_message = encrypt(message, e, 27, n); // converting message to number in base 27
	string decrypted_string = decrypt(encrypted_message, d, 27, n ); 
	
	cout << "Press any number to exit " << endl;
	int pause;
	cin >> pause;
	return 0;
}

/*to check the probability of number being prime.
this function in turn calls miller rabin to check the primality.
this function returns true if the number is prime
*/
bool isProbablePrime(int number, int iteration) {
	//checking base cases
	if (number == 0 || number == 1) { return false; }
	else if (number == 2) { return true; }
	else if (number % 2 == 0) { return false; }

	int d = number - 1;
	while (d % 2 == 0) {
		d /= 2;
	}
	for (int i = 0; i < iteration; i++)
		if (!millerRabin(d, number))  //calling miller rabin
			return false;

	return true;
}

//this function checks if the number is prime using miller rabin test
bool millerRabin(int d, int number) {

	int a = 2 + rand() % (number - 4);
	int x = modPower(a, d, number);

	if (x == 1 || x == number - 1) { return true; }

	while (d != number - 1) {
		x = (x * x) % number;
		d *= 2;
		if (x == 1)      return false;
		if (x == number - 1)    return true;
	}

	return false;

}

/*to check whether gcd (e, phi) = 1
euclid gcd is implemented*/
unsigned long long gcd(unsigned long long n1, unsigned long long n2) {
	unsigned long long rem;
	rem = n1 % n2;
	while (rem) {
		n1 = n2;
		n2 = rem;
		rem = n1 % n2;
	}

	return n2;
}

/*function to find the modular inverse multiplicative calculation*/
unsigned long long modInverse(unsigned long long e, unsigned long long phi) {
	e = e % phi;
	for (unsigned long long x = 1; x < phi; x++)
		if ((e * x) % phi == 1) { return x; }
}

// function to encrypt the message
unsigned long long encrypt(string message, unsigned long long e, int base, unsigned long long n) {
	unsigned long long dec = bearcatii_to_num(message, base); 
	if (dec > n) { 
		cout << "converted text is greater than n. Exiting." << endl;
		exit(0); 
	}
	unsigned long long encrypted_message = modPower(dec, e, n);

	string cyberText = num_to_bearcatii(encrypted_message, base);
	cout << "encrypted message " << cyberText << endl;

	return encrypted_message;
}

// function to find modular exponentiation
unsigned long long modPower(unsigned long long x, unsigned long long y, unsigned long long p)
{
	unsigned long long res = 1;      // Initialize result 

	x = x % p;  // Update x if it is more than or  
				// equal to p 

	while (y > 0)
	{
		// If y is odd, multiply x with result 
		if (y & 1)
			res = (res * x) % p;

		// y must be even now 
		y = y >> 1; // y = y/2 
		x = (x * x) % p;
	}
	return res;
}

//function to decrypt the converted number back to text 
string decrypt(unsigned long long encrypted_message, unsigned long long d, int base, unsigned long long n)
{
	unsigned long long enc2 = modPower(encrypted_message, d, n);
	
	string decrypted_message = num_to_bearcatii(enc2, base);

	cout << "decrypted message is " << decrypted_message << endl;

	return decrypted_message;

}

//converting the text to number in BEARCATII and converting to base 27
unsigned long long bearcatii_to_num(string message, int base) {
	int newMessage, cnt = 0;
	unsigned long long dec = 0;
	for (int i = message.length() - 1; i >= 0; i--) {
		int alphabet = message.at(i);
		if (alphabet == 32) { /* Convert space to 0*/
			newMessage = 0;
		}
		else if (alphabet >= 65 && alphabet <= 90) { /*convert capital letters to numbers from 1-26*/
			newMessage = alphabet - 'A' + 1;
			//cout << "new message " << newMessage << endl;
			//cout << "alphabet " << alphabet << endl;
		}
		dec += newMessage * pow(base, cnt);
		//cout << "dec " << dec << endl;
		cnt++;
	}

	cout << "BEARCATII in base 27 " << dec << endl;

	return dec;
}

//function to convert encrypted number value to BEARCATII
string num_to_bearcatii(unsigned long long number, int base) 
{
	int bearcat = 0;
	string decrypted_message;

	while (number > 0) {
		int q = number / base;
		int r = number % base;

		bearcat = r + 'A' - 1 ;
		decrypted_message = (char)bearcat + decrypted_message;

		number = q;

	}

	return decrypted_message;
	 
}

