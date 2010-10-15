RM = rm -f
TOUCH = touch

SOURCES = $(wildcard *.c)
PROGRAMS = $(basename $(SOURCES))

.PHONY: all
all: $(PROGRAMS)
all: .gitignore

.gitignore: $(PROGRAMS)
	$(RM) $@
	$(TOUCH) $@
	for d in $(PROGRAMS); do \
		echo $$d >> $@; \
	done

%: %.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) .gitignore *.o $(PROGRAMS)
