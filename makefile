.SUFFIXES: .cxx .hxx
DIR = `basename $(CURDIR)

MAZE = Maze

CMAZE = $(MAZE).cxx

XMAZE = $(MAZE).x

HEADER1 = Maze.hxx

CFLAGS = -lncurses

CXX = g++

.PHONY: all tar clean

all: $(XMAZE)

$(XMAZE): $(CMAZE) $(HEADER1)
	$(CXX) $< $(CFLAGS) -o $(XMAZE) 

clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out

tar: clean
	(cd ../;tar zcvf $(DIR).tar.gz $(DIR))
