
DEBUG_OP = -g3 -D_DEBUG 
CPP_DEBUG_OP = -D_DEBUG 

OPTIMIZATION_OP = -O0 
CPP_OPTIMIZATION_OP = 

COMPILE_FLAGS = $(DEBUG_OP) $(OPTIMIZATION_OP) -Wall -c -fmessage-length=0 

CPP_COMPILE_FLAGS = $(CPP_DEBUG_OP) $(CPP_OPTIMIZATION_OP) 

LINK_FLAGS = 

ifeq ($(STRIP_INFO),off)
LINK_FLAGS = 
else
ifeq ($(STRIP_INFO),on)
LINK_FLAGS = -s
endif
endif

AR_FLAGS = 

EDC_COMPILE_FLAGS = 