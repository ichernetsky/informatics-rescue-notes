RM = rm -f
TOUCH = touch

PROGRAMS = client-seq

.PHONY: all
all: $(PROGRAMS)
all: .gitignore

.gitignore: $(PROGRAMS)
	$(RM) $@
	$(TOUCH) $@
	for d in $(PROGRAMS); do \
		echo $$d >> $@; \
	done

%: %.c netaux.c
	$(CC) $(CFLAGS) netaux.c $< -o $@

.PHONY: clean
clean:
	$(RM) $(PROGRAMS) *.o
