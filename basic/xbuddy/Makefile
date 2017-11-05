include config.mak

CSRC = xbuddy_mempool.c
SLSRC = 

COBJS = ${CSRC:%.c=%.o}
SOBJS = ${SLSRC:%.s=%.o}
LOPTS = -g

TARGET = libbuddy_mempool.a

$(TARGET) : $(COBJS) $(SOBJS) 
	$(AR) -r $(TARGET)  *.o

%.o: %.c
	$(CC) -c $(COPS) -I. -o $@ $<

%.o : %.s
	$(CC) -c $(COPS) -I. -o $@ $<

share:
	$(CC) $(COPS) buddy_mempool.c  -shared -o libbuddy_mempool.so
clean :
	/bin/rm -f *.o
	/bin/rm -r *.a
test:
	gcc  $(COPS) -g  test.c $(TARGET)  -o test_buddy_mempool -lpthread


