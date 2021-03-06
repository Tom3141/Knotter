# Copyright (C) 2012-2014  Mattia Basaglia
#
# Knotter is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Knotter is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/image_exporter.hpp \
    $$PWD/xml_loader_v2.hpp \
    $$PWD/xml_loader_v3.hpp \
    $$PWD/xml_exporter.hpp \
    $$PWD/xml_loader_v4.hpp \
    $$PWD/xml_loader.hpp

SOURCES += \
    $$PWD/image_exporter.cpp \
    $$PWD/xml_loader_v2.cpp \
    $$PWD/xml_loader_v3.cpp \
    $$PWD/xml_exporter.cpp \
    $$PWD/xml_loader_v4.cpp \
    $$PWD/xml_loader.cpp
