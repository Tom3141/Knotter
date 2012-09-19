#!/bin/sh
# Copyright (C) 2012  Mattia Basaglia
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

# This script generates the user guide html and man page from the docbook sources
# WARNING this script may not be very portable
# Usage: doc_builder.sh [clean|pdf|obfuscate]
#   clean   -   remove old files
#   pdf     -   generate pdf (and only pdf)
#   obfuscate   After generation, obfuscate HTML email adresses,
#               useful when publishing the online make spammer life more difficult


package=`./get_info.sh name`


if  [ "$1" = "clean" ] ; then
    echo "Removing old files..."
    rm -f user_guide/*.html user_guide/man_page.xml man/$package.1.gz
    exit 0
elif [ "$1" = "pdf" ] ; then
    dblatex -P latex.output.revhistory=0 manual.xml
fi

# man page
./info_preprocessor.sh user_guide/man_page.in.xml >user_guide/man_page.xml
xmlto -o man --skip-validation man user_guide/man_page.xml 
gzip -9f man/$package.1



cd user_guide ################################################################## NOTE cd user_guide

# html manual
xmlto --skip-validation  \
    --stringparam html.stylesheet=style.css \
    --stringparam admon.graphics=1 \
    --stringparam admon.graphics.extension=.svg \
    --stringparam admon.graphics.path=img/ \
    --stringparam navig.graphics=1 \
    --stringparam navig.graphics.extension=.svg \
    --stringparam navig.graphics.path=img/ \
    --stringparam use.id.as.filename=1 \
    --stringparam chunk.section.depth=0 \
    xhtml manual.xml

rcc -project | sed 's|<qresource|\0 prefix="/doc" |' >doc.qrc


# Obfuscate email addresses by hex-encoding them
if  [ "$1" = "obfuscate" ] ; then
    #grep -Eo "<a[^>]+href=['\"]mailto:[^'\"]+['\"][^>]*>[^<]+</a>" *.html
    echo "Obfuscating email addresses..."
    for html in `find -name \*.html`
    do
        echo $html:
        for email in `grep -Eo [a-z.]+@[a-z.]+ $html`
        do
            echo "  $email"
            obfuscated=`echo -n $email | hexdump -v -e '/1 "<<AMP>>#x%02X;"'`
            mailto=`echo -n mailto: | hexdump -v -e '/1 "<<AMP>>#x%02X;"'`
            sed -i -e "s/$email/$obfuscated/g" -e "s/mailto:/$mailto/g" -e 's/<<AMP>>/\&/g' $html
        done
    done
fi
