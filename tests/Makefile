ifndef CUSTOM_INSTALL_DIR
	CUSTOM_INSTALL_DIR=/usr/local
endif

LIBS	=	-lpthread -L${CUSTOM_INSTALL_DIR}/lib -lxml2 -lcurl
INCLUDE	=	-I../include -I/usr/local/include -I${CUSTOM_INSTALL_DIR}/include -I${CUSTOM_INSTALL_DIR}/include/libxml2
OUTPUT  = XmlParser HtmlParser Vector String HttpClient Bitset \
    Util HashMap HashSet Queue Stack ThreadPool
CC = gcc -g
	
all:$(OUTPUT)
	
.c.o:
	$(CC) $(INCLUDE) -c $(filter %.c, $^) 
	
.o:
	$(CC) $(INCLUDE) -o $@ $^ 

XmlParser: ../at_xmlparser.o ../at_vector.o ../at_string.o test_xmlparser.o 
	$(CC) -o $@ $^ ${LIBS}

HtmlParser: ../at_xmlparser.o ../at_vector.o ../at_string.o test_htmlparser.o 
	$(CC) -o $@ $^ ${LIBS}

Vector: ../at_vector.o test_vector.o
	$(CC) -o $@ $^ ${LIBS}

String: ../at_string.o test_string.o
	$(CC) -o $@ $^ ${LIBS}

HttpClient: ../at_string.o ../at_httpclient.o test_httpclient.o
	$(CC) -o $@ $^ ${LIBS}

Bitset: ../at_bitset.o test_bitset.o
	$(CC) -o $@ $^ ${LIBS}
	
Util: ../at_util.o test_util.o ../at_string.o
	$(CC) -o $@ $^ ${LIBS}
	
HashMap: ../at_util.o ../at_hashmap.o test_hashmap.o
	$(CC) -o $@ $^ ${LIBS}
	
HashSet: ../at_util.o ../at_hashset.o ../at_hashmap.o test_hashset.o
	$(CC) -o $@ $^ ${LIBS}
	
Queue: ../at_queue.o test_queue.o
	$(CC) -o $@ $^ ${LIBS}
	
Stack: ../at_stack.o test_stack.o
	$(CC) -o $@ $^ ${LIBS}
	
ThreadPool: ../at_thread_pool.o ../at_stack.o test_thread_pool.o
	$(CC) -o $@ $^ ${LIBS}
	
clean:
	rm -f *.o *.~ *.bak
	rm -f $(OUTPUT)
	
dep:
	mkdep -p$(VPATH) $(INCLUDE) $(SOURCES) > .depend

ifneq ($(wildcard .depend),)
include .depend
endif
