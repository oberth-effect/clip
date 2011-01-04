#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include <QGraphicsObject>

class CircleItem : public QGraphicsObject
{
  Q_OBJECT
public:
  explicit CircleItem(double r, QGraphicsItem *parent = 0);

protected:
  QVariant itemChange(GraphicsItemChange change, const QVariant & value );
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  QRectF boundingRect() const;
  QPainterPath shape() const;
signals:
  void positionChanged();
  void itemClicked();
public slots:
  void setPosNoSig(const QPointF& p);
  void setRadius(double);
  void setColor(QColor);
  void setLineWidth(double);
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
  double radius;
  double lineWidth;
  bool skipNextPosChange;
  QColor color;
};

#endif // CIRCLEITEM_H
