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

#include "zoneitem.h"
#include <QPainter>
#include <QPen>
#include <QCursor>
 
#include <QPointF>

#include "core/projector.h"
#include "tools/circleitem.h"
#include "tools/xmltools.h"
#include "config/configstore.h"

using namespace std;

ZoneItem::ZoneItem(const QPointF& p1, const QPointF& p2, Projector* p, QGraphicsItem* parent):
    PropagatingGraphicsObject(parent),
    AbstractProjectorMarkerItem(p, AbstractMarkerItem::ZoneMarker),
    imgRect(-0.01, -0.01, 1.02, 1.02),
    startHandle(new CircleItem(projector->getSpotSize(), this)),
    endHandle(new CircleItem(projector->getSpotSize(), this))
{
  highlight(false);
  setFlag(QGraphicsItem::ItemSendsGeometryChanges);
  QList<CircleItem*> l = QList<CircleItem*>() << startHandle << endHandle;
  connect(projector, SIGNAL(projectionParamsChanged()), this, SLOT(updatePolygon()));
  connect(projector, SIGNAL(projectionParamsChanged()), this, SLOT(updateOptimalZone()));
  foreach (CircleItem* item, l) {
    ConfigStore::getInstance()->ensureColor(ConfigStore::ZoneMarkerHandles, item, SLOT(setColor(QColor)));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setCursor(QCursor(Qt::SizeAllCursor));
    connect(item, SIGNAL(itemClicked()),     this, SIGNAL(itemClicked()));
    connect(item, SIGNAL(positionChanged()), this, SIGNAL(positionChanged()));
    connect(item, SIGNAL(positionChanged()), this, SLOT(updatePolygon()));
    connect(item, SIGNAL(positionChanged()), this, SLOT(updateOptimalZone()));
    connect(projector, SIGNAL(spotSizeChanged(double)), item, SLOT(setRadius(double)));
  }
  connect(this, SIGNAL(positionChanged()), this, SLOT(slotInvalidateCache()));
  updatePolygon();
  updateOptimalZone();
  startHandle->setPos(p1);
  endHandle->setPos(p2);

  ConfigStore::getInstance()->ensureColor(ConfigStore::ZoneMarkerLine, this, SLOT(colorChanged()));
  ConfigStore::getInstance()->ensureColor(ConfigStore::ZoneMarkerBackground, this, SLOT(colorChanged()));
  setWidth(ConfigStore::getInstance()->getZoneMarkerWidth());
  connect(ConfigStore::getInstance(), SIGNAL(zoneMarkerWidthChanged(double)), this, SLOT(setWidth(double)));
}


ZoneItem::~ZoneItem() {
}

QRectF ZoneItem::boundingRect() const {
  return tightBoundingRect;
}

QPainterPath ZoneItem::shape() const {
  QPainterPath path;
  foreach (QPolygonF poly, zonePolys)
    path.addPolygon(poly);
  return path;
}

void ZoneItem::paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *) {
  ConfigStore* config = ConfigStore::getInstance();

  p->setPen(Qt::NoPen);
  p->setBrush(QBrush(config->color(ConfigStore::ZoneMarkerBackground)));
  foreach (QPolygonF poly, zonePolys)
    p->drawPolygon(poly);


  //PDF-Export via QPrinter::setOutputFormat(PdfFormat) has a Bug concerning
  //Cosmetic Pens and very small coordinates (here, the rect is (0, 0, 1, 1))
  //thus reset the World Transform and paint with noncosmetic pens
  QPen pen;
  pen.setWidthF(highlighted?2.0:1.0);
  pen.setCosmetic(false);
  pen.setColor(config->color(ConfigStore::ZoneMarkerLine));
  pen.setStyle(Qt::DashLine);
  p->setPen(pen);
  p->setBrush(Qt::NoBrush);
  QTransform t = p->worldTransform();
  p->resetTransform();
  foreach (QPolygonF poly, zoneLines)
    p->drawPolyline(t.map(poly));
  p->setWorldTransform(t);

}


bool PointSort(const QPointF& p1, const QPointF& p2) {
  return atan2(p1.y()-0.5, p1.x()-0.5)<atan2(p2.y()-0.5, p2.x()-0.5);
}

QPolygonF getPath(const QPointF& from, const QPointF& to, QRectF on, bool clockwise, QPointF& via) {
  QPolygonF path;
  path << from;
  int quadrant = int(M_2_PI*(atan2(from.y()-0.5, from.x()-0.5)+5*M_PI/4))%4;
  int quadrant_to = int(M_2_PI*(atan2(to.y()-0.5, to.x()-0.5)+5*M_PI/4))%4;

  QPolygonF corners;
  corners << on.topLeft() << on.topRight() << on.bottomRight() << on.bottomLeft();

  while (quadrant!=quadrant_to) {
    if (clockwise) {
      path << corners[quadrant];
      quadrant = (quadrant+1)%4;
    } else {
      quadrant = (quadrant+3)%4;
      path << corners[quadrant];
    }
  }

  path << to;

  via = 0.5*(path[0]+path[1]);

  return path;
}

