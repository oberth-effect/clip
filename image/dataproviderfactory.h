#ifndef DATAPROVIDERFACTORY_H
#define DATAPROVIDERFACTORY_H

#include <QObject>
#include <QMultiMap>
#include <image/dataprovider.h>

class DataProviderFactory : public QObject
{
  Q_OBJECT
public:
  typedef DataProvider*(*ImageLoader)(const QString&, QObject*);
  typedef DataProvider*(*DeviceOpener)(QObject*);

  static DataProviderFactory& getInstance();
  static bool registerImageLoader(int, ImageLoader);
  static bool registerDeviceOpener(int, DeviceOpener);

  DataProvider* loadImage(const QString&, QObject*);

private:
  explicit DataProviderFactory(QObject *parent = 0);
  DataProviderFactory(const DataProviderFactory&);
  ~DataProviderFactory();

  QMultiMap<int, ImageLoader> imageLoaders;


signals:

public slots:

};

#endif // DATAPROVIDERFACTORY_H
