all: main.tex
	xelatex main.tex
	xelatex main.tex

check:
	./check/compile.sh

stress:
	./stress/run.sh

clean:
	rm -f *.log *.aux *.out *.pdf *.toc
	rm -rf .check-build .stress-build

.PHONY: check stress clean
