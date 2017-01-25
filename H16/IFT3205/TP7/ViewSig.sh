#! /bin/sh
# ViewSig.sh

NameWithExt=${1}
NameWitoutExt=${NameWithExt%.*}
NameWithExtEps=${NameWitoutExt}.eps

echo visu de $NameWithExtEps

if [ -f Out.input ]
then
rm Out.input
fi

if [ -f Out.eps ]
then
rm Out.eps
fi

echo set terminal postscript color eps > Out.input
echo set output \"${NameWithExtEps}\" >> Out.input
echo set grid  >> Out.input
echo " "   >> Out.input
echo plot \'${1}\' with lines >> Out.input

gnuplot Out.input
rm Out.input

gv ${NameWithExtEps}&
