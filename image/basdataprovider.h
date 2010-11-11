#ifndef BASDATAPROVIDER_H
#define BASDATAPROVIDER_H

#include <QMap>
#include <QVariant>
#include <QVector>
#include "image/dataprovider.h"


class BasDataProvider : public DataProvider
{
  Q_OBJECT
public:
  static DataProvider* loadImage(const QString&);

  virtual const void* getData();
  virtual int width();
  virtual int height();
  virtual int bytesCount();
  virtual int pixelCount();
private:
  explicit BasDataProvider(QObject *parent = 0);
signals:

public slots:
private:
  QMap<QString, QVariant> headerData;
  QVector<float> pixelData;
};

#endif // BASDATAPROVIDER_H