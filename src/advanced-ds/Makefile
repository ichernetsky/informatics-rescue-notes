RM = rm -f
TOUCH = touch

OCAML = ocamlc
OFLAGS = -thread -package batteries -linkpkg

PROGRAMS = trie \
	   bloom \
	   rbtree \
	   btree

.PHONY: all
all: $(PROGRAMS)
all: .gitignore

.gitignore: $(PROGRAMS)
	$(RM) $@
	$(TOUCH) $@
	for d in $(PROGRAMS); do \
		echo $$d >> $@; \
	done

%: %.ml
	ocamlfind $(OCAML) $(OFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) $(PROGRAMS) *.cmi *.cmo *.cmx *.o
