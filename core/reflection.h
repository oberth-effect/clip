#ifndef __REFLECTION_H__
#define __REFLECTION_H__

#include <QVector>
#include <tools/mat3D.h>
#include <tools/vec3D.h>

class  Reflection {
public:
  //Reflection(): orders() {}
  // Index
  int h;
  int k;
  int l;

  TVec3D<int> hkl() { return TVec3D<int>(h,k,l); }
  
  //h^2+k^2+l^2
  int hklSqSum;

  // lowest order (if ==0, this reflection is not in scattering position)
  int lowestDiffOrder;

  // highest order (if ==0, this reflection is not in scattering position)
  int highestDiffOrder;

  // direct space d-Value
  double d;

  // Reziprocal lattice Vector = 1/(2*d)
  double Q;

  // Order 1 scattering Wavevectors (Q/sin(theta))
  double Qscatter;

  // All contributing orders
  QVector<int> orders;
  // Real and imaginary part of Structure factor
  //vector<double> Fr;
  //vector<double> Fi;
  
  // reziprocal direction (without rotations)
  Vec3D normalLocal;

  // reziprocal direction (with rotations)
  Vec3D normal;

  // Direction of scattered ray
  Vec3D scatteredRay;

  static QString formatOverline(int);
  static QString hkl2text(int, int, int);
  QString toHtml() const;
  QString toText() const;
};

Q_DECLARE_TYPEINFO(Reflection, Q_MOVABLE_TYPE);

#endif
