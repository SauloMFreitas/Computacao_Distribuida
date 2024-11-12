all: tempo

tempo: Tempo.o smpl.o rand.o
	$(LINK.c) -o $@ -Bstatic Tempo.o smpl.o rand.o -lm

smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g -Wno-all smpl.c

Tempo.o: Tempo.c smpl.h
	$(COMPILE.c) -g  -Wno-all Tempo.c

rand.o: rand.c
	$(COMPILE.c) -g -Wno-all rand.c

clean:
	$(RM) *.o tempo relat saida
