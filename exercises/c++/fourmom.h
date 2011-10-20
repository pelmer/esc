// dear emacs, this is -*- C++ -*-
#ifndef FOURMOM_FOURMOM_H
#define FOURMOM_FOURMOM_H 1

class FourMom
{
  float m_px, m_py, m_pz, m_ene;
public:
  // implicit inlining
  // definition provided w/ declaration
  float px() const { return m_px; }
  void set_px(float px);

  FourMom();
  FourMom(float px, float py, float pz, float ene);
  ~FourMom();
};
// use inline keyword
inline
void
FourMom::set_px(float px)
{
  m_px = px;
}
#endif // !FOURMOM_FOURMOM_H
