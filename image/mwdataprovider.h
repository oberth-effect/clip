#ifndef MWDATAPROVIDER_H
#define MWDATAPROVIDER_H

#include <QMap>
#include <QVariant>
#include <QVector>

#include "image/dataprovider.h"


class MWDataProvider : public DataProvider
{
  Q_OBJECT
public:
  class Factory: public DataProvider::ImageFactoryClass {
  public:
    Factory() {}
    QStringList fileFormatFilters();
    DataProvider* getProvider(QString, ImageDataStore*, QObject* = 0);
  };

  virtual ~MWDataProvider();

  virtual const void* getData();
  virtual QSize size();
  virtual int bytesCount();
  virtual int pixelCount();
  virtual Format format();
private:
  explicit MWDataProvider(QObject *parent = 0);
signals:

public slots:
private:
  QVector<float> pixelData;
};

#endif // MWDATAPROVIDER_H