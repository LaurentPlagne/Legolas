echo $1
export basename="${1%.*}"
echo $basename

rm -f ${basename}.eps
rm -f ${basename}.pdf


latex ${basename}.tex
dvips -q -E ${basename}.dvi -o ${basename}.eps
epstopdf ${basename}.eps --outfile=${basename}.pdf

xpdf ${basename}.pdf &

#dvipdf ${basename}_tmp.dvi pdf/${basename}.pdf

# cleaning
/bin/rm -f ${basename}.eps ${basename}.dvi ${basename}.aux *.log 

#gv -scale=8 eps/${basename}.eps



