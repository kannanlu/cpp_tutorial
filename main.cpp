#include <iostream>

int main(int argc, char *argv[])
{
    //////////////////////////////////////////////////////////////
    //  variable types:
    //  int i = 1;
    //  float a = 3.25;
    //  double b = 3.25;
    //  char x = 'a';    this is a character
    // std::string expr = "some string" ; need to include <string> for c++ >11
    // bool flag = true ;  this is a boolean variable
    // int a[2] , double b[3]  ,  these are arrays
    // int a[2][2] , double b[5][6] , these are matrices (2D arrays)

    /////////////////////////////////////////////////////////////////
    // control of flow
    //  logic control:
    //  if (condition1 )
    // { statement1; }
    // else
    // { stateent2; }

    //  loop
    // while (condition) {
    //  statement; }

    // or
    // for (int ii =0 ; ii<10; ii++) {
    // statement; }

    //////////////////////////////////////////////////////////////////
    // pointers (some exposure to memory in C++)
    // for an integer variable x, &x is the location in memory or the address
    // we thus can define a pointer to this: int* p_x; which stores the address of the integer
    // dynamically alocation of memory for arrays:
    // double * x;  an address to store double
    // x = new double [10]; this address points to the first element of a 10-element array
    // delete[] x; after usage of this array, delete this to release memory

    // double ** x; two addresses to store a 2D array of double, say 3x4 below
    // x = new double * [3]; 3 rows
    // for (int ii =0; ii<3, ii++) { x[ii] = new double [4]; }   4 columns
    // to delete, you need to delete in two dimensions
    // for (int ii = 0; ii<3; ii++) {delete[] x[ii]; }
    // delete[] x;

    // other important usage of pointers is to overcome the scope of variables,
    //  allowing certain variable to be accessed in different scopes and functions since it's referencing an address
    // a easier way is to use referencing variables,
    // e.g. void function(double& x, double& y, double z) { x = sqrt(z) ; y = pow(z,2); }
    // this way one can still use x and y as the above function changes the value in the addresses

    // to learn: object-oritented programming
    // to-do: io with file, linear system class, define matrices, vector, with methods of matrix inversion, newton method to solve linear systems
    // and newton-raphson method to solve nonlinear systems, use template to extend the linear system module to complex domain

    char x = 'a';
    std::cout << "the character is " << x << "\n";
    return 0;
}