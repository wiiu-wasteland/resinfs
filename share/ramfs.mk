ifneq ($(strip $(V)), 1)
	Q ?= @
endif

ifneq (, $(shell which gtar 2>/dev/null))
	TAR := gtar
else
	TAR := tar
endif

ifneq ($(strip $(RAMFS_DIR)),)

TOPDIR		?=	.

RAMFS_TARGET	:=	app.ramfs.o

%.ramfs.o: $(TOPDIR)/$(RAMFS_DIR)
	@echo RAMFS $(notdir $@)
	$(Q)$(TAR) -H ustar -cvf ramfs.tar -C $(TOPDIR)/$(RAMFS_DIR) .
	$(Q)$(PREFIX)ld -r -b binary ramfs.tar -o $@
	@rm -f ramfs.tar

else

RAMFS_TARGET	:=

endif
