#ifndef SRC_CACHE_COMPAT_UTILS_H
#define SRC_CACHE_COMPAT_UTILS_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#ifndef is_dir_sep
static inline int git_is_dir_sep(int c)
{
	return c == '/';
}
#define is_dir_sep git_is_dir_sep
#endif

#ifndef has_dos_drive_prefix
static inline int git_has_dos_drive_prefix(const char *path)
{
	return 0;
}
#define has_dos_drive_prefix git_has_dos_drive_prefix
#endif


namespace cache {

	int is_absolute_path(const char *path);

}

#endif
