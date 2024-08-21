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

#include "dataproviderfactory.h"

 
#include <QStringList>



DataProviderFactory::DataProviderFactory()
{
}

DataProviderFactory::DataProviderFactory(const DataProviderFactory &) {};

DataProviderFactory::~DataProviderFactory() {
  foreach (DataProvider::ImageFactoryClass* item, imageLoaders.values()) {
    delete item;
  }
}

auto DataProviderFactory::getInstance() -> DataProviderFactory& {
  static DataProviderFactory instance;
  return instance;
}

auto DataProviderFactory::loadImage(const QString &filename, ImageDataStore* store, QObject* _parent) -> DataProvider* {
  foreach (int key, imageLoaders.uniqueKeys()) {
    foreach (auto loader, imageLoaders.values(key)) {
      DataProvider* dp = loader->getProvider(filename, store, _parent);
      if (dp) return dp;
    }
  }
  return nullptr;
}

auto DataProviderFactory::registeredImageFormats() -> QStringList {
  QStringList formats;
  foreach (int key, imageLoaders.uniqueKeys()) {
    foreach (auto loader, imageLoaders.values(key)) {
      formats += loader->fileFormatFilters();
    }
  }
  return formats;
}

auto DataProviderFactory::registerImageLoader(int priority, DataProvider::ImageFactoryClass* loader) -> bool {
  DataProviderFactory::getInstance().imageLoaders.insert(priority, loader);
  return true;
}

auto DataProviderFactory::registerDeviceOpener(int, DeviceOpener) -> bool {
return true;
}

