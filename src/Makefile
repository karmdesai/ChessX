CXX = g++
CXXFLAGS = -std=c++14 -g -MMD -Wall -Wextra \
		   -I board -I computer -I pieces -I observers
EXEC = chess
OBJECTS = board.o computer.o bishop.o king.o knight.o pawn.o piece.o queen.o rook.o driver.o nullPiece.o observer.o studio.o subject.o textObserver.o
DEPENDS = ${OBJECTS:.o=.d}
# The only real change from the canonical Makefile,
# 	mirror changes in CXXFLAGS with -I
VPATH=board:computer:pieces:observers

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS}
