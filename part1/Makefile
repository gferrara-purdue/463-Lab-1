#Flags
WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Werror
COVERAGE = -fprofile-arcs -ftest-coverage
GCC = gcc -std=c99 -g $(WARNING) $(ERROR) $(COVERAGE)
VAL = valgrind --tool=memcheck --log-file=memcheck.txt --leak-check=full --verbose

#Sources
SRCS = httpclient.gferrara.c

#Compile for debug
part1: 
	$(GCC) $(TESTFALGS) $(SRCS) -o httpclient

#Submit
test: clean
	gcc httpclient.gferrara.c -o httpclient

test1: test
	./httpclient dtunes.ecn.purdue.edu 80 /ece463/lab1/path_short.txt > output1.out
	diff output1.out test_short.txt
	
test2: test
	./httpclient dtunes.ecn.purdue.edu 80 /ece463/lab1/path_very_long.txt > output2.out
	diff output2.out test_very_long.txt

testmemory1: clean part1
	$(VAL) 	./httpclient dtunes.ecn.purdue.edu 80 /ece463/lab1/path_short.txt > output_MEM_1.out
	
testmemory2: clean part1
	$(VAL)	./httpclient dtunes.ecn.purdue.edu 80 /ece463/lab1/path_very_long.txt > output_MEM_2.out
	
#Etc
clean: 
	rm -f httpclient *.o *.out *.zip *gcda *gcno *gcov	
zipitandsubmitit:
	zip httpclient httpclient.gferrara.c
depend:
	makedepend $(SRCS)
	
	



















