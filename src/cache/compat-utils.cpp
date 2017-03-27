#include "cache/compat-utils.h"

namespace cache {

	int is_absolute_path(const char *path) {
		return is_dir_sep(path[0]) || has_dos_drive_prefix(path);
	}

}


