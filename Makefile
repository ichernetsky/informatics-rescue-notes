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

.PHONY: clean
clean:
	$(RM) *.aux $(PDFNAME) $(LOGNAME)
