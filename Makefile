PDFLATEX = pdflatex
RM = rm -f

NAME = notes
TEXNAMES = $(wildcard *.tex)
PDFNAME = $(NAME).pdf
LOGNAME = $(NAME).log

.PHONY: all
all: $(PDFNAME)

$(PDFNAME): $(TEXNAMES)
	$(PDFLATEX) $(NAME)
	while grep 'Label(s) may have changed' $(LOGNAME) > /dev/null 2>&1 ; do \
		$(PDFLATEX) $(NAME) ; \
        done

.PHONY: clean
clean:
	$(RM) *~ \#* *.aux *.toc $(PDFNAME) $(LOGNAME)
