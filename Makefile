CCC = g++
CCFLAGS = -std=c++11

FILES = Project.cc Elements.cc Game.cc Graphics.cc Level.cc Logic.cc
OBJECTS = Elements.o Game.o Graphics.o Level.o Logic.o Project.o
SFMLINCLUDES = -lsfml-graphics -lsfml-window -lsfml-system

all: Project.cc Elements.cc Game.cc Graphics.cc Level.cc Logic.cc
	$(CCC) $(CCFLAGS) $(FILES) -o project $(SFMLINCLUDES)

# 'make clean' tar bort objektkodsfiler och 'core' (minnesdump).
clean:
	@ \rm -rf *.o *.gch core

# 'make zap' tar även bort det körbara programmet och reservkopior (filer
# som slutar med tecknet '~').
zap: clean
	@ \rm -rf project *~

