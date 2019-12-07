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

typedef enum
{
	RAMFS_DIR,
	RAMFS_FILE,
} ramfsNodeType;

typedef struct ramfsNode ramfsNode;
struct ramfsNode
{
	char *name;
	uint32_t inode;
	uint32_t size;
	ramfsNodeType type;
	time_t mtime;
	union {
		/* cont: file content for files */
		char *cont;
		/* ent: subdirectory entry for directories */
		ramfsNode *ent;
	};
	ramfsNode *next;
	ramfsNode *up;
};

void ramfsCreateNode (const char *cpath, time_t mtime, int32_t isdir, uint32_t size, char *content);
void ramfsDestroyNodes (ramfsNode *n, int32_t recursion);
ramfsNode *ramfsGetNode (const char *cpath);

void ramfsCreateFromTar(char *ptr, char *end);

extern devoptab_t ramfsDevoptab;
extern ramfsNode ramfsRoot;
extern ramfsNode *ramfsCwd;

#endif
