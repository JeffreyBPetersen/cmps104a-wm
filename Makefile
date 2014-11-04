# Jeffrey Petersen | jebpeter@ucsc.edu

GCC        = g++ -g -O0 -Wall -Wextra -std=gnu++11
MKDEP      = g++ -MM -std=gnu++11
VALGRIND   = valgrind --leak-check=full --show-reachable=yes

MKFILE     = Makefile
DEPFILE    = Makefile.dep
SOURCES    = astree.cpp auxlib.cpp cppstrtok.cpp \
lyutils.cpp main.cpp stringset.cpp
HEADERS    = astree.h auxlib.h cppstrtok.h lyutils.h stringset.h
OBJECTS    = ${CLGEN} ${HYGEN} ${CYGEN} ${SOURCES:.cpp=.o}
EXECBIN    = oc
SRCFILES   = ${HEADERS} ${SOURCES} ${MKFILE}
SMALLFILES = ${DEPFILE} README parser.y scanner.l
CHECKINS   = ${SRCFILES} ${SMALLFILES}
LSOURCES   = scanner.l
YSOURCES   = parser.y
CLGEN      = yylex.cpp
HYGEN      = yyparse.h
CYGEN      = yyparse.cpp
LREPORT    = yylex.output
YREPORT    = yyparse.output

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${GCC} -o${EXECBIN} ${OBJECTS}

%.o : %.cpp
	${GCC} -c $<

${CLGEN} : ${LSOURCES}
	flex --outfile=${CLGEN} ${LSOURCES} 2>${LREPORT}
	- grep -v '^ ' ${LREPORT}

${CYGEN} ${HYGEN} : ${YSOURCES}
	bison --defines=${HYGEN} --output=${CYGEN} ${YSOURCES}

testscan :
	flex scanner.l
	g++ lex.yy.c -lfl -o testscan

style :
	checksource ${CHECKINS}

ci : push
	checksource ${CHECKINS}

push:
	git push

up-%:
	git add ${CHECKINS}
	git commit -m "$(@:up-%=%)"
	git push

clean :
	- rm ${OBJECTS}

spotless : clean
	- rm ${EXECBIN} ${DEPFILE} \
	     test.out test.err yylex.output yyparse.output

${DEPFILE} :
	${MKDEP} ${SOURCES} >${DEPFILE}

deps :
	- rm ${DEPFILE}
	${MAKE} --no-print-directory ${DEPFILE}

submit:
	submit cmps104a-wm.f14 asg2 ${CHECKINS}

include Makefile.dep

test : ${EXECBIN}
	${VALGRIND} ${EXECBIN} foo.oc 1>test.out 2>test.err
