TEX = rapport.tex
SRC = diagramme.eps capture.png
SRCTMP = diagramme.pdf capture.eps
DVISRC = diagramme.eps capture.eps
PDFSRC = diagramme.pdf capture.png

DVI  = $(TEX:.tex=.dvi)
PS   = $(TEX:.tex=.ps)
PSGZ = $(TEX:.tex=.ps.gz)
PDF  = $(TEX:.tex=.pdf)
TMP  = $(TEX:.tex=.aux) $(TEX:.tex=.log) $(TEX:.tex=.out) $(TEX:.tex=.toc) \
       $(TEX:.tex=.tpt) $(SRCTMP)

.SUFFIXES:
.SUFFIXES: .tex .dvi .ps .ps.gz .pdf .eps .png
.PHONY:    default all dvi ps psgz pdf clean cleantmp

.tex.dvi:
	latex $<
	latex $<

.dvi.ps:
	dvips $< -o $@

.ps.ps.gz:
	gzip -9 -c $< > $@

.tex.pdf:
	pdflatex $<
	pdflatex $<
	-thumbpdf $@
	pdflatex $<

.eps.pdf:
	epstopdf $<

.png.eps:
	convert $< $@


#default: all
default: pdf cleantmp

$(DVI): $(DVISRC)
$(PDF): $(PDFSRC)

all:  dvi ps psgz pdf
dvi:  $(DVI)
ps:   $(PS)
psgz: $(PSGZ)
pdf:  $(PDF)

clean:
	rm -f $(DVI) $(PS) $(PSGZ) $(PDF) $(TMP)

cleantmp:
	rm -f $(TMP)
