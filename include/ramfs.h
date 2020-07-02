#ifndef _RAMFS_H
#define _RAMFS_H

#ifdef __cplusplus
extern "C" {
#endif


/*
 * ramfsMount: mount specified ramfs
 * returns 0 when successful or a negative value for errors
 * 
 * Parameters
 *     label: the ramfs mountpoint name
 *     start: pointer to the start of a ustar file in memory
 *     end:   pointer to the end of a ustar file in memory 
 * 
 * Note: the specified ustar file must be kept in memory
 *       while the ramfs is mounted, otherwise undefined
 *       behaviour will occur
 */
int ramfsMount(char *label, void *start, void *end);


/*
 * ramfsUnmount: unmount ramfs
 * returns 0 when successful or a negative value for errors
 * 
 * Parameters
 *     label: the ramfs mountpoint name
 */
int ramfsUnmount(char *label);


/*
 * ramfsMountDefault: mount the default ramfs, generated by ramfs.mk
 * returns 0 when successful or a negative value for errors
 */
static inline int ramfsMountDefault(void)
{
	extern char _binary_ramfs_tar_start[];
	extern char _binary_ramfs_tar_end[];
	return ramfsMount("resinfs", _binary_ramfs_tar_start, _binary_ramfs_tar_end);
}


/*
 * ramfsUnmountDefault: unmount the default ramfs, generated by ramfs.mk
 * returns 0 when successful or a negative value for errors
 */
static inline int ramfsUnmountDefault(void)
{
	return ramfsUnmount("resinfs");
}


#ifdef __cplusplus
}
#endif

#endif
