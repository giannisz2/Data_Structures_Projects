# Το Makefile αυτό βρίσκεται στο root ολόκληρου του project και χρησιμεύει για
# να κάνουμε εύκολα compile πολλά τμήματα του project μαζί. Το Makefile αυτό
# καλεί το make στα διάφορα directories ως
#   $(MAKE) -C <dir> <target>
# το οποίο είναι ισοδύναμο με το να τρέξουμε make <target> μέσα στο directory <foo>

# Ολα τα directories μέσα στο programs directory
PROGRAMS = $(subst programs/, , $(wildcard programs/*))

# Compile: όλα
all: tests

.PHONY: tests
tests:
	$(MAKE) -C tests all

# Εκτέλεση: όλα
run: run-tests

run-tests:
	$(MAKE) -C tests run

# Εκκαθάριση
clean:
	$(MAKE) -C tests clean