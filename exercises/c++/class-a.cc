// Example class A contains regular and
// static member variables and methods.

#include <iostream>

class A
{
private:
  int m_x;
  static int g_y;
  int m_z;

  // Should be invoked when the object ends
  void NotifyEnd();

public:

  A(int x);
  ~A();

  void UpdateX(int newX);
  static void UpdateY(int newY);
};

 

// Initialization of the static variable
int A::g_y = 0;

// Invoked by d-tor
void
A::NotifyEnd()
{
  std::cout << "A::NotifyEnd..." << std::endl;
}

// The non-static member variables
// are initialized in the constructor
A::A(int x) :
  m_x(x),
  m_z(0)
{}

// Destructor invokes a private method
A::~A()
{
  NotifyEnd();
}



// UpdateX checks the value of X against
// a static variable before updating the value
void 
A::UpdateX(int newX)
{
  if (g_y != 0 && m_x < newX)
  {
    m_x = newX;
  }
}


// Unconditional update of static variable m_y
void A::UpdateY(int newY)
{
  g_y = newY;
}


int
main(int argc, char **argv)
{
  // Create an object on the heap
  A *pA = new A(5);

  // Create an object on the stack
  A a(6);

  // Example of an access via a pointer
  pA->UpdateX(8);

  // Example of a direct access
  a.UpdateX(9);

  // Example of static method call
  A::UpdateY(1000);

  // Deleting the object
  delete pA;

  return 0;
}
