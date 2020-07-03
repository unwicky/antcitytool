ifndef CUSTOM_INSTALL_DIR
	CUSTOM_INSTALL_DIR=/usr/local
endif

OBJECTS	=	at_util.o at_xmlparser.o at_vector.o at_string.o at_httpclient.o at_httpclient_pool.o \
    at_bitset.o at_hashmap.o at_hashset.o at_queue.o at_stack.o at_thread_pool.o
LIBS	=	-lpthread -L${CUSTOM_INSTALL_DIR}/lib -lcurl -lxml2
INCLUDE	=	-Iinclude -I/usr/local/include -I${CUSTOM_INSTALL_DIR}/include
OUTPUT	=	libantcitytools.so
CC	=	gcc -g -fpic
	
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
	@if [ ! -d ${CUSTOM_INSTALL_DIR}/include/antcity ]; \
	then \
		mkdir ${CUSTOM_INSTALL_DIR}/include/antcity; \
	fi
	cp -R include/http ${CUSTOM_INSTALL_DIR}/include/antcity
	cp -R include/util ${CUSTOM_INSTALL_DIR}/include/antcity
	cp include/*.h ${CUSTOM_INSTALL_DIR}/include/antcity
	cp libantcitytools.so ${CUSTOM_INSTALL_DIR}/lib
	
clean:
	rm -f *.o *.~ *.bak
	rm -f $(OUTPUT)
	cd tests && make clean
