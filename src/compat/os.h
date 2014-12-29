/* vifm
 * Copyright (C) 2014 xaizek.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef VIFM__COMPAT__OS_H__
#define VIFM__COMPAT__OS_H__

#include <dirent.h> /* DIR dirent opendir() readdir() closedir() */

#ifndef _WIN32

#include <sys/stat.h> /* mkdir() */
#include <unistd.h> /* access() chdir() chmod() lstat() rename() */

#include <stdlib.h> /* system() */

#define os_access access
#define os_chdir chdir
#define os_chmod chmod
#define os_closedir closedir
#define os_opendir opendir
#define os_readdir readdir
#define os_lstat lstat
#define os_mkdir mkdir
#define os_rename rename
#define os_stat stat
#define os_system system

#else

#include <fcntl.h> /* *_OK */

/* Not straight forward for Windows and not very important. */
#define os_lstat os_stat

struct stat;

int os_access(const char pathname[], int mode);

int os_chdir(const char path[]);

int os_chmod(const char path[], int mode);

int os_closedir(DIR *dirp);

DIR * os_opendir(const char name[]);

struct dirent * os_readdir(DIR *dirp);

int os_rename(const char oldpath[], const char newpath[]);

int os_mkdir(const char pathname[], int mode);

int os_stat(const char path[], struct stat *buf);

int os_system(const char command[]);

#endif

#endif /* VIFM__COMPAT__OS_H__ */

/* vim: set tabstop=2 softtabstop=2 shiftwidth=2 noexpandtab cinoptions-=(0 : */
/* vim: set cinoptions+=t0 : */