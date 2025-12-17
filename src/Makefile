PROJ_NAME=ted
ALUNO=eduardoLanza
LIBS= -lm

OBJETOS= \
	main.o \
	args.o \
	lista.o \
	listadj.o \
	digraph.o \
	boundingbox.o \
	enderecos.o \
	entity.o \
	funcoesqry.o \
	viaFile.o \
	picture.o \
	retangulo.o \
	circulo.o \
	linha.o \
	hash_table.o \
	tratar_strings.o \
	STreap.o

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

main.o: main.c args.h picture.h viaFile.h funcoesqry.h

args.o: args.c args.h

lista.o: lista.c lista.h

listadj.o: listadj.c listadj.h lista.h

digraph.o: digraph.c digraph.h listadj.h lista.h

boundingbox.o: boundingbox.c boundingbox.h STreap.h

enderecos.o: enderecos.c enderecos.h retangulo.h

entity.o: entity.c entity.h circulo.h linha.h retangulo.h texto.h

funcoesqry.o: funcoesqry.c funcoesqry.h picture.h boundingbox.h lista.h digraph.h listadj.h entidade.h circulo.h linha.h tratar_strings.h

viaFile.o: viaFile.c viaFile.h picture.h digraph.h boundingbox.h hash_table.h

picture.o: picture.c picture.h entity.h lista.h

retangulo.o: retangulo.c retangulo.h

circulo.o: circulo.c circulo.h

linha.o: linha.c linha.h

hash_table.o: hash_table.c hash_table.h

tratar_strings.o: tratar_strings.c tratar_strings.h

STreap.o: STreap.c STreap.h

