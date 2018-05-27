CELL_TOP := /opt/cell/sdk

PROGRAMS_spu := engine
LIBRARY_embed := engine.a

CPPFLAGS := -funroll-loops
IMPORTS = md5.o /opt/cell/sdk/prototype/usr/spu/lib/libspucrypto.a 

ifdef CELL_TOP
 include $(CELL_TOP)/buildutils/make.footer
else
 include ../../../../buildutils/make.footer
endif

