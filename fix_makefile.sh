#!/bin/sh
# Copyright (C) 2012-2013  Mattia Basaglia
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

# This script fixes the makefile generated by qmake
# it ensures that the Makefile will keep calling this script when it regenerates itself

check_string="# Makefile already fixed"

if  ! grep -q "$check_string" Makefile ; then
    sed -i -e "s/^dist/buggydist/g" -e "s/mydist/dist/g" Makefile # fix dist target
    sed -i -e "s/Makefile: /Makefile: fix_makefile.sh /" Makefile # add this as dependency
    sed -ri -e 's/(\$\(QMAKE\).*)/\1 ; .\/fix_makefile.sh/g' Makefile # add this on regenerate
    echo $check_string >>Makefile #add trailing line to avoid re-execution of this
fi

