CD = cd
CP = cp
PDFLATEX = pdflatex
RM = rm -f

NAME = notes
TEXNAMES = $(wildcard *.tex)
PDFNAME = $(NAME).pdf
RELPDFNAME = $(NAME)-$(shell date +%Y%m%d).pdf
LOGNAME = $(NAME).log

.PHONY: all
all: notes slides

.PHONY: release
release: clean notes
	$(CP) $(PDFNAME) $(RELPDFNAME)

.PHONY: notes
notes: $(PDFNAME)

$(PDFNAME): $(TEXNAMES)
	$(PDFLATEX) $(NAME)
	makeindex $(NAME)
	$(PDFLATEX) $(NAME)
	while grep 'Label(s) may have changed' $(LOGNAME) > /dev/null 2>&1 ; do \
		$(PDFLATEX) $(NAME) ; \
        done

.PHONY: slides
slides:
	$(CD) slides && $(MAKE)

.PHONY: clean
clean:
	$(RM) *~ \#* *.aux *.toc $(NAME)*.pdf *.log *.out *.bbl *.blg *.idx *.ilg *.ind
	$(CD) slides && $(MAKE) clean
