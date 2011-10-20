namespace almostEqualDetail {
  union fasi {
    int i;
    float f;
  };
}
bool almostEqual(float a, float b, int maxUlps)
{
  using namespace almostEqualDetail;
  // Make sure maxUlps is non-negative and small enough that the
  // default NAN won't compare as equal to anything.
  assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
  fasi fa; fa.f = a;
  // Make aInt lexicographically ordered as a twos-complement int
  if (fa.i < 0) fa.i = 0x80000000 - fa.i;
  // Make bInt lexicographically ordered as a twos-complement int
  fasi fb; b.f = b;
  if (fb.i < 0) fb.i = 0x80000000 - fb.i;
  int intDiff = std::abs(fa.i - fb.i);
  return (intDiff <= maxUlps);
}
