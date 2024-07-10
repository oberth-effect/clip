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

#include "laueimage.h"

#include <QImage>
#include <QPixmap>
#include <QApplication>
 
#include <QtConcurrentRun>

#include "ui/clip.h"
#include "image/dataproviderfactory.h"
#include "image/datascalerfactory.h"
#include "tools/xmltools.h"
#include "tools/zipiterator.h"
#include "image/beziercurve.h"
#include "image/datascaler.h"


LaueImage::LaueImage(QObject* _parent) :
    QObject(_parent), provider(nullptr), scaler(nullptr), dataStore()
{
  connect(&watcher, SIGNAL(finished()), this, SLOT(doneOpenFile()));
}


void LaueImage::startOpenFile(QString filename, QDomElement base) {
  watcher.setFuture(QtConcurrent::run(this, &LaueImage::doOpenFile, filename, base));
}

QPair<DataProvider*, DataScaler*> LaueImage::doOpenFile(QString filename, QDomElement base) {
  DataProvider* dp = DataProviderFactory::getInstance().loadImage(filename, &dataStore);
  DataScaler* ds = dp ? DataScalerFactory::getInstance().getScaler(dp) : nullptr;
  if (ds && dp) {
    if (!base.isNull()) {
      dp->loadFromXML(base);
      ds->loadFromXML(base);
      loadCurvesFromXML(base, ds);
    }
    // dp and ds are created in this thread, but should live in the thread of LaueImage
    dp->moveToThread(thread());
    ds->moveToThread(thread());
  }
  return qMakePair(dp, ds);
}

void LaueImage::doneOpenFile() {
  DataProvider* dp = watcher.result().first;
  DataScaler* ds = watcher.result().second;
  if (dp!=nullptr && ds!=nullptr) {
    provider = dp;
    scaler = ds;
    // ds and dp were created in another thread and thus without _parent.
    // Set us as their _parent
    provider->setParent(this);
    scaler->setParent(this);
    connect(scaler, SIGNAL(imageContentsChanged()), this, SIGNAL(imageContentsChanged()));
    connect(scaler, SIGNAL(histogramChanged(QVector<int>,QVector<int>,QVector<int>)), this, SIGNAL(histogramChanged(QVector<int>,QVector<int>,QVector<int>)));
  } else {
    if (dp!=nullptr) delete dp;
    if (ds!=nullptr) delete ds;
  }
  emit openFinished(this);
}

LaueImage::~LaueImage() {
  if (scaler!=nullptr) delete scaler;
  if (provider!=nullptr) delete provider;
}

QImage LaueImage::getScaledImage(const QSize& requestedSize, const QPolygonF& r) {
  return scaler->getImage(requestedSize, r);
}


QList<BezierCurve*> LaueImage::getTransferCurves() {
  return scaler->getTransferCurves();
}

void LaueImage::addTransform(const QTransform &t) {
  scaler->addTransform(t);
  dataStore.addTransform(t);
}

void LaueImage::resetAllTransforms() {
  scaler->resetAllTransforms();
}

QString LaueImage::name() {
  return provider->name();
}

QList<QString> LaueImage::infoKeys() {
  return provider->getProviderInfoKeys();
}

bool LaueImage::hasInfo(const QString& key) {
  return !provider->getProviderInfo(key).isNull();
}

QVariant LaueImage::getInfo(const QString& key) {
  return provider->getProviderInfo(key);
}


QList<QWidget*> LaueImage::toolboxPages() {
  QList<QWidget*> pages;
  pages << provider->toolboxPages();
  pages << scaler->toolboxPages();
  return pages;
}

const char XML_LaueImage_element[] = "Image";
const char XML_LaueImage_element_fn[] = "Filename";

void LaueImage::saveToXML(QDomElement base) {
  QDomElement image = ensureElement(base, XML_LaueImage_element);
  image.setAttribute(XML_LaueImage_element_fn, provider->getProviderInfo(DataProvider::Info_ImagePath).toString());
  provider->saveToXML(image);
  scaler->saveToXML(image);
  saveCurvesToXML(image);
}

void LaueImage::loadFromXML(QDomElement base) {
  QDomElement element = base.elementsByTagName(XML_LaueImage_element).at(0).toElement();
  if (element.isNull()) return;
  QString filename = element.attribute(XML_LaueImage_element_fn);
  if (scaler!=nullptr) delete scaler;
  if (provider!=nullptr) delete provider;
  scaler = nullptr;
  provider = nullptr;

  startOpenFile(filename, element);
}


const char XML_laueImage_Transfercurves[] = "Transfercurves";
QStringList TransfercurveNames = QStringList() << "Value" << "Red" << "Green" << "Blue";

void LaueImage::saveCurvesToXML(QDomElement base) {
  QDomElement curves = ensureElement(base, XML_laueImage_Transfercurves);
  QPair<BezierCurve*, QString> p;
  foreach (p, Zip(getTransferCurves(), TransfercurveNames)) {
    p.first->saveToXML(curves, p.second);
  }
}

void LaueImage::loadCurvesFromXML(QDomElement base, DataScaler* ds) {
  if (!ds) ds=scaler;
  QDomElement curves = base.elementsByTagName(XML_laueImage_Transfercurves).at(0).toElement();
  if (curves.isNull()) return;
  QPair<BezierCurve*, QString> p;
  foreach (p, Zip(ds->getTransferCurves(), TransfercurveNames)) {
    p.first->loadFromXML(curves, p.second);
  }
}



