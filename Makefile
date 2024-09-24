OFLAGS = 
DFLAGS = -g3
LDFLAGS = -lsqlite3

.phony = clean all

all : gendb

clean :
	@rm -f gendb gendb.o

gendb : gendb.o
	$(CC) -o gendb $(DFLAGS) $(OFLAGS) $(LDFLAGS) gendb.o

gendb.o : gendb.c nc_data.h sqlite_schema.h
	$(CC) -c -o gendb.o $(DFLAGS) $(OFLAGS) gendb.c
