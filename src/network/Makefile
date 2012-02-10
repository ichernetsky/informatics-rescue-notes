CFLAGS = -pedantic -Wall -Wextra -pthread
LDFLAGS = -pthread

PROGRAMS = date-client-basic \
	   date-client \
	   date-server

.PHONY: all
all: $(PROGRAMS)
all: .gitignore

.gitignore: $(PROGRAMS)
	rm -f $@
	touch $@
	for p in $(PROGRAMS); do \
		echo $$p >> $@; \
	done

.PHONY: clean
clean:
	rm -f $(PROGRAMS) *.o *.pyc
