CP = cp
PDFLATEX = pdflatex
RM = rm -f

NAME = notes
TEXNAMES = $(wildcard *.tex)
PDFNAME = $(NAME).pdf
RELPDFNAME = $(NAME)-$(shell date +%Y%m%d).pdf
LOGNAME = $(NAME).log

.PHONY: release
release: clean all
	$(CP) $(PDFNAME) $(RELPDFNAME)

.PHONY: all
all: $(PDFNAME)

$(PDFNAME): $(TEXNAMES)
	$(PDFLATEX) $(NAME)
	while grep 'Label(s) may have changed' $(LOGNAME) > /dev/null 2>&1 ; do \
		$(PDFLATEX) $(NAME) ; \
        done

.PHONY: clean
clean:
	$(RM) *~ \#* *.aux *.toc $(NAME)*.pdf $(LOGNAME) missfont.log
