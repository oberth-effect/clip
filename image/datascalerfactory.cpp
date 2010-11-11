#include "datascalerfactory.h"
#include <iostream>

using namespace std;

DataScalerFactory::DataScalerFactory(QObject *parent) :
    QObject(parent)
{
    cout << "init DataScalerFactory" << endl;
}

DataScalerFactory::DataScalerFactory(const DataScalerFactory &) {};

DataScalerFactory::~DataScalerFactory() {}

DataScalerFactory& DataScalerFactory::getInstance() {
  static DataScalerFactory instance;
  return instance;
}

DataScaler* DataScalerFactory::getScaler(DataProvider* dp) {
  if (scalerGenerators.contains(dp->format())) {
    ScalerGenerator gen = scalerGenerators[dp->format()];
    return (*gen)(dp);
  }
  return NULL;
}

bool DataScalerFactory::registerDataScaler(DataProvider::Format format, ScalerGenerator generator) {
  DataScalerFactory::getInstance().scalerGenerators.insert(format, generator);
  return true;
}
