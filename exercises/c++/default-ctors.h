// dear emacs, this is -*- c++ -*-

#ifndef DFLT_CTORS_H
#define DFLT_CTORS_H 1

class A
{
  int data;
};

class B
{
  float data;
};

class C
{
  float data;
};

class D
{
  float data;
};

class E
{
  int data;
};

class F
{
  int data;
};

class X
{
  A a;
  B b;
public:
  virtual void fct();
};

class Y : public X
{
  C c;
  D d;
};

class Z : public Y
{
  E e;
  F f;
public:
  Z() {}
};

#endif // !DFLT_CTORS_H
