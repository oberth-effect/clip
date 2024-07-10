/**********************************************************************
  Copyright (C) 2010 Olaf J. Schumann

  This file is part of the Cologne Laue Indexation Program.
  For more information, see <http://clip.berlios.de>

  Clip is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Clip is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see http://www.gnu.org/licenses/
  or write to the Free Software Foundation, Inc., 51 Franklin Street,
  Fifth Floor, Boston, MA 02110-1301, USA.
 **********************************************************************/

#include <QtCore/QString>
#include <QtTest/QtTest>

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <ctime>


#include "../tools/mat3d.h"
#include "../tools/vec3d.h"
class ClipUnitTestTest : public QObject
{
    Q_OBJECT

public:
    ClipUnitTestTest();

private Q_SLOTS:
    void testMatrixInit();
    void testMatrixMultiply();
    void testMatrixSVDRandomCombine();
    //void testMatrixSVDWithData();
    void benchmarkMatrixMultiply();
    void benchmarkMatrixSVD();
private:
    unsigned long long tmax;
};

ClipUnitTestTest::ClipUnitTestTest()
{
  tmax = 0;
}

void ClipUnitTestTest::testMatrixInit()
{
  Mat3D M;
  QCOMPARE(M(0,0)    , 1.0);
  QCOMPARE(M(0,1)    , 0.0);
  QCOMPARE(M(0,2)    , 0.0);
  QCOMPARE(M(1,0)    , 0.0);
  QCOMPARE(M(1,1)    , 1.0);
  QCOMPARE(M(1,2)    , 0.0);
  QCOMPARE(M(2,0)    , 0.0);
  QCOMPARE(M(2,1)    , 0.0);
  QCOMPARE(M(2,2)    , 1.0);
}

void ClipUnitTestTest::testMatrixMultiply() {
  Mat3D M1(3,7,2,2,0,5,9,8,0);
  Mat3D M2(0,5,5,5,1,6,6,3,5);

  Mat3D M = M1*M2;

  QCOMPARE(M(0,0)    , 47.0);
  QCOMPARE(M(0,1)    , 28.0);
  QCOMPARE(M(0,2)    , 67.0);
  QCOMPARE(M(1,0)    , 30.0);
  QCOMPARE(M(1,1)    , 25.0);
  QCOMPARE(M(1,2)    , 35.0);
  QCOMPARE(M(2,0)    , 40.0);
  QCOMPARE(M(2,1)    , 53.0);
  QCOMPARE(M(2,2)    , 93.0);


}

void ClipUnitTestTest::testMatrixSVDRandomCombine() {

  for (int i=0; i<10; i++) {

    Mat3D M1;

    for (int i=0; i<3; i++) {
      for (int j=0; j<3; j++) {
        M1(i,j) = 2.0*(1.0*QRandomGenerator::global()->generate()/RAND_MAX-0.5);
      }
    }`

    Mat3D M2(M1);
    Mat3D Q1;
    Mat3D Q2;

    M2.svd(Q1, Q2);

    Mat3D M3 = Q1 * M2 * Q2;

    QCOMPARE( (M3-M1).sqSum() + 1.0, 1.0 );
  }
}


void ClipUnitTestTest::benchmarkMatrixMultiply() {
  Mat3D M1;
  Mat3D M2;

  QRandomGenerator rand(time(0));

  for (int i=0; i<3; i++) {
    for (int j=0; j<3; j++) {
      M1(i,j) = 2.0*(1.0*rand.generate()/RAND_MAX-0.5);
      M2(i,j) = 2.0*(1.0*rand.generate()/RAND_MAX-0.5);
    }
  }

  QBENCHMARK {
    M1 * M2;
  }
}

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

#include <fstream>

void ClipUnitTestTest::benchmarkMatrixSVD() {
  Mat3D M(4, 0, 6, 5, 2, 8, 8, 7, -6);
  QBENCHMARK {
    unsigned long long t1 = rdtsctime();
    Mat3D Q1;
    Mat3D Q2;

    for (int i=0; i<3; i++) {
      for (int j=0; j<3; j++) {
        M(i,j) = QRandomGenerator::global()->generate()%19 - 9;
      }
    }
    Mat3D M2(M);
    int loops = M2.svd(Q1, Q2);
    unsigned long long dt = rdtsctime() - t1;
      for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
              std::cout << std::setw(2) << M(i, j) << " ";
          }
      }
      std::cout << "= " << std::setw(12) << dt << " " << M.det() << std::endl;
      std::ofstream f;
      f.open("data.dat", std::ofstream::app);
      f << std::setw(12) << dt << " " << M.det() << " " << loops << std::endl;
      f.close();
      tmax = dt;
  }




}
#include <Windows.h>
#include <Winbase.h>

int main () {
  QRandomGenerator rand(time(0));
  int N = 0;
  double m1 = 0.0;
  double m2 = 0.0;
  double m3 = 0.0;
  unsigned long long dtsum = 0;
  unsigned long loopsum = 0;

  double sigma = 0;
  double mean = 0;
  do {
    Mat3D La, Lb, Ra, Rb;
    Mat3D M;

    for (int i=0; i<3; i++) {
      for (int j=0; j<3; j++) {
        //M(i,j) = 1.0-2.0*QRandomGenerator::global()->generate()/RAND_MAX;
        M(i,j) = rand.generate()%19 - 9;
      }
    }
    Mat3D Ma = M;
    Mat3D Mb = M;
    unsigned long long t1 = rdtsctime();
    int loops1 = 512-Ma.svd(La, Ra);
    unsigned long long t2 = rdtsctime();
    int loops2 = 512-Mb.fastsvd(Lb, Rb);
    unsigned long long t3 = rdtsctime();

    Mat3D rMa = La * Ma * Ra;
    Mat3D rMb = Lb * Mb * Rb;

    if ((rMa-M).sqSum()>1e-8) std::cout << "error on MA" << std::endl;
    if ((rMb-M).sqSum()>1e-8) std::cout << "error on MB" << std::endl;
    //if (fabs((La*Ra).det()-1.0)>1e-8) cout << "error on Det Ma" << endl;
    if (fabs((Lb*Rb).det()-1.0)>1e-8) std::cout << "error on Det Mb " << (Lb*Rb).det() << " " << (Lb*Rb).det()-1.0 << std::endl;
    //if (Mb(2,2)<0) cout << "Mb value low " << Mb(2,2) << endl;


    dtsum += t2-t1;
    double v = 1.0*(t2-t1)/loops1;
    m1 += v;
    m2 += v*v;
    m3 += 1.0*(t3-t2)/loops2;
    loopsum += loops1;
    N++;
    mean = m1 / N;
    double sqmean = m2 / N;
    sigma = sqrt((sqmean - mean*mean)/N);
    if (N%1000==0) {
      QString s = QString("%1 %2 %3 %4 %5 %6\n").arg(mean).arg(sigma).arg(N).arg(dtsum).arg(loopsum).arg(m3/N);
      OutputDebugStringA(qPrintable(s));
    }
  } while (N<1000 || sigma>0.00005*mean);
  mean = m1 / N;
  double sqmean = m2 / N;
  sigma = sqrt((sqmean - mean*mean)/N);
  QString s = QString("%1 %2 %3 %4 %5\n").arg(mean).arg(sigma).arg(N).arg(dtsum).arg(loopsum);
  OutputDebugStringA(qPrintable(s));
}


//QTEST_APPLESS_MAIN(ClipUnitTestTest);

#include "tst_clipunittesttest.moc"
