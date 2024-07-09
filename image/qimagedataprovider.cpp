/**********************************************************************
  Copyright (C) 2008-2011 Olaf J. Schumann

  This file is part of the Cologne Laue Indexation Program.
  For more information, see <http://clip4.sf.net>

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

#include "qimagedataprovider.h"

#include <QStringList>
#include <QMap>
 
#include <QImageReader>

#include "image/dataproviderfactory.h"
#include "image/imagedatastore.h"

#include <tiffio.h>

using namespace std;


QImageDataProvider::QImageDataProvider(const QImage& img, bool mono, QObject* _parent) :
    DataProvider(_parent),
    mono(mono),
    data(img)
{
}

QImageDataProvider::~QImageDataProvider() {}

QStringList QImageDataProvider::Factory::fileFormatFilters() {
  QStringList formats;
  foreach (QByteArray format, QImageReader::supportedImageFormats()) {
    formats += QString(format);
  }
  return formats;
}

DataProvider* QImageDataProvider::Factory::getProvider(QString filename, ImageDataStore *store, QObject* _parent) {
  //QImage img(filename);
  QImageReader reader(filename);
  QImage img;
  bool ismono = false;
  if (reader.read(&img)) {
    QMap<QString, QVariant> headerData;
    foreach (QString key, img.textKeys()) {
      if (key!="") {
        headerData.insert(key, QVariant(img.text(key)));
      }
    }
    if (reader.format() == "tiff") {
      TIFF* tif = TIFFOpen(filename.toStdString().c_str(), "r");
      if (tif) {
          char* value;
          if (TIFFGetField(tif, TIFFTAG_IMAGEDESCRIPTION, &value)) {
            QString desc(value);
            //printf("Desc: |%s|\n", qPrintable(desc));
            ismono = true; //convert all tiff files to monochrome
            if (desc.trimmed().startsWith("instrument:")) {
              printf("ILL Tiff file detected, parsing description\n");
              QStringList pairs = desc.split(',');
              for (const QString &pair : pairs) {
                QStringList keyValue = pair.split(':');
                if (keyValue.size() == 2) {
                  headerData.insert(keyValue[0].trimmed(), keyValue[1].trimmed());
                  if (keyValue[0].trimmed() == "angle_horizontal") {
                    bool ok;
                    double d = keyValue[1].trimmed().toDouble(&ok);
                    if (ok) {
                      printf("Setting detector - sample distance to %f\n", d);
                      store->setData(ImageDataStore::PlaneDetectorToSampleDistance, d);
                    }
                  }
                }
              }

            } 
          }
          TIFFClose(tif);
      }
    } 
    store->setData(ImageDataStore::PixelSize, img.size());
    headerData.insert(Info_ImageSize, QString("%1x%2 pixels").arg(img.width()).arg(img.height()));
    if (ismono) {
      img = img.convertToFormat(QImage::Format_Grayscale16 );
    } else {
      img = img.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    }
    QImageDataProvider* provider = new QImageDataProvider(img, ismono, _parent);
    
    
    provider->insertFileInformation(filename);
    provider->providerInformation.unite(headerData);
    return provider;
  }
  return nullptr;
}

const void* QImageDataProvider::getData() {
  return data.bits();
}

QSize QImageDataProvider::size() {
  return data.size();
}

int QImageDataProvider::bytesCount() {
  return data.sizeInBytes();
}

int QImageDataProvider::pixelCount() {
  return data.width()*data.height();
}

DataProvider::Format QImageDataProvider::format() {
  if (mono)
    return UInt16;
  else
    return RGB8Bit;
}

void QImageDataProvider::saveToXML(QDomElement) {

}

void QImageDataProvider::loadFromXML(QDomElement) {

}

bool registerOK = DataProviderFactory::registerImageLoader(128, new QImageDataProvider::Factory());
