
#include <cctype>          // Provides toupper
#include <iostream>        // Provides cout and cin
#include <cstdlib>         // Provides EXIT_SUCCESS
#include <string>
using namespace std;


// PROTOTYPES for functions used by this test program:
void pattern(ostream& outs, unsigned int n, unsigned int i);

int main()
{
  int i2;
  cout << "Enter an integer: ";
  cin >> i2;
  pattern(cout, i2, 0);
  return (EXIT_SUCCESS);
}

// i is the offset
// n is the asterisks
void pattern(ostream& outs, unsigned int n, unsigned int i)
{
	if(n > 1)
	pattern(outs, n/2, i);
	for(unsigned int k=0; k < i; k++)
		outs << " ";
	for(unsigned int k=0; k < n; k++)
		outs << "*";
	outs << endl;
	if(n > 1)
	pattern(outs, n/2, i + n/2);
}

