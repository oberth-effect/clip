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

#include "imagedatastore.h"

#include <QDebug>

#include "tools/tools.h"

ImageDataStore::ImageDataStore(QObject *parent) :
    QObject(parent)
{
}


bool ImageDataStore::hasData(DataType d) const {
  return dataStore.contains(d);
}

QVariant ImageDataStore::getData(DataType d) const {
  return dataStore.value(d, 0.0);
}

void ImageDataStore::setData(DataType d, QVariant v) {
  if (!hasData(d) || getData(d)!=v) {
    dataStore[d]=v;
    emit dataChanged(d, v);
  }
}

void ImageDataStore::addTransform(const QTransform &t) {
  imageTransform = t * imageTransform;
  emit transformChanged();
}

QSizeF ImageDataStore::getTransformedSizeData(DataType d) const {
  if (hasData(d)) {
    return transformSize(getData(d).toSizeF(), imageTransform);
  } else {
    return QSizeF();
  }
}

void ImageDataStore::setTransformedSizeData(DataType d, const QSizeF& s) {
  setData(d, transformSize(s, imageTransform.inverted()));
}
