#!/bin/bash

srcdir=../..
QMAKE=qmake-qt5-windows $srcdir/configure.sh
package=`echo __package__-__version__ | $srcdir/info_preprocessor.sh`

if [ -d $package ]
then
  rm -rf $package
fi
mkdir $package

if [ -f $package-windows.zip ]
then
  rm -f $package-windows.zip
fi

make || exit

cp release/knotter.exe $package
cp -R $srcdir/data $package
zip -r $package-windows.zip $package