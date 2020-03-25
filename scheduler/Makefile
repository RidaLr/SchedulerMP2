CC=gcc
FLAGS=-g -pedantic -Wall -Werror  

all: sched

list.o: sched.h list.c
	$(CC) $(FLAGS) -std=c99 -c list.c

sched: sched.c sched.h list.o
	$(CC) $(FLAGS) -std=c99 -o sched sched.c list.o

%.tex: sched  
	./sched $* > $*.tex

%.source: sched.c
	csplit -f split sched.c "/--Scheduler $*--/" {1}
	cp split01 $*.source


simulate_%: %.tex %.source
	cat template.tex > preview.tex
	/bin/echo -E "\\input{$*.tex}" >> preview.tex
	/bin/echo -E "\\lstinputlisting{$*.source}" >> preview.tex
	/bin/echo -E "\\end{document}" >> preview.tex

	latex preview.tex
	dvips preview.dvi -f > preview.ps
	evince preview.ps

rapport: fcfs.tex rr.tex sjf.tex srtf.tex fcfs.source rr.source \
	sjf.source srtf.source
	latex rapport.tex
	dvips rapport.dvi -f > rapport.ps
	evince rapport.ps

clean:
	rm -f sched *.o rr.tex fcfs.tex sjf.tex srtf.tex *.source split* preview.tex *.aux *.ps *.dvi *.log
