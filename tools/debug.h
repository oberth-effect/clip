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

#ifndef DEBUG_H
#define DEBUG_H

 
#ifdef CLIP_DEBUG

#include <iostream>


template<class T> void printRect(const T& r) {
  std::cout << "Rect = " << r.left() << " <-> " << r.right() << " | " << r.top() << " <-> " << r.bottom() << "  (" << r.width() << " x " << r.height() << ")" << std::endl;
}


template<class T> void printPoint(const QString& s, const T& r) {
  std::cout << qPrintable(s) << " = (" << r.x() << "," << r.y() << ")" << std::endl;
}
#endif

#endif // DEBUG_H
