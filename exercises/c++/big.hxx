#ifndef BIG_HXX
#define BIG_HXX 1

class Big
{
public:
  int *m_data;

public:
  Big();
  ~Big();

  Big(const Big&);
  Big &operator=(const Big&);
  Big(Big &&);
  Big &operator=(Big &&);
};

Big operator+(const Big&, const Big&);
Big munge(const Big&);
Big makeBig() { return Big(); }

#endif /* !BIG_HXX */
