# CLIP (Cologne Laue Indexation Program)

To download latest executable binary, go to the [Latest release](https://gitlab.mff.cuni.cz/alsa/clip4/-/releases/permalink/latest)

Program was originally developed by Olaf Schumann, it was originally written in Python as a part of his [thesis](http://nbn-resolving.de/urn:nbn:de:hbz:38-32507) and then ported to pure C++ (version 4). Program was no longer maintained after 2011. In 2024 we have moved source to github and start maintaining it again. For support it is best to use old still available resources:

 - [Original webapge](https://clip4.sourceforge.net/)
 - [PDF Manual](https://clip4.sourceforge.net/manual.pdf)
 - [ILL tutorial](https://www.ill.eu/users/instruments/instruments-list/orientexpress/manuals-and-tutorials/clip)
 - [Mercurial repository on Sourceforge](https://sourceforge.net/p/clip4/code/ci/default/tree/)
 - [Github repository of O.J. Schumann](https://github.com/ojschumann/clip)

## Description 

This is the Cologne Laue Indexation Program or short CLIP

It is a program to index and refine laue exposures. It can do simulations as well
as index and refine from spots and zones marked on a Laue photograph.
Afterward the rotations to obtain a special orientation could be calculated
for arbitary goniometer configurations. The program is written in the
C++ programing language and the cross platform toolkit Qt.
It runs on Windows, Linux and MacOS X.

## Getting Started

### Installation

Download the zip file from the [release section](https://gitlab.mff.cuni.cz/alsa/clip4/-/releases) and unzip it somewhere. Then run clip.exe - no installation is required.


## Acknowledgement


O.J.Schumann would like to thank Gregory Tucker for his contributions to the
Multiwire-Import filter.


## License


Clip is Copyright (C) 2008-2011 by O.J.Schumann (o.j.schumann@gmail.com)
Changes in 2024 by P.Čermák and Š. Venclík from Charles University.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.

This software uses the [Qt library](http://qt.io) licensed under the terms
of the GNU General Public License. Parts of the library are covered under different other
licenses, see [qt webpage](https://doc.qt.io/qt-5/licensing.html).

This software uses the [Eigen library](http://eigen.tuxfamily.org) licensed under the terms
of the GNU Lesser General Public License.
