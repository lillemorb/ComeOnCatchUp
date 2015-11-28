# Makefile for ComeOnCatchUp
CCC = g++
CCFLAGS = -std=c++11

FILES = Project.cc
OBJECTS = Elements.o Game.o GameSounds.o Graphics.o Level.o Logic.o 
SFMLLIBS = -lsfml-system -lsfml-audio -lsfml-graphics -lsfml-window 

# 'make' kör detta, bygger hela programmet
all: Project.cc Elements.o Game.o GameSounds.o Graphics.o Level.o Logic.o 
	$(CCC) $(CCFLAGS) $(FILES) -o project $(OBJECTS) $(SFMLLIBS)

Elements.o: Elements.h Elements.cc
	$(CCC) $(CCFLAGS) -c Elements.cc -o Elements.o

Level.o: Level.h Level.cc
	$(CCC) $(CCFLAGS) -c Level.cc -o Level.o

Graphics.o: Graphics.h Graphics.cc
	$(CCC) $(CCFLAGS) -c Graphics.cc -o Graphics.o

Logic.o: Logic.h Logic.cc
	$(CCC) $(CCFLAGS) -c Logic.cc -o Logic.o

Game.o: Game.h Game.cc
	$(CCC) $(CCFLAGS) -c Game.cc -o Game.o

GameSounds.o: GameSounds.h GameSounds.cc
	$(CCC) $(CCFLAGS) -c GameSounds.cc -o GameSounds.o

# 'make clean' tar bort objektkodsfiler och 'core' (minnesdump).
clean:
	@ \rm -rf *.o *.gch core

# 'make zap' tar även bort det körbara programmet och reservkopior (filer
# som slutar med tecknet '~').
zap: clean
	@ \rm -rf project *~

