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

#include "xmltools.h"

#include <QDomDocument>
#include <QRectF>
#include <QRect>
#include <QFile>

#include "config/configstore.h"

auto readXMLFile(QString filename) -> QDomDocument {
  QDomDocument doc;
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly))
    return QDomDocument();
  if (!doc.setContent(&file)) {
    file.close();
    return QDomDocument();
  }

  file.close();
  return doc;
}

auto readDouble(QDomElement element, QString name, bool& ok, double defaultValue) -> double {
  if (element.hasAttribute(name)) {
    bool _ok;
    double value = element.attribute(name).toDouble(&_ok);
    ok &= ok;
    if (_ok) return value;
  }
  ok = false;
  return defaultValue;
}

auto readInt(QDomElement element, QString name, bool &ok, int defaultValue) -> int {
  if (element.hasAttribute(name)) {
    bool _ok;
    int value = element.attribute(name).toInt(&_ok);
    ok &= ok;
    if (_ok) return value;
  }
  ok = false;
  return defaultValue;
}




template <class R> void RectToTag(QDomElement base, QString name, R rect) {
  QDomElement e = base.ownerDocument().createElement(name);
  base.appendChild(e);
  e.setAttribute("x", rect.x());
  e.setAttribute("y", rect.y());
  e.setAttribute("width", rect.width());
  e.setAttribute("height", rect.height());
}
template void RectToTag(QDomElement base, QString name, QRect rect);
template void RectToTag(QDomElement base, QString name, QRectF rect);



auto TagToRect(QDomElement element, QRectF defaultValue, bool* _ok) -> QRectF {
  bool ok = true;
  double x = readDouble(element, "x", ok, defaultValue.x());
  double y = readDouble(element, "y", ok, defaultValue.y());
  double w = readDouble(element, "width", ok, defaultValue.width());
  double h = readDouble(element, "height", ok, defaultValue.height());
  if (_ok) *_ok = ok;
  return QRectF(x,y,w,h);
}

auto TagToRect(QDomElement element, QRect defaultValue, bool obeyGeometryReadSettings, bool* _ok) -> QRect {
  bool ok = true;
  int x = defaultValue.x();
  int y = defaultValue.y();
  int w = defaultValue.width();
  int h = defaultValue.height();

  if (!obeyGeometryReadSettings || ConfigStore::getInstance()->loadPositionFromWorkspace()) {
    x=readInt(element, "x", ok, defaultValue.x());
    y=readInt(element, "y", ok, defaultValue.y());
  }
  if (!obeyGeometryReadSettings || ConfigStore::getInstance()->loadSizeFromWorkspace()) {
    w=readInt(element, "width", ok, defaultValue.width());
    h=readInt(element, "height", ok, defaultValue.height());
  }

  if (_ok) *_ok = ok;
  return QRect(x,y,w,h);
}


void PointToTag(QDomElement base, QString name, const QPointF& p) {
  QDomElement e = base.ownerDocument().createElement(name);
  base.appendChild(e);
  e.setAttribute("x", p.x());
  e.setAttribute("y", p.y());
}

auto TagToPoint(QDomElement element, const QPointF& defaultValue, bool* _ok) -> QPointF {
  bool ok = true;
  double x = readDouble(element, "x", ok, defaultValue.x());
  double y = readDouble(element, "y", ok, defaultValue.y());
  if (_ok) *_ok = ok;
  return QPointF(x,y);
}

auto ensureElement(QDomElement base, QString name) -> QDomElement {
  if (base.tagName()==name) {
    return base;
  } else {
    return base.appendChild(base.ownerDocument().createElement(name)).toElement();
  }
}


