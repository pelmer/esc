/*
The following code maps the C++ code for the Shape and Circle classes
to C code.
*/

#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

typedef int BOOLEAN;

/*
Error handler used to stuff dummy VTable
entries. This is covered later.
*/
void pure_virtual_called_error_handler();

void pure_virtual_called_error_handler()
{
  printf("pure virtual method called\n");
  abort();
}

/* Prototype graphics library function to draw a circle */
void glib_draw_circle (int x, int y, int radius);

void
glib_draw_circle (int x, int y, int radius)
{
  printf("::glib_draw_circle(%d, %d, %d)...\n",
	 x, y, radius);
}


typedef void (*VirtualFunctionPointer)(...);

/*
VTable structure used by the compiler to keep
track of the virtual functions associated with a class.
There is one instance of a VTable for every class
containing virtual functions. All instances of
a given class point to the same VTable.
*/
struct VTable
{
  /*
   d and i fields are used when multiple inheritance and virtual
   base classes are involved. We will be ignoring them for this
   discussion.
  */
  int d;
  int i;

  /*
   A function pointer to the virtual function to be called is
   stored here.
  */
  VirtualFunctionPointer pFunc;
};

extern VTable VTableArrayForShape[];
extern VTable VTableArrayForCircle[];

/*
The Shape class maps into the Shape structure in C. All
the member variables present in the class are included
as structure elements. Since Shape contains a virtual
function, a pointer to the VTable has also been added.
*/
struct Shape
{
  int m_x;
  int m_y;

  /*
  The C++ compiler inserts an extra pointer to a vtable which
  will keep a function pointer to the virtual function that
  should be called.
  */
  VTable *pVTable;
};

/*
Function prototypes that correspond to the C++ methods
for the Shape class,
*/
Shape *Shape_Constructor(Shape *this_ptr, int x, int y);
void Shape_Destructor(Shape *this_ptr, BOOLEAN dynamic);
void Shape_MoveTo(Shape *this_ptr, int newX, int newY);
void Shape_Erase(Shape *this_ptr);

Shape*
Shape_Constructor(Shape *this_ptr, int x, int y)
{
  /* Check if memory has been allocated for struct Shape. */
  if (this_ptr == NULL)
  {
    /* Allocate memory of size Shape. */
    this_ptr = (Shape *) malloc(sizeof(Shape));
  }

  /*
  Once the memory has been allocated for Shape,
  initialise members of Shape.
  */
  if (this_ptr)
  {  
    /* Initialize the VTable pointer to point to shape */
    this_ptr->pVTable = VTableArrayForShape;
    this_ptr->m_x = x;
    this_ptr->m_y = y;
  }
  return this_ptr;
}

void
Shape_Destructor(Shape *this_ptr, BOOLEAN dynamic)
{
  /*
  Restore the VTable to that for Shape. This is
  required so that the destructor does not invoke
  a virtual function defined by a inheriting class.
  (The base class destructor is invoked after inheriting
  class actions have been completed. Thus it is not
  safe to invoke the ineriting class methods from the
  base class destructor)
  */
  this_ptr->pVTable = VTableArrayForShape;

  /*...*/

  /*
  If the memory was dynamically allocated
  for Shape, explicitly free it.
  */
  if (dynamic)
  {
    free(this_ptr);
  }
}

void
Shape_MoveTo(Shape *this_ptr, int newX, int newY)
{
  if (this_ptr)
  {
    this_ptr->m_x = newX;
    this_ptr->m_y = newY;
  }
}

void
Shape_Erase(Shape* this_ptr)
{
  printf("::Shape::Erase...\n");
}

/*
The Shape vtable array contains entries for Draw and MoveTo
virtual functions. Notice that there is no entry for Erase,
as it is not virtual. Also, the first two fields for every
vtable entry are zero, these fields might have non zero
values with multiple inheritance, virtual base classes
A third entry has also been defined for the virtual destructor
*/
VTable VTableArrayForShape[] =
  {
    /*
    Vtable entry virtual function Draw.
    Since Draw is pure virtual, this entry
    should never be invoked, so call error handler
    */
    { 0, 0, (VirtualFunctionPointer) &pure_virtual_called_error_handler },

    /*
    This vtable entry invokes the base class's
    MoveTo method.
    */
    { 0, 0, (VirtualFunctionPointer) &Shape_MoveTo },

    /* Entry for the virtual destructor */
    { 0, 0, (VirtualFunctionPointer) (void*) &Shape_Destructor }
};


/*
The struct Circle maps to the Circle class in the C++ code.
The layout of the structure is:
- Member variables inherited from the the base class Shape.
- Vtable pointer for the class.
- Member variables added by the inheriting class Circle.
*/
struct Circle
{
  /* Fields inherited from Shape */
  int m_x;
  int m_y;
  VTable *pVTable;

