OBJECTS	=	at_util.o at_xmlparser.o at_vector.o at_string.o at_httpclient.o at_httpclient_pool.o \
    at_bitset.o at_hashmap.o at_hashset.o at_queue.o at_stack.o at_thread_pool.o
LIBS	=	-lpthread -lcurl -L/usr/local/lib/libxml2 -lxml2
INCLUDE	=	-Iinclude -I/usr/local/include -I/usr/local/include/libxml2
OUTPUT	=	libantcitytools.so
CC	=	gcc -g -fpic
INSTALLDIR	=	/usr/local
	
all:$(OUTPUT)
	cd tests && make

VPATH = src:src/http:src/util
	
.c.o:
	$(CC) $(INCLUDE) -c $(filter %.c, $^) 
	
.o:
	$(CC) $(INCLUDE) -o $@ $^ 
	
$(OUTPUT): $(OBJECTS)
	$(CC) -shared -o $@ $^ ${LIBS}
	
test:
	cd tests && make
	
install:
	@if [ ! -d ${INSTALLDIR}/include/antcity ]; \
	then \
		mkdir ${INSTALLDIR}/include/antcity; \
	fi
	cp -R include/http ${INSTALLDIR}/include/antcity
	cp -R include/util ${INSTALLDIR}/include/antcity
	cp include/*.h ${INSTALLDIR}/include/antcity
	cp libantcitytools.so ${INSTALLDIR}/lib
	
clean:
	rm -f *.o *.~ *.bak
	rm -f $(OUTPUT)
	cd tests && make clean
	
dep:
	mkdep -p$(VPATH) $(INCLUDE) $(SOURCES) > .depend

ifneq ($(wildcard .depend),)
include .depend
endif
