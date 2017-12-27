# Cose del compilatore
CXX = g++
CFLAGS = -std=c++11 -g
# Oggetti
OBJ = obj/main.o obj/application.o obj/sdlApp.o obj/launchpad.o
# Librerie da linkare
LIBS = -lSDL2 -lSDL2_gfx -lrtmidi
# Cartelle sorgenti e oggetti
OBJDIR = obj
SRCDIR = src
# File eseguibile
OUT = eseguibile.out

# Compile
$(OUT): $(OBJDIR) $(OBJ) $(OBJDIR)
	$(CXX) $(OBJ) $(LIBS) -o $(OUT) $(CFLAGS)

# Compile and run
all: $(OUT) run

install: all

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: run
run:
	./$(OUT)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $< -c -o $@ $(CFLAGS)

.PHONY: clean

clean:
	rm $(OBJDIR) -rf
	rm $(OUT) -f

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
