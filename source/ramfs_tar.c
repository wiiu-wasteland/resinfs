/*
 * ramfs_tar.c
 * ustar file parsing
 * Copyright (C) 2019 rw-r-r-0644
 */

#include "ramfs_internal.h"
#include <stdint.h>
#include <string.h>

/* ustar file header definition */
typedef struct __attribute__((packed))
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
} tar_header_t;

/* oct2bin: convert octal string to unsigned */
static uint32_t oct2bin(char *c, int32_t size)
{
	uint32_t n = 0;
	while (size-- > 0)
		n = (n * 8) + (*(c++) - '0');
	return n;
}

/* ramfsCreateFromTar: create node tree entries for tar's files and folders */
void ramfsCreateFromTar(char *ptr, char *end)
{
	tar_header_t *hdr = (tar_header_t *)ptr;
	tar_header_t *ehdr = (tar_header_t *)end;

	/* loop trough all tar entries */
	while ((hdr < ehdr) && !memcmp(hdr->tar_magic, "ustar", 5))
	{
		/* get entry attributes */
		uint32_t fsize = oct2bin(hdr->fsize, 11);
		uint32_t mtime = oct2bin(hdr->mtime, 11);
		int32_t isdir = hdr->typeflag[0] == '5';
		int32_t isfile = hdr->typeflag[0] == '0';

		/* only files and directories are supported */
		if (isfile || isdir)
			/* create a node for the entry */
			ramfsCreateNode(hdr->fname, mtime, isdir, fsize, (char *)(hdr + 1));

		/* go to the next entry */
		hdr += ((fsize + 511) / 512) + 1;
	}
}
