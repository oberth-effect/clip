/**************************************************************************
  Copyright (C) 2011 schumann

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
**************************************************************************/

#include "core/reflection.h"
#include "core/projectorfactory.h"
#include "ui/stereocfg.h"
#include "diffractingstereoprojector.h"

DiffractingStereoProjector::DiffractingStereoProjector(QObject* _parent) :
    StereoProjector(_parent)
{
}

auto DiffractingStereoProjector::getInstance() -> Projector* {
  return new DiffractingStereoProjector();
}

auto DiffractingStereoProjector::projectorName() const -> QString {
  return QString("DiffractingStereoProjector");
}

auto DiffractingStereoProjector::displayName() -> QString {
  return QString("Stereographic Projection");
}

auto DiffractingStereoProjector::configWidget() -> QWidget* {
  return new StereoCfg(this);
}


auto DiffractingStereoProjector::scattered2det(const Vec3D& n) const -> QPointF {
  Vec3D v=localCoordinates*n;
  double s=1.0+v.x();
  return (s>1e-5) ? QPointF(v.y()/s, v.z()/s) : QPointF();
}

auto DiffractingStereoProjector::scattered2det(const Vec3D& n, bool& b) const -> QPointF {
  Vec3D v=localCoordinates*n;
  double s=1.0+v.x();
  b = (s>1e-5);
  return (b) ? QPointF(v.y()/s, v.z()/s) : QPointF();
}

auto DiffractingStereoProjector::det2scattered(const QPointF& p) const -> Vec3D {
  double x=p.x();
  double y=p.y();
  double n=1.0/(x*x+y*y+1.0);
  return localCoordinates.transposed()*Vec3D(n*(1.0-x*x-y*y), 2*x*n, 2*y*n);
}

auto DiffractingStereoProjector::det2scattered(const QPointF& p, bool& b) const -> Vec3D {
  double x=p.x();
  double y=p.y();
  double n=1.0/(x*x+y*y+1.0);
  b=true;
  return localCoordinates.transposed()*Vec3D(n*(1.0-x*x-y*y), 2*x*n, 2*y*n);
}

auto DiffractingStereoProjector::normal2det(const Vec3D& n) const -> QPointF {
  return scattered2det(normal2scattered(n));
}

auto DiffractingStereoProjector::normal2det(const Vec3D& n, bool& b) const -> QPointF {
  Vec3D t(normal2scattered(n, b));
  if (b) {
    return scattered2det(t, b);
  } else {
    return QPointF();
  }
}

auto DiffractingStereoProjector::det2normal(const QPointF& p) const -> Vec3D {
  return scattered2normal(det2scattered(p));
}

auto DiffractingStereoProjector::det2normal(const QPointF& p, bool& b) const -> Vec3D {
  Vec3D t(det2scattered(p, b));
  if (b) {
    return scattered2normal(t, b);
  } else {
    return Vec3D();
  }
}

auto DiffractingStereoProjector::project(const Reflection &r, QPointF &p) -> bool {
  bool reflectionInRange=false;
  QPair<double, double> limits = validOrderRange(r.Q, r.Qscatter);
  for (int n : r.orders) {
    if ((limits.first<=n) and (n<=limits.second)) {
      reflectionInRange=true;
      break;
    }
  }
  if (not reflectionInRange)
    return false;

  Vec3D v=localCoordinates*r.scatteredRay;
  double s=1.0+v.x();
  if (s>1e-5) {
    s=1.0/s;
    p.setX(v.y()*s);
    p.setY(v.z()*s);
    return true;
  }
  return false;
}

bool DiffractingStereoProjector_registered = ProjectorFactory::registerProjector("DiffractingStereoProjector", &StereoProjector::getInstance);
