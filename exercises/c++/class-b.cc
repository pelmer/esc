// A typical example of inheritance and virtual function use.
// We would be mapping this code to equivalent C.

#include <iostream>

// Prototype graphics library function to draw a circle
void glib_draw_circle (int x, int y, int radius);

void
glib_draw_circle (int x, int y, int radius)
{
  std::cout << "::glib_draw_circle(" 
	    << x << ", " 
	    << y << ", "
	    << radius << ")..."
	    << std::endl;
}

// Shape base class declaration
class Shape
{
protected:
  int m_x;    // X coordinate
  int m_y;  // Y coordinate
 
public:
  // Pure virtual function for drawing
  virtual void Draw() = 0; 

  // A regular virtual function
  virtual void MoveTo(int newX, int newY);

  // Regular method, not overridable.
  void Erase();

  // Constructor for Shape
  Shape(int x, int y);

  // Virtual destructor for Shape
  virtual ~Shape();
};

 
// Circle class declaration
class Circle : public Shape
{
private:
  int m_radius;    // Radius of the circle

public:

  // Override to draw a circle
  virtual void Draw();   

  // Constructor for Circle
  Circle(int x, int y, int radius);

  // Destructor for Circle
  virtual ~Circle();
};

 
// Shape constructor implementation
Shape::Shape(int x, int y) :
  m_x(x),
  m_y(y)
{}

// Shape destructor implementation
Shape::~Shape()
{
  //...
}

void
Shape::MoveTo(int newX, int newY)
{
  m_x = newX;
  m_y = newY;
}

void
Shape::Erase()
{
  std::cout << "::Shape::Erase..."
	    << std::endl;
}

// Circle constructor implementation
Circle::Circle(int x, int y, int radius) : 
  Shape (x, y),
  m_radius(radius)
{}

// Circle destructor implementation
Circle::~Circle()
{
  //...
}

// Circle override of the pure virtual Draw method.
void
Circle::Draw()
{
  glib_draw_circle(m_x, m_y, m_radius);
}

int
main(int argc, char **argv)
{
  // Define a circle with a center at (50,100) and a radius of 25
  Shape *pShape = new Circle(50, 100, 25);

  // Define a circle with a center at (5,5) and a radius of 2
  Circle aCircle(5,5, 2);

  // Various operations on a Circle via a Shape pointer
  pShape->Draw();
  pShape->MoveTo(100, 100);
  pShape->Erase();
  delete pShape;

  // Invoking the Draw method directly
  aCircle.Draw();
}
