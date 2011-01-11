#ifndef SOLUTION_H
#define SOLUTION_H

#include <QList>

#include "tools/vec3D.h"
#include "tools/mat3D.h"
#include "indexing/indexer.h"

class Solution {
public:
  Solution();
  Solution(const Solution& s);
  Solution& operator=(const Solution& s);

  Mat3D bestRotation;
  double hklDeviation;
  double indexMean;
  double indexRMS;

  double allIndexRMS() const;
  double allIndexMean() const;
  double hklDeviationSum() const;

  QList< TVec3D<int> > markerIdx;
  QList<double> markerScore;

};

#include <QMetaType>
Q_DECLARE_METATYPE(Solution);

#endif // SOLUTION_H
