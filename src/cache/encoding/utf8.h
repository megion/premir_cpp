#ifndef SRC_CACHE_UTF8_H
#define SRC_CACHE_UTF8_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>
#include <iconv.h>

namespace cache {
	namespace encoding {

		typedef unsigned int ucs_char_t;  /* assuming 32bit int */

		size_t display_mode_esc_sequence_len(const char *s);
		int utf8_width(const char **start, size_t *remainder_p);
		int utf8_strnwidth(const char *string, int len, int skip_ansi);
		int utf8_strwidth(const char *string);
		int is_utf8(const char *text);
		int is_encoding_utf8(const char *name);
		int same_encoding(const char *, const char *);
		__attribute__((format (printf, 2, 3)))
			int utf8_fprintf(FILE *, const char *, ...);

		extern const char utf8_bom[];
		extern int skip_utf8_bom(char **, size_t);

		//#ifndef NO_ICONV
		char *reencode_string_iconv(const char *in, size_t insz,
				iconv_t conv, int *outsz);
		char *reencode_string_len(const char *in, int insz,
				const char *out_encoding,
				const char *in_encoding,
				int *outsz);
		//#else
		//static inline char *reencode_string_len(const char *a, int b,
		//const char *c, const char *d, int *e)
		//{ if (e) *e = 0; return NULL; }
		//#endif

		static inline char *reencode_string(const char *in,
				const char *out_encoding,
				const char *in_encoding)
		{
			return reencode_string_len(in, strlen(in),
					out_encoding, in_encoding,
					NULL);
		}

		int mbs_chrlen(const char **text, size_t *remainder_p, const char *encoding);

		/*
		 * Returns true if the path would match ".git" after HFS case-folding.
		 * The path should be NUL-terminated, but we will match variants of both ".git\0"
		 * and ".git/..." (but _not_ ".../.git"). This makes it suitable for both fsck
		 * and verify_path().
		 */
		int is_hfs_dotgit(const char *path);
	}

}

#endif
