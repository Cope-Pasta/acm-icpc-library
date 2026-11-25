all: main.tex
	xelatex main.tex
	xelatex main.tex

check:
	./check/compile.sh

clean:
	rm -f *.log *.aux *.out *.pdf *.toc
	rm -rf .check-build

.PHONY: check clean
