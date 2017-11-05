PWD_PATH		= /home/liyi/streamserver/basic/buddy 
CONF_PATH 		=$(PWD_PATH)/config.mak



#output path
PUT_INCLUDE 	=$(PWD_PATH)/include
PUT_OBJ			=$(PWD_PATH)/obj


#D_QUEUE_DIR 	=  
#INCLUDE_PATH 	= -I$(D_LIST_DIR)
#INCLUDE_PATH 	+= -I$(D_QUEUE_DIR)
INCLUDE_PATH 	+= -I$(MEM_POOL_DIR)
#INCLUDE_PATH 	+= -I$(THREAD_POOL_DIR)
#INCLUDE_PATH	+= -I$(D_DARR_DIR)

COPS = $(INCLUDE_PATH) -D 
#ARM_ASM -O2 -O3 -Wall
CROSS_COMPILE =
#arm-goke-linux-uclibcgnueabi-

CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
AS=$(CROSS_COMPILE)as
STRIP=$(CROSS_COMPILE)strip



 


