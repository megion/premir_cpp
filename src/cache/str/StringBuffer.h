#ifndef SRC_CACHE_STR_STRING_BUFFER_H
#define SRC_CACHE_STR_STRING_BUFFER_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <limits.h>
#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include <exception>
#include <stdexcept>
#include <iostream>
#include <fcntl.h>
#include "utils/console_colors.h"
#include "cache/encoding/utf8.h"
#include "cache/wrapper.h"
#include "cache/compat-utils.h"

#define STRBUF_MAXLINK (2 * PATH_MAX)

namespace cache {
namespace str {

class StringBuffer {

  public:
    StringBuffer() : alloc(0), len(0), buf(nullptr) {}

    StringBuffer(size_t hint) : alloc(0), len(0), buf(nullptr) {
        if (hint) {
            grow(hint);
        }
    }

    ~StringBuffer() { release(); }

    // extern char strbuf_slopbuf[];
    //#define STRBUF_INIT  { 0, 0, strbuf_slopbuf }
    void init(size_t hint) {
        alloc = len = 0;
        buf = nullptr;
        if (hint) {
            grow(hint);
        }
    }

    /**
     * Set the length of the buffer to a given value. This function does *not*
     * allocate new memory, so you should not perform a `strbuf_setlen()` to a
     * length that is larger than `len + strbuf_avail()`. `strbuf_setlen()` is
     * just meant as a 'please fix invariants from this strbuf I just messed
     * with'.
     */
    void setLen(size_t newLen) {
        if (newLen > (alloc ? alloc - 1 : 0)) {
            LOG(ERR, "BUG: setLen() beyond buffer");
        }
        len = newLen;
        buf[len] = '\0';
    }

    /**
     * Empty the buffer by setting the size of it to zero.
     * strbuf_reset(sb)  strbuf_setlen(sb, 0)
     */
    void reset() { setLen(0); }

    void release() {
        if (buf) {
            std::free(buf);
            alloc = 0;
            len = 0;
            buf = nullptr;
        }
    }

    /**
     * Determine the amount of allocated but unused memory.
     */
    size_t avail() const { return alloc ? alloc - len - 1 : 0; }

    /**
     * Add a single character to the buffer.
     */
    void addch(int c) {
        if (!avail()) {
            grow(1);
        }
        buf[len++] = c;
        buf[len] = '\0';
    }

    void grow(size_t extra);

    /**
     * Detach the string from the strbuf and returns it; you now own the
     * storage the string occupies and it is your responsibility from then on
     * to release it with `free(3)` when you are done with it.
     */
    char* detach(size_t* sz);

    /**
     * Attach a string to a buffer. You should specify the string to attach,
     * the current length of the string and the amount of allocated memory.
     * The amount must be larger than the string length, because the string you
     * pass is supposed to be a NUL-terminated string.  This string _must_ be
     * malloc()ed, and after attaching, the pointer cannot be relied upon
     * anymore, and neither be free()d directly.
     */
    void attach(char* _buf, size_t _len, size_t _alloc);

    void trim() {
        rtrim();
        ltrim();
    }

    void rtrim() {
        while (len > 0 && std::isspace(buf[len - 1])) {
            len--;
        }
        buf[len] = '\0';
    }

    void ltrim() {
        char* b = buf;
        while (len > 0 && std::isspace(*b)) {
            b++;
            len--;
        }
        std::memmove(buf, b, len);
        buf[len] = '\0';
    }

    void tolower() {
        char* p = buf, *end = buf + len;
        for (; p < end; p++) {
            *p = std::tolower(*p);
        }
    }

    /*
     * cut 'removeLen' elements start with 'pos' and insert data
     */
    void splice(size_t pos, size_t removeLen, const void* data, size_t dataLen);

    void insert(size_t pos, const void* data, size_t dataLen) {
        splice(pos, 0, data, dataLen);
    }

    void remove(size_t pos, size_t removeLen) { splice(pos, removeLen, "", 0); }

    /**
     * add data of given length to the buffer.
     */
    void add(const void* data, size_t dataLen) {
        grow(dataLen);
        memcpy(buf + len, data, dataLen);
        setLen(len + dataLen);
    }

