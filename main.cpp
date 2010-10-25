#include <QtGui/QApplication>
#include <ui/clip.h>

#include <tools/mat3D.h>
#include <tools/vec3D.h>
#include <QTime>
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Clip w;
  w.show();
  return a.exec();
}


