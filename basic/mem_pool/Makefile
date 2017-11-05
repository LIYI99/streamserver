include config.mak

CSRC = mem_pool.c darr_queue.c
SLSRC = 

COBJS = ${CSRC:%.c=%.o}
SOBJS = ${SLSRC:%.s=%.o}
LOPTS = -g

TARGET = libmem_pool.a

$(TARGET) : $(COBJS) $(SOBJS) 
	$(AR) -r $(TARGET)  *.o

%.o: %.c
	$(CC) -c $(COPS) -I. -o $@ $<

%.o : %.s
	$(CC) -c $(COPS) -I. -o $@ $<

share:
	$(CC) $(COPS) mem_pool.c darr_queue.h -shared -o libmem_pool.so
clean :
	/bin/rm -f *.o
	/bin/rm -r *.a
test:
	gcc  $(COPS) -g  test.c $(TARGET) librbtree.a  -o test_mem_pool -lpthread

