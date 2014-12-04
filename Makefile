# Jeffrey Petersen | jebpeter@ucsc.edu

GCC        = g++ -g -O0 -Wall -Wextra -std=gnu++11
MKDEP      = g++ -MM -std=gnu++11
VALGRIND   = valgrind --leak-check=full --show-reachable=yes

MKFILE     = Makefile
DEPFILE    = Makefile.dep
SOURCES    = astree.cpp auxlib.cpp \
lyutils.cpp main.cpp stringset.cpp symtable.cpp
HEADERS    = astree.h auxlib.h lyutils.h stringset.h symtable.h
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

fakesubmit :
	cp ${CHECKINS} oc-programs

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
	- rm ${EXECBIN} ${DEPFILE} yylex.output yyparse.output

${DEPFILE} : ${HYGEN}
	${MKDEP} ${SOURCES} >${DEPFILE}

deps :
	- rm ${DEPFILE}
	${MAKE} --no-print-directory ${DEPFILE}

submit:
	submit cmps104a-wm.f14 asg4 ${CHECKINS}

include Makefile.dep
