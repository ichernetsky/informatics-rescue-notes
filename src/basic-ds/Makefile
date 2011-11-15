CFLAGS = -g -Wall -Wextra -std=c99

binaries = $(sort $(basename $(wildcard *.c)))

.PHONY: all
all: $(binaries)
all: .gitignore

$(binaries) : % : %.c

.gitignore: $(binaries)
	rm -f $@
	touch $@
	for d in $(binaries); do \
	    echo $$d >> $@; \
	done

.PHONY: clean
clean:
	rm -f $(binaries)
