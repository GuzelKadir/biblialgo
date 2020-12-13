CC = cc
CFLAGS = -Wall -O2 -g -Iinclude -std=c99

CXX = c++
CXXFLAGS = -Wall -O2 -g -Iinclude -Itests/googletest/googletest/include -Itests/googletest/googletest

LIBDIR = lib

LIBALGO_OBJ = $(LIBDIR)/array.o $(LIBDIR)/list.o $(LIBDIR)/tree.o

RUNTESTS_OBJ = tests/googletest/googletest/src/gtest-all.o tests/test_array.o tests/test_list.o tests/test_tree.o run_tests.o

all: libalgo.a run_tests

test: run_tests
	./run_tests

libalgo.a: $(LIBALGO_OBJ)
	ar cr $@ $(LIBALGO_OBJ)

run_tests: $(RUNTESTS_OBJ) libalgo.a
	$(CXX) -o $@ $(RUNTESTS_OBJ) -L. -lalgo -lpthread

clean:
	rm -f $(LIBALGO_OBJ)
	rm -f $(RUNTESTS_OBJ)

mrproper: clean
	rm -f libalgo.a
	rm -f run_tests

ARCHIVEDIR = biblialgo

archive: clean
	test "$(LIBDIR)" = "lib"
	rm -rf $(ARCHIVEDIR)
	mkdir $(ARCHIVEDIR)
	cp -r include lib tests Makefile run_tests.cc $(ARCHIVEDIR)
	tar zcvf $(ARCHIVEDIR).tar.gz $(ARCHIVEDIR)
	rm -rf $(ARCHIVEDIR)
