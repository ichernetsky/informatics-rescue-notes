CD = cd
PDFLATEX = pdflatex
RM = rm -f

SLIDES = network
SOURCES = $(addsuffix /slides.tex,$(SLIDES))
PDFS = $(addsuffix /slides.pdf,$(SLIDES))

.PHONY:
all: $(PDFS)

%.pdf: %.tex
	$(CD) `dirname $@` && $(PDFLATEX) slides

.PHONY: clean
clean:
	for d in $(SLIDES); do \
		$(CD) $$d; \
			$(RM) *.log *.aux *.pdf *.snm *.toc *.out *.nav; \
		$(CD) ..; \
	done
