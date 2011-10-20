/*****************************   sfmt.cpp   ***********************************
* Authors:
* Mutsuo Saito (Hiroshima University)
* Makoto Matsumoto (Hiroshima University)
* Agner Fog (Copenhagen University College of Engineering)
* Date created:  2006
* Last modified: 2009-02-08
* Project:       randomc
* Platform:      This C++ version requires an x86 family microprocessor 
*                with the SSE2 or later instruction set and a compiler 
*                that supports intrinsic functions.
* Source URL:    www.agner.org/random
* Source URL for original C language implementation:
*                www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT/index.html
*
* Description:
* "SIMD-oriented Fast Mersenne Twister" (SFMT) random number generator.
* The SFMT random number generator is a modification of the Mersenne Twister 
* with improved randomness and speed, adapted to the SSE2 instruction set.
* The SFMT was invented by Mutsuo Saito and Makoto Matsumoto.
* The present C++ implementation is by Agner Fog.
*
* Class description and member functions: See sfmt.h
*
* Example:
* ========
* The file EX-RAN.CPP contains an example of how to generate random numbers.
*
* Library version:
* ================
* An optimized version of this random number generator is provided as function
* libraries in randoma.zip. These function libraries are coded in assembly
* language and support only x86 platforms, including 32-bit and 64-bit
* Windows, Linux, BSD, Mac OS-X (Intel based). Use randoma.h from randoma.zip
*
*
* Further documentation:
* ======================
* See the file ran-instructions.pdf for detailed instructions and documentation
*
*
* Copyright notice
* ================
* GNU General Public License http://www.gnu.org/licenses/gpl.html
* This C++ implementation of SFMT contains parts of the original C code
* which was published under the following BSD license, which is therefore
* in effect in addition to the GNU General Public License.
*
Copyright (c) 2006, 2007 by Mutsuo Saito, Makoto Matsumoto and Hiroshima University.
Copyright (c) 2008 by Agner Fog.
All rights reserved.
Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:
    > Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    > Redistributions in binary form must reproduce the above copyright notice, 
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    > Neither the name of the Hiroshima University nor the names of its 
      contributors may be used to endorse or promote products derived from 
      this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "sfmt.h"                      // Class definition and other declarations


void CRandomSFMT::RandomInit(int seed) {
   // Re-seed
   unsigned int i;                         // Loop counter
   unsigned int y = seed;                  // Temporary
   unsigned int statesize = SFMT_N*4;      // Size of state vector
   if (UseMother) statesize += 5;      // Add states for Mother-Of-All generator

// Typecast state to unsigned int *
   unsigned int * sta = (unsigned int*)state;

   // Fill state vector with random numbers from seed
   sta[0] = y;
   const unsigned int factor = 1812433253U;// Multiplication factor

   for (i = 1; i < statesize; i++) {
      y = factor * (y ^ (y >> 30)) + i;
      sta[i] = y;
   }

   // Further initialization and period certification
   Init2();
}

namespace {
// Functions used by CRandomSFMT::RandomInitByArray
  unsigned int func1(unsigned int x) {
    return (x ^ (x >> 27)) * 1664525U;
  }
  unsigned int func2(unsigned int x) {
    return (x ^ (x >> 27)) * 1566083941U;
  }

}

void CRandomSFMT::RandomInitByArray(int const seeds[], int NumSeeds) {
   // Seed by more than 32 bits
   unsigned int i, j, count, r, lag;

   if (NumSeeds < 0) NumSeeds = 0;

   const unsigned int size = SFMT_N*4; // number of 32-bit integers in state

   // Typecast state to unsigned int *
   unsigned int * sta = (unsigned int*)state;

   if (size >= 623) {
      lag = 11;} 
   else if (size >= 68) {
      lag = 7;}
   else if (size >= 39) {
      lag = 5;}
   else {
      lag = 3;
   }
   const unsigned int mid = (size - lag) / 2;

   if ((unsigned int)NumSeeds + 1 > size) {
      count = (unsigned int)NumSeeds;
   }
   else {
      count = size - 1;
   }
#if 0
   // Original code. Argument to func1 is constant!
   for (i = 0; i < size; i++) sta[i] = 0x8B8B8B8B;
   r = func1(sta[0] ^ sta[mid] ^ sta[size - 1]);
   sta[mid] += r;
   r += NumSeeds;
   sta[mid + lag] += r;
   sta[0] = r;
#else
   // 1. loop: Fill state vector with random numbers from NumSeeds
   const unsigned int factor = 1812433253U;// Multiplication factor
   r = (unsigned int)NumSeeds;
   for (i = 0; i < SFMT_N*4; i++) {
      r = factor * (r ^ (r >> 30)) + i;
      sta[i] = r;
   }

#endif

   // 2. loop: Fill state vector with random numbers from seeds[]
   for (i = 1, j = 0; j < count; j++) {
      r = func1(sta[i] ^ sta[(i + mid) % size] ^ sta[(i + size - 1) % size]);
      sta[(i + mid) % size] += r;
      if (j < (unsigned int)NumSeeds) r += (unsigned int)seeds[j];
      r += i;
      sta[(i + mid + lag) % size] += r;
      sta[i] = r;
      i = (i + 1) % size;
   }

   // 3. loop: Randomize some more
   for (j = 0; j < size; j++) {
      r = func2(sta[i] + sta[(i + mid) % size] + sta[(i + size - 1) % size]);
      sta[(i + mid) % size] ^= r;
      r -= i;
      sta[(i + mid + lag) % size] ^= r;
      sta[i] = r;
      i = (i + 1) % size;
   }
   if (UseMother) {
      // 4. loop: Initialize MotherState
      for (j = 0; j < 5; j++) {
         r = func2(r) + j;
         MotherState[j] = r + sta[2*j];
      }
   }
   
   // Further initialization and period certification
   Init2();
}


void CRandomSFMT::Init2() {
   // Various initializations and period certification
   unsigned int i, j, temp;

   // Initialize mask
   static const unsigned int maskinit[4] = {SFMT_MASK};
   mask = _mm_loadu_si128((__m128i*)maskinit);

   // Period certification
   // Define period certification vector
   static const unsigned int parityvec[4] = {SFMT_PARITY};

   // Check if parityvec & state[0] has odd parity
   temp = 0;
   for (i = 0; i < 4; i++) {
      temp ^= parityvec[i] & ((unsigned int*)state)[i];
   }
   for (i = 16; i > 0; i >>= 1) temp ^= temp >> i;
   if (!(temp & 1)) {
      // parity is even. Certification failed
      // Find a nonzero bit in period certification vector
      for (i = 0; i < 4; i++) {
         if (parityvec[i]) {
            for (j = 1; j; j <<= 1) {
               if (parityvec[i] & j) {
                  // Flip the corresponding bit in state[0] to change parity
                  ((unsigned int*)state)[i] ^= j;
                  // Done. Exit i and j loops
                  i = 5;  break;
               }
            }
         }
      }
   }
   // Generate first random numbers and set ix = 0
   Generate();
}


  namespace {
    // Subfunction for the sfmt algorithm
    inline __m128i sfmt_recursion(__m128i const &a, __m128i const &b, 
				  __m128i const &c, __m128i const &d, __m128i const &mask) {
      __m128i a1, b1, c1, d1, z1, z2;
      b1 = _mm_srli_epi32(b, SFMT_SR1);
      a1 = _mm_slli_si128(a, SFMT_SL2);
      c1 = _mm_srli_si128(c, SFMT_SR2);
      d1 = _mm_slli_epi32(d, SFMT_SL1);
      b1 = _mm_and_si128(b1, mask);
      z1 = _mm_xor_si128(a, a1);
      z2 = _mm_xor_si128(b1, d1);
      z1 = _mm_xor_si128(z1, c1);
    z2 = _mm_xor_si128(z1, z2);
    return z2;
    }
  }

void CRandomSFMT::Generate() {
   // Fill state array with new random numbers
   int i;
   __m128i r, r1, r2;

   r1 = state[SFMT_N - 2];
   r2 = state[SFMT_N - 1];
   for (i = 0; i < SFMT_N - SFMT_M; i++) {
      r = sfmt_recursion(state[i], state[i + SFMT_M], r1, r2, mask);
      state[i] = r;
      r1 = r2;
      r2 = r;
   }
   for (; i < SFMT_N; i++) {
      r = sfmt_recursion(state[i], state[i + SFMT_M - SFMT_N], r1, r2, mask);
      state[i] = r;
      r1 = r2;
      r2 = r;
   }
   ix = 0;
}
/*
unsigned int CRandomSFMT::BRandom() {
   // Output 32 random bits
   unsigned int y;

   if (ix >= SFMT_N*4) {
      Generate();
   }
   y = ((unsigned int*)state)[ix++];
   if (UseMother) y += MotherBits();
   return y;
}
*/

