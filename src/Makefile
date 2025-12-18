PROJ_NAME=ted
ALUNO=eduardoLanza
LIBS= -lm

OBJETOS= \
	main.o \
	lista.o \
	fila.o \
	quadras.o \
	via.o \
	qry.o \
	drawSvg.o \
	dotFile.o \
	path.o \
	streap.o \
	digraph.o \
	hash.o

# Compilador
CC=gcc

# Flags
CFLAGS= -ggdb -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration
LDFLAGS= -O0

$(PROJ_NAME): $(OBJETOS)
	$(CC) -o $(PROJ_NAME) $(OBJETOS) $(LIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

# DEPENDÊNCIAS

main.o: main.c quadras.h via.h qry.h dotFile.h drawSvg.h lista.h path.h

hash.o: hash.c hash.h

lista.o: lista.c lista.h

fila.o: fila.c fila.h lista.h

quadras.o: quadras.c quadras.h streap.h hash.h

via.o: via.c via.h

qry.o: qry.c qry.h quadras.h via.h streap.h drawSvg.h digraph.h

drawSvg.o: drawSvg.c drawSvg.h

dotFile.o: dotFile.c dotFile.h streap.h

path.o: path.c path.h digraph.h lista.h

streap.o: streap.c streap.h

digraph.o: digraph.c digraph.h lista.h