    /**
     * Add a NUL-terminated string to the buffer.
     *
     * NOTE: This function will *always* be implemented as an inline or a macro
     * using strlen, meaning that this is efficient to write things like:
     *
     *     strbuf_addstr(sb, "immediate string");
     *
     */
    void addstr(const char* s) { add(s, std::strlen(s)); }

    /**
     * Copy the contents of another buffer at the end of the current one.
     */
    void addbuf(const StringBuffer* sb2) {
        grow(sb2->len);
        memcpy(buf + len, sb2->buf, sb2->len);
        setLen(len + sb2->len);
    }

    /**
     * Copy part of the buffer from a given position till a given length to the
     * end of the buffer.
     */
    void adddup(size_t pos, size_t dupLen) {
        grow(dupLen);
        memcpy(buf + len, buf + pos, dupLen);
        setLen(len + dupLen);
    }

    void addchars(int c, size_t num) {
        grow(num);
        std::memset(buf + len, c, num);
        setLen(len + num);
    }

    /**
     * Add a formatted string to the buffer.
     */
    void addf(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        vaddf(fmt, ap);
        va_end(ap);
    }

    /**
     * Add a NUL-terminated string to the buffer. Each line will be prepended
     * by a comment character and a blank.
     */
    void addCommentedLines(const char* bufLines, size_t size);

    void commentedAddf(const char* fmt, ...);

    void vaddf(const char* fmt, va_list ap);

    void addLines(const char* prefix, const char* bufLines, size_t size) {
        add_lines(prefix, NULL, bufLines, size);
    }

    int strbuf_getwholeline(FILE* fp, int term);

    int getDelim(FILE* fp, int term) {
        if (strbuf_getwholeline(fp, term)) {
            return EOF;
        }
        if (buf[len - 1] == term) {
            setLen(len - 1);
        }
        return 0;
    }

    int getLine(FILE* fp) {
        if (strbuf_getwholeline(fp, '\n')) {
            return EOF;
        }
        if (buf[len - 1] == '\n') {
            setLen(len - 1);
            if (len && buf[len - 1] == '\r') {
                setLen(len - 1);
            }
        }
        return 0;
    }

    int getLineLf(FILE* fp) { return getDelim(fp, '\n'); }

    int getLineNul(FILE* fp) { return getDelim(fp, '\0'); }

    int strbuf_getwholeline_fd(int fd, int term) {
        reset();

        while (1) {
            char ch;
            ssize_t len = xread(fd, &ch, 1);
            if (len <= 0) {
                return EOF;
            }
            addch(ch);
            if (ch == term) {
                break;
            }
        }
        return 0;
    }

  private:
    size_t alloc;

    /*
     * The character that begins a commented line in user-editable file
     * that is subject to stripspace.
     */
    const static char COMMENT_LINE_CHAR = '#';

    void reallocBuf() {
        size_t amount = sizeof(char) * alloc;
        char* newBuf = (char*)std::realloc(buf, amount);

        if (newBuf == nullptr) {
            throw std::runtime_error(std::strerror(errno));
        }
        buf = newBuf;
    }

    /**
     * strbuf_complete
     * "Complete" the contents of `sb` by ensuring that either it ends with the
     * character `term`, or it is empty.  This can be used, for example,
     * to ensure that text ends with a newline, but without creating an empty
     * blank line if there is no content in the first place.
     */
    void complete(char term) {
        if (len && buf[len - 1] != term) {
            addch(term);
        }
    }

    void completeLine() { complete('\n'); }

    /*
     * add_lines
     */
    void add_lines(const char* prefix1, const char* prefix2,
                   const char* bufLines, size_t size) {
        while (size) {
            const char* prefix;
            const char* next = (const char*)memchr(bufLines, '\n', size);
            next = next ? (next + 1) : (bufLines + size);

            prefix = ((prefix2 && (bufLines[0] == '\n' || bufLines[0] == '\t'))
                          ? prefix2
                          : prefix1);
            addstr(prefix);
            add(bufLines, next - bufLines);
            size -= next - bufLines;
            bufLines = next;
        }
        completeLine();
    }

  public:
    size_t len;
    char* buf;

    size_t getAlloc() const { return alloc; }
};
}
}

#endif
