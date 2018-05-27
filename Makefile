CELL_TOP := /opt/cell/sdk

DIRS := spu
PROGRAM_ppu := Riga

IMPORTS = spu/engine.a -lspe2 -lpthread

ifdef CELL_TOP
  include $(CELL_TOP)/buildutils/make.footer
else
  include ../../../../buildutils/make.footer
endif
