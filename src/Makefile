src = $(wildcard *.cpp)
hdr = $(wildcard *.h)
obj = $(src:.cpp=.o)


LDFLAGS = -lsfml-graphics -lsfml-audio -lsfml-system -lsfml-window

maze-runner: $(obj)
	g++ -Wall -g -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) maze-runner

PREFIX = /usr/local

.PHONY: install
install: maze-runner
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/maze-runner
#	cp -r resources /usr/share/pacman/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/maze-runner
