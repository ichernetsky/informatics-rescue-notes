RM = rm -f
TOUCH = touch

PROGRAMS = date-client-basic \
	   date-client \
	   date-server

.PHONY: all
all: $(PROGRAMS)
all: .gitignore

.gitignore: $(PROGRAMS)
	$(RM) $@
	$(TOUCH) $@
	for d in $(PROGRAMS); do \
		echo $$d >> $@; \
	done

%: %.c netaux.c netaux.h
	$(CC) $(CFLAGS) netaux.c $< -o $@

.PHONY: clean
clean:
	$(RM) $(PROGRAMS) *.o
