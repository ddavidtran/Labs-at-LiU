
EXECS=blurc thresc
MPICXX?=mpicxx
CXXFLAGS= -std=c++11

all: ${EXECS}

clean:
	rm -f ${EXECS}

blurc: blurmain.cpp
	${MPICXX} ${CXXFLAGS} -o blurc blurmain.cpp blurfilter.cpp

thresc: thresmain.cpp
	${MPICXX} ${CXXFLAGS} -o thresc thresmain.cpp thresfilter.cpp
