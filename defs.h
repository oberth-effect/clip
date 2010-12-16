#ifndef DEFS_H
#define DEFS_H

// Will be generated by make via call to HG
extern const char* HG_REPRO_ID;
extern const char* HG_REPRO_REV;
extern const char* HG_REPRO_DATE;
extern const char* BUILD_DATE;
extern const char* BUILD_TIME;

static inline unsigned long long rdtsctime()
{
     unsigned int eax, edx;
     unsigned long long val;
     __asm__ __volatile__("cpuid": : : "ax", "bx", "cx", "dx");
     __asm__ __volatile__("rdtsc":"=a"(eax), "=d"(edx));
     val = edx;
     val = val << 32;
     val += eax;
     return val;
}

static inline int ggt(int a, int b) {
  while (b) {
    int tb=b;
    b=a%b;
    a=tb;
  }
  if (a<0) return -a;
  return a;
}

#include <QString>
#include "tools/vec3D.h"

Vec3D parseHKLString(const QString& s, bool* ok=NULL);

#endif // DEFS_H
