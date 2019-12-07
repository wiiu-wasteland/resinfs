ifneq ($(strip $(V)), 1)
	Q ?= @
endif

ifneq (, $(shell which gtar))
	Q += g
endif

ifneq ($(strip $(RAMFS_DIR)),)

TOPDIR		?=	.

RAMFS_TARGET	:=	app.ramfs.o

%.ramfs.o: $(TOPDIR)/$(RAMFS_DIR)
	@echo ROMFS $(notdir $@)
	$(Q)tar -H ustar -cvf ramfs.tar -C $(TOPDIR)/$(RAMFS_DIR) .
	$(PREFIX)ld -r -b binary ramfs.tar -o $@
	@rm -f ramfs.tar

else

RAMFS_TARGET	:=

endif