  /* Fields added by Circle */
  int m_radius;
};

/*
Function prototypes for methods in the Circle class.
*/
Circle *Circle_Constructor(Circle *this_ptr, int x, int y, int radius);
void Circle_Draw(Circle *this_ptr);
void Circle_Destructor(Circle *this_ptr, BOOLEAN dynamic);

Circle*
Circle_Constructor(Circle *this_ptr, int x, int y, int radius)
{
  /* Check if memory has been allocated for struct Circle. */
  if (this_ptr == NULL)
  {
    /* Allocate memory of size Circle. */
    this_ptr = (Circle *) malloc(sizeof(Circle));
  }

  /*
  Once the memory has been allocated for Circle,
  initialise members of Circle.
  */
  if (this_ptr)
  {
    /* Invoking the base class constructor */
    Shape_Constructor((Shape *)this_ptr, x, y);
    this_ptr->pVTable = VTableArrayForCircle;

    this_ptr->m_radius = radius;
  }
  return this_ptr;
}

void
Circle_Destructor(Circle *this_ptr, BOOLEAN dynamic)
{
  /* Restore the VTable to that for Circle */
  this_ptr->pVTable = VTableArrayForCircle;

  /*...*/

  /*
  Invoke the base class destructor after ineriting class
  destructor actions have been completed. Also note that
  that the dynamic flag is set to false so that the shape
  destructor does not free any memory.
  */
  Shape_Destructor((Shape *) this_ptr, FALSE);

  /*
  If the memory was dynamically allocated
  for Circle, explicitly free it.
  */
  if (dynamic)
  {
    free(this_ptr);
  }
}

void
Circle_Draw(Circle *this_ptr)
{
  glib_draw_circle(this_ptr->m_x, this_ptr->m_y, this_ptr->m_radius);
}

/* Vtable array for Circle */
VTable VTableArrayForCircle[] =
  {
    /*
    Vtable entry virtual function Draw.
    Circle_Draw method will be invoked when Shape's
    Draw method is invoked
    */
    { 0, 0, (VirtualFunctionPointer) Circle_Draw },

    /*
    This vtable entry invokes the base class's
    MoveTo method.
    */
    { 0, 0, (VirtualFunctionPointer) Shape_MoveTo },

    /* Entry for the virtual destructor */
    { 0, 0, (VirtualFunctionPointer) Circle_Destructor }
  };

int
main(int argc, char **argv)
{   
  /*
  Dynamically allocate memory by passing NULL in this arguement.
  Also initialse members of struct pointed to by pShape.
  */
  Shape *pShape = (Shape *) Circle_Constructor(NULL, 50, 100, 25);

  /* Define a local variable aCircle of type struct Circle. */
  Circle aCircle;

  /* Initialise members of struct variable aCircle. */
  Circle_Constructor(&aCircle, 5, 5, 2);

  /*
  Virtual function Draw is called for the shape pointer. The compiler
  has allocated 0 offset array entry to the Draw virtual function.
  This code corresponds to "pShape->Draw();"
  */
  (pShape->pVTable[0].pFunc)(pShape);

  /*
  Virtual function MoveTo is called for the shape pointer. The compiler
  has allocared 1 offset array entry to the MoveTo virtual function.
  This code corresponds to "pShape->MoveTo(100, 100);"
  */
  (pShape->pVTable[1].pFunc)(pShape, 100, 100);

  /*
  The following code represents the Erase method. This method is
  not virtual and it is only defined in the base class. Thus
  the Shape_Erase C function is called.
  */
  Shape_Erase(pShape);

  /* Delete memory pointed to by pShape (explicit delete in original code).
  Since the destructor is declared virtual, the compiler has allocated
  2 offset entry to the virtual destructor
  This code corresponds to "delete pShape;".
  */
  (pShape->pVTable[2].pFunc)(pShape, TRUE);

  /*
  The following code corresponds to aCircle.Draw().
  Here the compiler can invoke the method directly instead of
  going through the vtable, since the type of aCircle is fully
  known. (This is very much compiler dependent. Dumb compilers will
  still invoke the method through the vtable).
  */
  Circle_Draw(&aCircle);

  /*
  Since memory was allocated from the stack for local struct 
  variable aCircle, it will be deallocated when aCircle goes out of scope.
  The destructor will also be invoked. Notice that dynamic flag is set to
  false so that the destructor does not try to free memory. Again, the
  compiler does not need to go through the vtable to invoke the destructor.
  */
  Circle_Destructor(&aCircle, FALSE);

  return 0;
}
