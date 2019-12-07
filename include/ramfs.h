#ifndef _RAMFS_H
#define _RAMFS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ramfsInit: mount ramfs
 * returns 0 when successful or a negative value for errors
 */
int ramfsInit(void);

/*
 * ramfsExit: unmount ramfs
 * returns 0 when successful or a negative value for errors
 */
int ramfsExit(void);


#ifdef __cplusplus
}
#endif

#endif