void ZoneItem::updatePolygon() {
  zonePolys.clear();
  if ((startHandle->pos()!=endHandle->pos()) && projector->isProjectionEnabled()) {

    Vec3D u = projector->det2normal(projector->img2det.map(startHandle->pos()));
    Vec3D v = projector->det2normal(projector->img2det.map(endHandle->pos()));
    // Vector perpendicular to u and v
    Vec3D z = u%v;
    z.normalize();
    // Maximal scattering angle in this zone
    Vec3D n(-1,0,0);
    n = n-z*(z*n);
    if (n.norm()<1e-8) {
      n = Vec3D(0,0,1);
    } else {
      n.normalize();
    }

    // Rotate u markerWidth deg out of plane
    Mat3D R = Mat3D(z%n, markerWidth);
    v = R*n;
    u = R.transposed()*n;

    QList<QPolygonF> polys;
    polys << generatePolygon(z*(-1), u) << generatePolygon(z, v);

    QList<QPointF> borderPoints;
    QRectF sImgRect = imgRect.adjusted(0.0001, 0.0001, -0.0001, -0.0001);
    //QRectF sImgRect = imgRect.adjusted(-0.01, -0.01, 0.01, 0.01);
    foreach (QPolygonF q, polys) {
      if (!sImgRect.contains(q.first())) borderPoints << q.first();
      if (!sImgRect.contains(q.last())) borderPoints << q.last();
    }
    qSort(borderPoints.begin(), borderPoints.end(), PointSort);

    while (borderPoints.size()>1) {
      QPointF p = borderPoints.takeFirst();
      QPointF q;
      QPointF via;
      bool ok;
      QPolygonF cornerPath = getPath(p, borderPoints.first(), imgRect, true, via);
      Vec3D v = projector->det2normal(projector->img2det.map(via), ok);
      if (!ok || fabs(v*z)>sin(markerWidth)) {
        cornerPath = getPath(p, borderPoints.last(), imgRect, false, via);
        q = borderPoints.takeLast();
      } else {
        q = borderPoints.takeFirst();
      }
      QPolygonF res;
      foreach (QPolygonF poly, polys) {
        if (poly.last()==p) {
          res << poly;
          polys.removeOne(poly);
          break;
        } else if (poly.first()==p) {
          for (int n=poly.size(); n--; ) res << poly[n];
          polys.removeOne(poly);
          break;
        }
      }
      res << cornerPath;
      if (res.first()!=q) {
        foreach (QPolygonF poly, polys) {
          if (poly.first()==q) {
            res << poly;
            polys.removeOne(poly);
            break;
          } else if (poly.last()==q) {
            for (int n=poly.size(); n--; ) res << poly[n];
            polys.removeOne(poly);
            break;
          }
        }
      }
      polys << res;

    }


    QList<QPolygonF> closedItems;
    QPolygonF bigPoly;
    foreach (QPolygonF p, polys)
      if (p.isClosed()) {
      closedItems << p;
    } else {
      bigPoly << p;
    }
    if (!bigPoly.isEmpty())
      zonePolys << bigPoly;

    for (int n=0; n<closedItems.size(); n++) {
      for (int m=0; m<closedItems.size(); m++) {
        if (m==n) continue;
        bool allContained=true;
        foreach (QPointF p, closedItems[m]) {
          if (!closedItems[n].contains(p)) {
            allContained = false;
            break;
          }
        }
        if (allContained) {
          QPolygonF poly;
          closedItems[n] << closedItems[m];
          closedItems[m].clear();
        }
      }
    }
    bool firstPoly = true;
    foreach (QPolygonF p, closedItems) {
      if (!p.empty()) {
        zonePolys<< p;
        if (firstPoly) {
          firstPoly=false;
          tightBoundingRect = p.boundingRect();
        } else {
          tightBoundingRect = tightBoundingRect.united(p.boundingRect());
        }

      }
    }

  }


}

/*
 Optimal Plane:
 Set of Vectors vi
 M = sum(vi^vi)
 M.svd(Q1,Q2)
 smalest eigenvalue at M(2,2), thus Eigenvector with that value is
 n = Q2.transposed()*Vec3D(0,0,1)
 */
