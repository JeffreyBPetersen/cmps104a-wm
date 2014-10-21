# Jeffrey Petersen | jebpeter@ucsc.edu

GCC        = g++ -g -O0 -Wall -Wextra -std=gnu++11
MKDEP      = g++ -MM -std=gnu++11
VALGRIND   = valgrind --leak-check=full --show-reachable=yes

MKFILE     = Makefile
DEPFILE    = Makefile.dep
SOURCES    = auxlib.cpp cppstrtok.cpp main.cpp stringset.cpp
HEADERS    = auxlib.h cppstrtok.h stringset.h
OBJECTS    = ${SOURCES:.cpp=.o}
EXECBIN    = oc
SRCFILES   = ${HEADERS} ${SOURCES} ${MKFILE}
SMALLFILES = ${DEPFILE}
CHECKINS   = ${SRCFILES} ${SMALLFILES}

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o${EXECBIN} ${OBJECTS}

%.o : %.cpp
	${GCC} -c $<

ci : push
	checksource ${CHECKINS}

push:
	git push

up-%: spotless
	git add *
	git commit -m "$(@:git-%=%)"
	git push

clean :
	- rm ${OBJECTS}

spotless : clean
	- rm ${EXECBIN} ${DEPFILE} \
	     test.out test.err

${DEPFILE} :
	${MKDEP} ${SOURCES} >${DEPFILE}

dep :
	- rm ${DEPFILE}
	${MAKE} --no-print-directory ${DEPFILE}

submit:
	submit cmps104a-wm.f14 asg1 ${SOURCES} ${HEADERS} Makefile README

include Makefile.dep

test : ${EXECBIN}
	${VALGRIND} ${EXECBIN} foo.oc 1>test.out 2>test.err
