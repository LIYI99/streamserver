include config.mak

CSRC = rbtree.c
SLSRC = 

COBJS = ${CSRC:%.c=%.o}
SOBJS = ${SLSRC:%.s=%.o}
LOPTS = -g

TARGET = librbtree.a

$(TARGET) : $(COBJS) $(SOBJS) 
	$(AR) -r $(TARGET)  *.o

%.o: %.c
	$(CC) -c $(COPS) -I. -o $@ $<

%.o : %.s
	$(CC) -c $(COPS) -I. -o $@ $<

share:
	$(CC) $(COPS) rbtree.c -shared -o librbtree.so
clean :
	/bin/rm -f *.o
	/bin/rm -r *.a
test:
	gcc  test.c $(TARGET)  -o rbtree_test -lpthread

