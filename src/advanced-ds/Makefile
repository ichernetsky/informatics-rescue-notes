apps = rbtree \
       trie

.PHONY: all
all: $(apps)
all: .gitignore

.gitignore: $(apps)
	rm -f $@
	touch $@
	for d in $(apps); do \
		echo $$d >> $@; \
	done

%: %.hs
	ghc $< -o $@

.PHONY: clean
clean:
	rm -f $(apps) *.hi *.o
