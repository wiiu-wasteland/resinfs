/*
 * ramfs.c
 * ramfs library functions
 * Copyright (C) 2019 rw-r-r-0644
 */

#include "ramfs_internal.h"
#include <sys/iosupport.h>

/* ramfs binary symbols */
extern char _binary_ramfs_tar_start[];
extern char _binary_ramfs_tar_end[];

/* ramfs initialization flag */
static int ramfs_initialised = 0;

/* ramfsMountDefault: mount the default ramfs to the default mountpoint */
int ramfsMountDefault(void)
{
	/* already initialized */
	if (ramfs_initialised)
		return 0;

	/* create ramfs file entries */
	ramfsCreateFromTar(_binary_ramfs_tar_start, _binary_ramfs_tar_end);

	/* add the ramfs devoptab to devices list */
	if (AddDevice(&ramfsDevoptab) == -1)
	{
		ramfsDestroyNodes(NULL, 0);
		return -1;
	}

	/* set the intialized flag */
	ramfs_initialised = 1;

	return 0;
}

/* ramfsUnmountDefault: unmount the default ramfs mounpoint */
int ramfsUnmountDefault(void)
{
	/* never initialized */
	if (!ramfs_initialised)
		return -1;

	/* remove the ramfs devoptab from devices list */
	RemoveDevice("resin:");

	/* deallocate the file tree */
	ramfsDestroyNodes(NULL, 0);

	/* clear the initialized flag */
	ramfs_initialised = 0;

	return 0;
}


int ramfsMount(char *mountpoint, void *ramfsStart, void *ramfsEnd)
{
	
}

int ramfsUnmount(char *mountpoint)
{
	
}
