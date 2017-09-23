include config.mak

CSRC = mem_pool.c
SLSRC = 

COBJS = ${CSRC:%.c=%.o}
SOBJS = ${SLSRC:%.s=%.o}
LOPTS = -g

TARGET = libmem_pool.a

$(TARGET) : $(COBJS) $(SOBJS) 
#$(AR) -r libmem_pool.a  *.o

%.o: %.c
	$(CC) -c $(COPS) -I. -o $@ $<

%.o : %.s
	$(CC) -c $(COPS) -I. -o $@ $<

share:
	$(CC) $(COPS) mem_pool.c -shared -o libmem_pool.so
clean :
	/bin/rm -f *.o

test:
	$(CC) $(INCLUDE_PATH) test_any_thread.c libmem_pool.a libdarr_queue.a libthread_pool.a -lpthread  -o test_mem_pool

test2:
	$(CC) $(INCLUDE_PATH) test_one_thread.c libmem_pool.a libdarr_queue.a libthread_pool.a -lpthread  -o test_mem_pool2
	sudo cp test_mem_pool2 /rootfs
