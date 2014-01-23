VERSION =	0.5

INCLUDE =	-I/usr/local/include/Sockets -I/usr/devel/include \
		-I/usr/devel/include/Sockets \
		-I/usr/local/include/libxml2

CFLAGS =	-Wall -g -O2 $(INCLUDE) -MD `Sockets-config`
CPPFLAGS =	$(CFLAGS)

LIBS =		-L/usr/local/lib -lSockets -lssl -lcrypto -lxml2 \
		-L/usr/devel/lib -lhttpd -lmysqlwrapped \
		-L/usr/local/lib/mysql -lmysqlclient -lz -lpthread

PROGS =		server
all:		$(PROGS)

COMMON =	BaseXMLFile.o Configuration.o 

server:		server.o ServerSocket.o $(COMMON) \
		ServerHandler.o Deliver.o Mime.o
		g++ -o $@ $^ $(LIBS)


clean:
		rm -f *.o *~ *.d slask $(PROGS)

db:
		mysqldump httpd > str-httpd
		sql2class -build -global -lib libhttpd str-httpd -namespace db -overwrite -getset

-include	*.d

DOCDIR =	/usr/local/apache/www.alhem.net/htdocs/project/example8
tar:
		tar czf $(DOCDIR)/example8-$(VERSION).tar.gz \
			*.h *.cpp Makefile \
			str-httpd \
			README \
			config.xml gpl.txt

