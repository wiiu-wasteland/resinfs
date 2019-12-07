/*
 * ramfs.c
 * ramfs library functions
 * Copyright (C) 2019 rw-r-r-0644
 */

#include "ramfs_internal.h"
#include <sys/iosupport.h>
#include <stdint.h>

/* romfs binary symbols */
extern char _binary_romfs_tar_start[];
extern char _binary_romfs_tar_end[];

/* romfs initialization flag */
static int32_t ramfs_initialised = 0;

/* ramfsInit: intialize romfs */
int32_t ramfsInit(void)
{
	/* already initialized */
	if (ramfs_initialised)
		return 0;

	/* create romfs file entries */
	ramfsCreateFromTar(_binary_romfs_tar_start, _binary_romfs_tar_end);

	/* add the romfs devoptab to devices list */
	if (AddDevice(&ramfsDevoptab) == -1)
	{
		ramfsDestroyNodes(NULL, 0);
		return -1;
	}

	/* set the intialized flag */
	ramfs_initialised = 1;

	return 0;
}

/* ramfsExit: exit romfs */
int32_t ramfsExit(void)
{
	/* never initialized */
	if (!ramfs_initialised)
		return -1;

	/* remove the romfs devoptab from devices list */
	RemoveDevice("resin:");

	/* deallocate the file tree */
	ramfsDestroyNodes(NULL, 0);

	/* clear the initialized flag */
	ramfs_initialised = 0;

	return 0;
}
