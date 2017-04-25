#ifndef SRC_CACHE_STR_STRBUF_UTILS_H
#define SRC_CACHE_STR_STRBUF_UTILS_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include <exception>
#include <stdexcept>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "cache/str/StringBuffer.h"

namespace cache {
namespace str {

int is_rfc3986_reserved(char ch);
int is_rfc3986_unreserved(char ch);

bool starts_with(const char* str, const char* prefix);
void strbuf_addbufPercentquote(StringBuffer* dst, const StringBuffer* src);

// void addUniqueAbbrev(const StringBuffer *a, const unsigned char *sha1, int
// abbrev_len);

// int strbuf_cmp(const StringBuffer *a, const StringBuffer *b) {
// int len = a->len < b->len ? a->len: b->len;
// int cmp = std::memcmp(a->buf, b->buf, len);
// if (cmp) {
// return cmp;
//}
// return a->len < b->len ? -1: a->len != b->len;
//}

/**
 * Used as callback for `strbuf_expand()`, expects an array of
 * struct strbuf_expand_dict_entry as context, i.e. pairs of
 * placeholder and replacement string.  The array needs to be
 * terminated by an entry with placeholder set to NULL.
 */
// struct strbuf_expand_dict_entry {
// const char *placeholder;
// const char *value;
//};

/**
 * This function can be used to expand a format string containing
 * placeholders. To that end, it parses the string and calls the specified
 * function for every percent sign found.
 *
 * The callback function is given a pointer to the character after the `%`
 * and a pointer to the struct strbuf.  It is expected to add the expanded
 * version of the placeholder to the strbuf, e.g. to add a newline
 * character if the letter `n` appears after a `%`.  The function returns
 * the length of the placeholder recognized and `strbuf_expand()` skips
 * over it.
 *
 * The format `%%` is automatically expanded to a single `%` as a quoting
 * mechanism; callers do not need to handle the `%` placeholder themselves,
 * and the callback function will not be invoked for this placeholder.
 *
 * All other characters (non-percent and not skipped ones) are copied
 * verbatim to the strbuf.  If the callback returned zero, meaning that the
 * placeholder is unknown, then the percent sign is copied, too.
 *
 * In order to facilitate caching and to make it possible to give
 * parameters to the callback, `strbuf_expand()` passes a context pointer,
 * which can be used by the programmer of the callback as she sees fit.
 */
// typedef size_t (*expand_fn_t) (StringBuffer *sb, const char *placeholder,
// void *context);
// void strbuf_expand(StringBuffer *sb, const char *format, expand_fn_t fn, void
// *context);

// size_t expand_dict_cb(const char *placeholder, void *context) {
// strbuf_expand_dict_entry *e = context;
// size_t len;

// for (; e->placeholder && (len = std::strlen(e->placeholder)); e++) {
// if (!strncmp(placeholder, e->placeholder, len)) {
// if (e->value) {
// addstr(e->value);
//}
// return len;
//}
//}
// return 0;
//}

void strbuf_addIndentedText(StringBuffer* sb, const char* text, int indent,
                            int indent2);

/**
 * Read a given size of data from a FILE* pointer to the buffer.
 *
 * NOTE: The buffer is rewound if the read fails. If -1 is returned,
 * `errno` must be consulted, like you would do for `read(3)`.
 * `strbuf_read()`, `strbuf_read_file()` and `strbuf_getline_*()`
 * family of functions have the same behaviour as well.
 */
size_t strbuf_fread(StringBuffer* sb, ssize_t size, FILE* f);

/**
 * Read the contents of a given file descriptor. The third argument can be
 * used to give a hint about the file size, to avoid reallocs.  If read fails,
 * any partial read is undone.
 */
ssize_t strbuf_read(StringBuffer* sb, int fd, size_t hint);

/**
 * Read the contents of a given file descriptor partially by using only one
 * attempt of xread. The third argument can be used to give a hint about the
 * file size, to avoid reallocs. Returns the number of new bytes appended to
 * the sb.
 */
ssize_t strbuf_read_once(StringBuffer* sb, int fd, size_t hint);

/**
 * Read the contents of a file, specified by its path. The third argument
 * can be used to give a hint about the file size, to avoid reallocs.
 * Return the number of bytes read or a negative value if some error
 * occurred while opening or reading the file.
 */
ssize_t strbuf_read_file(StringBuffer* sb, const char* path, size_t hint);

/**
 * Read the target of a symbolic link, specified by its path.  The third
 * argument can be used to give a hint about the size, to avoid reallocs.
 */
int strbuf_readlink(StringBuffer* sb, const char* path, size_t hint);

/**
 * Write the whole content of the strbuf to the stream not stopping at
 * NUL bytes.
 */
ssize_t strbuf_write(StringBuffer* sb, FILE* f);

int strbuf_getcwd(StringBuffer* sb);
void strbuf_addstr_xml_quoted(StringBuffer* sb, const char* s);
void strbuf_addstr_urlencode(StringBuffer* sb, const char* s, int reserved);
void strbuf_humanise_bytes(StringBuffer* sb, off_t bytes);
char* xgetcwd(void);
void strbuf_addftime(StringBuffer* sb, const char* fmt, const struct tm* tm);
}
}

#endif
