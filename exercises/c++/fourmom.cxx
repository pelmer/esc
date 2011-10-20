#include "fourmom.h"

FourMom::FourMom() :
  m_px(0.),
  m_py(0.),
  m_pz(0.),
  m_ene(0.)
{}

FourMom::FourMom(float px, float py, float pz, float ene) :
  m_px(px),
  m_py(py),
  m_pz(pz),
  m_ene(ene)
{}

FourMom::~FourMom()
{}
