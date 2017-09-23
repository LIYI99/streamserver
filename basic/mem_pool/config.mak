PWD_PATH		= /home/710S/streamMeadia/test/IPC/basic
CONF_PATH 		=$(PWD_PATH)/config.mak
D_LIST_DIR		=$(PWD_PATH)/d_list
D_QUEUE_DIR		=$(PWD_PATH)/d_queue
MEM_POOL_DIR	=$(PWD_PATH)/mem_pool
THREAD_POOL_DIR =$(PWD_PATH)/thread_pool
D_DARR_DIR		=$(PWD_PATH)/d_darr

DATA_SERVER_DIR =$(PWD_PATH)/data_server
AVTSENC_DIR		=$(PWD_PATH)/avtsenc

AAC_DIR			=$(PWD_PATH)/aac

#output path
PUT_INCLUDE 	=$(PWD_PATH)/include
PUT_OBJ			=$(PWD_PATH)/obj


INCLUDE_PATH 	= -I$(D_LIST_DIR)
INCLUDE_PATH 	+= -I$(D_QUEUE_DIR)
INCLUDE_PATH 	+= -I$(MEM_POOL_DIR)
INCLUDE_PATH 	+= -I$(THREAD_POOL_DIR)
INCLUDE_PATH	+= -I$(D_DARR_DIR)

COPS = $(INCLUDE_PATH) -D ARM_ASM -O2 -O3 -Wall
CROSS_COMPILE =arm-goke-linux-uclibcgnueabi-

CC=$(CROSS_COMPILE)gcc
AR=$(CROSS_COMPILE)ar
AS=$(CROSS_COMPILE)as
STRIP=$(CROSS_COMPILE)strip



 


