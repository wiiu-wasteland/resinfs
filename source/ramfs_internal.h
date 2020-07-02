/*
 * ramfs_internal.h
 * Internal ramfs library functions
 * Copyright (C) 2019 rw-r-r-0644
 */

#ifndef _RAMFS_INTERNAL_H
#define _RAMFS_INTERNAL_H

#include <sys/iosupport.h>
#include <stdint.h>
#include <time.h>

typedef enum ramfsNodeType ramfsNodeType;
typedef struct ramfsNode ramfsNode;
typedef struct ramfsPrivate ramfsPrivate;
typedef struct ustarHeader ustarHeader;

enum ramfsNodeType
{
	RAMFS_FILE = (1 << 0),
	RAMFS_LINK = (1 << 1),
	RAMFS_DIR  = (1 << 5),
};

stuct ramfsPrivate
{
	ramfsNode root;
	ramfsNode *cwd;
	uint32_t inodes;
};



struct ramfsNode
{
	char *name;
	uint32_t inode;
	uint32_t size;
	ramfsNodeType type;
	time_t mtime;
	union {
		/* RAMFS_FILE: file content */
		char *ptr;
		/* RAMFS_DIR:  first directory entry */
		ramfsNode *sub;
		/* RAMFS_LINK: link target node */
		ramfsNode *lnk;
	};
	ramfsNode *next;
	ramfsNode *up;
};

void ramfsCreateNode (ramfsPrivate *ramfs, ustarHeader *hdr);
void ramfsDestroyNodes (ramfsPrivate *ramfs);
ramfsNode *ramfsFindPath (ramfsPrivate *ramfs, const char *path);

void ramfsCreateFromTar(char *ptr, char *end);

extern devoptab_t ramfsDevoptab;

#endif
