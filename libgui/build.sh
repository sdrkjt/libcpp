#!/bin/bash
echo  Script generated by libgui
echo  Building VSPLOT...
qtmake-qt4
qmake
make clean
make
mkdir -p bin
ar cru bin/libvsplot.a plot.o moc_plot.o sdrgui.o
ranlib bin/libvsplot.a
cd ..
echo VSPLOT built in bin/ directory
