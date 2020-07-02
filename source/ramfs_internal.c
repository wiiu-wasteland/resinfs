/*
 * ramfs_tree.c
 * Tree of nodes containing filesystem entries
 * Copyright (C) 2019 rw-r-r-0644
 */

#include "ramfs_internal.h"
#include <string.h>
#include <stdlib.h>

typedef struct ustarHeader ustarHeader;
struct ustarHeader __attribute__((packed))
{
	char fname[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char fsize[12];
	char mtime[12];
	char chksum[8];
	char typeflag[1];
	char link[100];
	char tar_magic[6];
	char tar_version[2];
	char username[32];
	char groupname[32];
	char dev_maj[8];
	char dev_min[8];
	char prefix[155];
	char pad[12];
};

/* find or create the requested path */
static ramfsNode *ramfsGetPath(ramfsPrivate *ramfs, ramfsNode *node, const char *path, int create)
{
	char *wpath = strdup(path), *wpath_s;
	for (char *entry = strtok_r(wpath, "/", &wpath_s); node && entry; entry = strtok_r(NULL, "/", &wpath_s))
	{
		ramfsNode *sub = NULL;
		while (node->type == RAMFS_LINK)
			node = node->lnk;
		if (node->type == RAMFS_DIR)
		{
			if (!strcmp(entry, "."))
				sub = node;
			else if (!strcmp(entry, ".."))
				sub = node->up;
			else
				for (sub = node->sub; sub && strcmp(entry, sub->name); sub = sub->next);
		}
		if (!sub && create)
		{
			sub = calloc(1, sizeof(ramfsNode));
			sub->name = strdup(entry);
			sub->type = RAMFS_DIR;
			sub->inode = ++ramfs->inodes;
			sub->up = node;
			sub->next = node->ptr;
			node->ptr = sub;
		}
		node = sub;
	}
	free(wpath);
	return node;
}

static unsigned oct2bin(char *c, int size)
{
	unsigned n = 0;
	while (size-- > 0)
		n = (n * 8) + ((*(c++) - '0') % 8);
	return n;
}

/* ramfsCreateNode: create a node and all its parent directories */
ramfsNode *ramfsCreateNode (ramfsPrivate *ramfs, ustarHeader *hdr) 
{
	/* make sure target file type is supported */
	ramfsNodeType type = 1 << oct2bin(hdr->typeflag, 1);
	if (!(type & (RAMFS_FILE | RAMFS_LINK | RAMFS_DIR)))
		return;
	
	/* create or find the target node */
	ramfsNode *n = ramfsGetPath(&ramfs->root, hdr->fname, 1);
	
	/* add attributes from tar header */
	n->size = oct2bin(hdr->fsize, 11);
	n->mtime = oct2bin(hdr->mtime, 11);
	n->type = type;
	
	if (type == RAMFS_FILE) {
		n->ptr = (void *)(hdr + 1);
	} else if (type == RAMFS_LINK) {
		n->ptr = ramfsGetPath(&ramfs->root, hdr->link, 1);
	}
	
	if (isdir)
	{
		c->type = RAMFS_DIR;
		c->sub = NULL;	
	}
	else
	{
		c->type = RAMFS_FILE;
		c->ptr = content;
	}
	
	return n;
}

/* ramfsGetNode: parse path string and return the corresponding node */
ramfsNode *ramfsFindPath (ramfsPrivate *ramfs, const char *path)
{
	/* remove the device label when present */
	char* colon = strchr(path, ':');
	if (colon)
		path = colon + 1;
	
	/* find the node */
	ramfsNode *n = (*path == '/') ? &ramfs->root : ramfs->cwd;
	return ramfsGetPath(n, path, 0);
}

void ramfsInternalInit (ramfsPrivate *ramfs, void *start, void *end)
{
	ustarHeader *hdr = (ustarHeader *)start;
	ustarHeader *ehdr = (ustarHeader *)end;
	
	/* create root node */
	memset(&ramfs->root, 0, sizeof(ramfsNode));
	ramfs->root.name = "resin:";
	ramfs->root.type = RAMFS_DIR;
	
	/* init tree */
	ramfs->cwd = &ramfs->root;
	ramfs->inodes = 0;
	
	/* create nodes from the ustar file */
	while ((hdr < ehdr) && !memcmp(hdr->tar_magic, "ustar", 5))
	{
		ramfsNode *n = ramfsCreateNode(ramfs, hdr);
		hdr += ((n->size + 511) / 512) + 1;
	}
}

void ramfsInternalDestroy (ramfsPrivate *ramfs)
{
	/* deallocate the nodes tree */
	ramfsNode *cur = ramfs->root.sub;
	while (cur != &ramfs->root)
	{
		ramfsNode *del = cur;
		if ((cur->type == RAMFS_DIR) && cur->sub)
		{
			cur = cur->sub;
			continue;
		}
		if (cur->next)
		{
			cur = cur->next;
		}
		else
		{
			cur = cur->up;
			cur->sub = NULL;
		}
		free(del->name);
		free(del);
	}
}
