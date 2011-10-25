/*
  This code maps from the C++ code to the equivalent C code.
  Mapping of the following entities is covered:

  - classes                 - methods
  - this pointer            - member variables
  - constructors            - static methods
  - destructors             - static variables
*/

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef int BOOLEAN;
 

/*
  Structure A represents the class A. Only the non-static member
  variables are present in the structure
*/
struct A
{
  int m_x;
  int m_z;
};

/* Notice that g_y is not a part of struct A. Its a separate global variable. */
int g_y = 0;

/*
  Prototype for the NotifyEnd method. The C++ version of this method
  did not have any parameters but the C mapped function needs the this
  pointer to obtain the address of the object. Note that all non-static
  methods in the C++ code would map to a C function the additional this
  pointer as the first parameter.
*/
void NotifyEnd(A *this_ptr)
{
  printf("A::NotifyEnd...\n");
}

/*
  The constructor maps to function with the this pointer and the size of the
  structure as parameters. this_ptr passed to the constructor is NULL when
  the operator new is used to create the object. this_ptr contains a valid
  pointer if the memory for the object to be constructed is already
  allocated. (e.g. local variable or part of another structure.)
*/

A*
A_Constructor(A *this_ptr, int x)
{
  /*Check if memory has been allocated for struct A. */
  if (this_ptr == NULL)
  {
    /*Allocate memory of size A. */
    this_ptr = (A *) malloc(sizeof(A));
  }

  /* Once the memory has been allocated for A, initialise members of A. */
  if (this_ptr)
  {
    this_ptr->m_x = x;
    this_ptr->m_z = 0;
  }
  return this_ptr;
}
 
/*
  The following function is equivalent to a destructor. The this
  pointer and a dynamic flag are passed as the two parameters to
  this function. The dynamic flag is set to true if the object is
  being deleted using the delete operator.
*/

void
A_Destructor(A *this_ptr, BOOLEAN dynamic)
{
  NotifyEnd(this_ptr);
  /* If the memory was dynamically allocated for A, explicitly free it. */
  if (dynamic)
  {
    free(this_ptr);
  }
}
 
/*
  A pointer this is passed as first argument. All member variables
  in the code will be accessed through an indirecion from the this
  pointer. Notice that static variables are accessed directly as
  they do not belong to any instance.
*/
void
A_UpdateX(A *this_ptr, int newX)
{
  if (g_y != 0 && this_ptr->m_x < newX)
  {
    this_ptr->m_x = newX;
  }
}

/*
  Notice that this is not passed here. This is so because
  A_UpdateY is a static function. This function can only access
  other static functions and static or global variables. This
  function cannot access any member variables or methods of class A
  as a static function does not correspond to an instance.
*/
void
A_UpdateY(int newY)
{
  g_y = newY;
}
 

int
main(int argc, char **argv)
{
  /*
    Dynamically allocate memory by passing NULL in this arguement.
    Also initialize members of struct pointed to by pA.
  */
  A *pA = A_Constructor(NULL, 5);

  /* Define local variable a of type struct A. */
  A a;

  /*
    Initialize members of struct variable a. Note that the
    constructor is called with the address of the object as
    a has been pre-allocated on the stack.
  */
  A_Constructor(&a, 6);

  /*
    Method invocations in C++ are handled by calling the
    corresponding C functions with the object pointer.
  */
  A_UpdateX(pA, 8);
  A_UpdateX(&a, 9);

  /* UpdateY is a static method, so object pointer is not passed */
  A_UpdateY(1000);

  /*
    Delete memory pointed to by pA (explicit delete in
    original code).
  */
  A_Destructor(pA, TRUE);

  /*
    Since memory was allocated on the stack for local struct 
    variable a, it will be deallocated when a goes out of scope.
    The destructor will also be invoked. Notice that dynamic flag
    is set to false so that the destructor does not try to
    free memory.
  */
  A_Destructor(&a, FALSE);

  return 0;
}     
