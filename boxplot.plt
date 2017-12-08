set term png size 1280, 720
set out "plot.png"
set style fill solid 0.25 border -1
set style boxplot outliers pointtype 7
set style data boxplot

set title 'My Plot' font 'Arial,14';
header = "`head -1 data.dat | cut -b 2-`"
set for [i=1:words(header)] xtics (word(header, i) i)
plot for [i=1:3] 'data.dat' using (i):i notitle