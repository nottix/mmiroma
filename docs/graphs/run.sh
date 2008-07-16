#!/bin/bash
#gnuplot script

XLB="Intervallo migrazione (sec)"
YLB="Tempo impiegato (sec)"

if [ "$1" = "" ]; then
	echo "Usage: run.sh <source_data>"
	exit -1
fi

echo $1
echo "set size 1.5" > aux.gnu
echo "set output \"$1.eps\"" >> aux.gnu
echo "set title \"Test\"" >> aux.gnu
echo "set xlabel \"$XLB\"" >> aux.gnu
echo "set ylabel \"$YLB\"" >> aux.gnu
echo "plot \"$1\" with linespoints pointtype 2 pointsize 3 lt 1 linewidth 8" >> aux.gnu
gnuplot script.input
wait
epstopdf $1.eps
wait
rm $1.eps