void ZoneItem::updateOptimalZone() {
  Vec3D u = projector->det2normal(projector->img2det.map(startHandle->pos()));
  Vec3D v = projector->det2normal(projector->img2det.map(endHandle->pos()));
  // Vector perpendicular to u and v
  Vec3D z = u%v;
  z.normalize();


  Mat3D M;
  M.zero();
  bool hasCrossingSpotMarkers = false;

  foreach (Vec3D s, projector->getSpotMarkerNormals()) {
    if (fabs(s*z)<sin(markerWidth)) {
      M += s^s;
      hasCrossingSpotMarkers = true;
    }
  }

  if (hasCrossingSpotMarkers) {
    M += u^u;
    M += v^v;
    Mat3D L,R;

    M.fastsvd(L, R);
    z = R.transposed()*Vec3D(0,0,1);
  }

  if (z!=zoneNormal) {
    zoneNormal = z;
    emit positionChanged();
  }

  if (projector->isProjectionEnabled()) {
    // Maximal scattering angle in this zone
    Vec3D n(-1,0,0);
    n = n-z*(z*n);
    if (n.norm()<1e-8) {
      n = Vec3D(0,0,1);
    } else {
      n.normalize();
    }

    zoneLines = generatePolygon(zoneNormal, n);
    /*for (int m=0; m<zoneLines.size(); m++)
      for (int n=0; n<zoneLines[m].size(); n++)
        zoneLines[m][n] *= 1000.0; */
    update();
  }
}

QList<QPolygonF> ZoneItem::generatePolygon(const Vec3D& n, const Vec3D& _v) {
  QList<QLineF> imgBorders;
  imgBorders << QLineF(imgRect.topLeft(), imgRect.topRight());
  imgBorders << QLineF(imgRect.topRight(), imgRect.bottomRight());
  imgBorders << QLineF(imgRect.bottomRight(), imgRect.bottomLeft());
  imgBorders << QLineF(imgRect.bottomLeft(), imgRect.topLeft());

  Vec3D v(_v);
  Mat3D M(n, 2.0*M_PI/400);
  QPolygonF zonePoly;
  QList<QPolygonF> zonePolys;

  bool firstOk = false;
  bool lastOk=false;
  bool ok=false;
  QPointF lastP;
  for (int i=0; i<401; i++) {
    QPointF p = projector->det2img.map(projector->normal2det(v, ok));
    ok = ok && imgRect.contains(p);
    if (i>0) {
      if (lastOk != ok) {
        QPointF onBorder;
        foreach (QLineF border, imgBorders) {
          if (border.intersect(QLineF(p, lastP), &onBorder)==QLineF::BoundedIntersection) {
            zonePoly << onBorder;
            break;
          }
        }
      }
      if (ok) {
        zonePoly << p;
      } else if (zonePoly.size()>1) {
        zonePolys << zonePoly;
        zonePoly.clear();
      }
    }
    lastOk = ok;
    lastP = p;
    if (i==1) firstOk=ok;
    v = M*v;
  }
  if (firstOk && ok && !zonePoly.empty()) zonePoly << zonePolys.takeFirst();
  if (zonePoly.size()==400) zonePoly << zonePoly.first();
  if (zonePoly.size()>1) zonePolys << zonePoly;

  return zonePolys;
}

void ZoneItem::setStart(const QPointF& p) {
  startHandle->setPos(p);
}

void ZoneItem::setEnd(const QPointF& p) {
  endHandle->setPos(p);
}

QPointF ZoneItem::getStart() {
  return startHandle->pos();
}

QPointF ZoneItem::getEnd() {
  return endHandle->pos();
}

Vec3D ZoneItem::getMarkerNormal() const {
  return zoneNormal;
}

void ZoneItem::highlight(bool h) {
  setZValue(h ? 10 : 0);
  highlighted = h;
  update();
}

void ZoneItem::slotInvalidateCache() {
  invalidateCache();
}

void ZoneItem::slotSetMaxSearchIndex(int n) {
  setMaxSearchIndex(n);
}

void ZoneItem::setWidth(double v) {
  markerWidth = v*M_PI/360;
  updatePolygon();
  updateOptimalZone();
  update();
}

const char XML_ZoneMarker_element[] = "Marker";
const char XML_ZoneMarker_start[] = "StartHandle";
const char XML_ZoneMarker_end[] = "EndHandle";

void ZoneItem::saveToXML(QDomElement base) {
  QDomElement marker = ensureElement(base, XML_ZoneMarker_element);
  PointToTag(marker, XML_ZoneMarker_start, getStart());
  PointToTag(marker, XML_ZoneMarker_end, getEnd());
}

void ZoneItem::loadFromXML(QDomElement base) {
  if (base.tagName()!=XML_ZoneMarker_element) return;
  for (QDomElement e=base.firstChildElement(); !e.isNull(); e=e.nextSiblingElement()) {
    if (e.tagName()==XML_ZoneMarker_start) {
      setStart(TagToPoint(e, getStart()));
    } else if (e.tagName()==XML_ZoneMarker_end) {
      setEnd(TagToPoint(e, getEnd()));
    }
  }
}

void ZoneItem::colorChanged() {
  update();
}
