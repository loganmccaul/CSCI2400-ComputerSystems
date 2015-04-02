
#include <cctype>          // Provides toupper
#include <iostream>        // Provides cout and cin
#include <cstdlib>         // Provides EXIT_SUCCESS
#include <string>
using namespace std;


// PROTOTYPES for functions used by this test program:
void d2b(ostream& outs, unsigned int n);

int main()
{
  int i2;
  cout << "Enter an integer: ";
  cin >> i2;
  d2b(cout, i2);

  return (EXIT_SUCCESS);
}

// i is the offset
// n is the asterisks
void d2b(ostream& outs, unsigned int n)
{
if(n != 0){
	d2b(outs, n/2);
	outs << n%2;
}	
}


