all: main.tex
	xelatex main.tex
	xelatex main.tex

check:
	./check/compile.sh

format:
	find code -name '*.cpp' -exec clang-format -i {} +

format-check:
	find code -name '*.cpp' -exec clang-format --dry-run --Werror {} +

stress:
	./stress/run.sh

clean:
	rm -f *.log *.aux *.out *.pdf *.toc
	rm -rf .check-build .stress-build

.PHONY: check format format-check stress clean
