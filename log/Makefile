DOTFILES = $(wildcard *.dot)
DOTPNG = $(patsubst %.png, %.dot, $(DOTFILES))

.PHONY : dot
dot : $(DOTPNG)
	@dot -Tpng $(DOTFILES) -O $(DOTPNG)

%.dot : %.png
	@dot -Tpng $< $@

clean :
	rm *.png *.dot
