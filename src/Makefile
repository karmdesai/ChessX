CXX = g++
CXXFLAGS = -std=c++14 -g -MMD -Wall -Wextra \
		   -I board -I computer -I pieces
EXEC = chess
OBJECTS = board.o bishop.o king.o knight.o pawn.o \
					abstractPlayer.o human.o computer1.o computer2.o \
					computer3.o computer4.o piece.o queen.o rook.o \
					driver.o nullPiece.o
DEPENDS = ${OBJECTS:.o=.d}
# The only real change from the canonical Makefile,
# 	mirror changes in CXXFLAGS with -I
VPATH=board:computer:pieces

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS}