unsigned int CRandomSFMT::MotherBits() {
   // Get random bits from Mother-Of-All generator
   unsigned long long sum;
   sum = 
      (unsigned long long)2111111111U * (unsigned long long)MotherState[3] +
      (unsigned long long)1492 * (unsigned long long)MotherState[2] +
      (unsigned long long)1776 * (unsigned long long)MotherState[1] +
      (unsigned long long)5115 * (unsigned long long)MotherState[0] +
      (unsigned long long)MotherState[4];
   MotherState[3] = MotherState[2];  
   MotherState[2] = MotherState[1];  
   MotherState[1] = MotherState[0];
   MotherState[4] = (unsigned int)(sum >> 32);       // Carry
   MotherState[0] = (unsigned int)sum;               // Low 32 bits of sum
   return MotherState[0];
}

/*
int  CRandomSFMT::IRandom (int min, int max) {
   // Output random integer in the interval min <= x <= max
   // Slightly inaccurate if (max-min+1) is not a power of 2
   if (max <= min) {
      if (max == min) return min; else return 0x80000000;
   }
   // Assume 64 bit integers supported. Use multiply and shift method
   unsigned int interval;                  // Length of interval
   unsigned long long longran;                   // Random bits * interval
   unsigned int iran;                      // Longran / 2^32

   interval = (unsigned int)(max - min + 1);
   longran  = (unsigned long long)BRandom() * interval;
   iran = (unsigned int)(longran >> 32);
   // Convert back to signed and return result
   return (int32_t)iran + min;
}

int  CRandomSFMT::IRandomX (int min, int max) {
   // Output random integer in the interval min <= x <= max
   // Each output value has exactly the same probability.
   // This is obtained by rejecting certain bit values so that the number
   // of possible bit values is divisible by the interval length
   if (max <= min) {
      if (max == min) {
         return min;                   // max == min. Only one possible value
      }
      else {
         return 0x80000000;            // max < min. Error output
      }
   }
   // Assume 64 bit integers supported. Use multiply and shift method
   unsigned int interval;                  // Length of interval
   unsigned long long longran;                   // Random bits * interval
   unsigned int iran;                      // Longran / 2^32
   unsigned int remainder;                 // Longran % 2^32

   interval = (unsigned int)(max - min + 1);
   if (interval != LastInterval) {
      // Interval length has changed. Must calculate rejection limit
      // Reject when remainder = 2^32 / interval * interval
      // RLimit will be 0 if interval is a power of 2. No rejection then.
      RLimit = (unsigned int)(((unsigned long long)1 << 32) / interval) * interval - 1;
      LastInterval = interval;
   }
   do { // Rejection loop
      longran  = (unsigned long long)BRandom() * interval;
      iran = (unsigned int)(longran >> 32);
      remainder = (unsigned int)longran;
   } while (remainder > RLimit);
   // Convert back to signed and return result
   return (int32_t)iran + min;
}

double CRandomSFMT::Random() {
   // Output random floating point number
   if (ix >= SFMT_N*4-1) {
      // Make sure we have at least two 32-bit numbers
      Generate();
   }
   unsigned long long r = *(unsigned long long*)((unsigned int*)state+ix);
   ix += 2;
   if (UseMother) {
      // We need 53 bits from Mother-Of-All generator
      // Use the regular 32 bits and the the carry bits rotated
      unsigned long long r2 = (unsigned long long)MotherBits() << 32;
      r2 |= (MotherState[4] << 16) | (MotherState[4] >> 16);
      r += r2;
   }
   // 53 bits resolution:
   // return (int64_t)(r >> 11) * (1./(67108864.0*134217728.0)); // (r >> 11)*2^(-53)
   // 52 bits resolution for compatibility with assembly version:
   return (int64_t)(r >> 12) * (1./(67108864.0*67108864.0));  // (r >> 12)*2^(-52)
}
*/
