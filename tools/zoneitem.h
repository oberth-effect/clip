#ifndef ZONEITEM_H
#define ZONEITEM_H

#include "tools/propagatinggraphicsobject.h"

#include <QPen>

#include "tools/vec3D.h"

class Projector;
class SignalingEllipseItem;
class GraphicsEllipseItem;

class ZoneItem : public PropagatingGraphicsObject
{
  Q_OBJECT
public:
  ZoneItem(const QPointF&, const QPointF&, Projector* p, QGraphicsItem* parent=0);
  ~ZoneItem();
  QRectF boundingRect() const;
  QPainterPath shape () const;
  void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *);
  void setStart(const QPointF&);
  void setEnd(const QPointF&);
  QPointF getStart();
  QPointF getEnd();
  void highlight(bool);
  bool isHighlighted();
private slots:
  void updatePolygon();
  void updateOptimalZone();
signals:
  void zoneChanged();
protected:
  QRectF imgRect;
  QList<QPolygonF> generatePolygon(const Vec3D&, const Vec3D&);
  SignalingEllipseItem* startHandle;
  SignalingEllipseItem* endHandle;
  qreal radius;
  bool highlighted;
  QPen pen;
  Projector* projector;
  QList<QPolygonF> zoneLines;
  QList<QPolygonF> zonePolys;
};

#endif // ZONEITEM_H
