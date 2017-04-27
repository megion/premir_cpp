#include "cache/str/StringBuffer.h"

namespace cache {
namespace str {

void StringBuffer::grow(size_t extra)
{
    bool new_buf = (alloc == 0);

    size_t newLen = len + extra + 1;
    if (newLen > alloc) {
        size_t newAlloc = (alloc + 16) * 3 / 2;
        if (newAlloc < newLen) {
            alloc = newLen;
        }
        else {
            alloc = newAlloc;
        }
        reallocBuf();
    }
    if (new_buf) {
        buf[0] = '\0';
    }
}

char* StringBuffer::detach(size_t* sz)
{
    char* res;
    grow(0);
    res = buf;
    if (sz) {
        *sz = len;
    }
    alloc = 0;
    len = 0;
    buf = nullptr;
    return res;
}

void StringBuffer::attach(char* _buf, size_t _len, size_t _alloc)
{
    release();
    buf = _buf;
    len = _len;
    alloc = _alloc;
    grow(0);
    buf[len] = '\0';
}

void StringBuffer::splice(size_t pos, size_t removeLen, const void* data,
                          size_t dataLen)
{
    if (pos > len) {
        LOG(ERR, "`pos' is too far after the end of the buffer");
        return;
    }
    if (pos + removeLen > len) {
        LOG(ERR, "`pos + removeLen' is too far after the end of the buffer");
        return;
    }

    if (dataLen >= removeLen) {
        grow(dataLen - removeLen);
    }
    std::memmove(buf + pos + dataLen, buf + pos + removeLen,
                 len - pos - removeLen);
    std::memcpy(buf + pos, data, dataLen);
    setLen(len + dataLen - removeLen);
}

void StringBuffer::addCommentedLines(const char* bufLines, size_t size)
{
    static char prefix1[3];
    static char prefix2[2];

    if (prefix1[0] != COMMENT_LINE_CHAR) {
        xsnprintf(prefix1, sizeof(prefix1), "%c ", COMMENT_LINE_CHAR);
        xsnprintf(prefix2, sizeof(prefix2), "%c", COMMENT_LINE_CHAR);
    }
    add_lines(prefix1, prefix2, bufLines, size);
}

void StringBuffer::commentedAddf(const char* fmt, ...)
{
    va_list params;
    StringBuffer tempBuf;
    int incomplete_line = len && buf[len - 1] != '\n';

    va_start(params, fmt);
    tempBuf.vaddf(fmt, params);
    va_end(params);

    addCommentedLines(tempBuf.buf, tempBuf.len);
    if (incomplete_line) {
        buf[--len] = '\0';
    }
}

void StringBuffer::vaddf(const char* fmt, va_list ap)
{
    va_list cp;

    if (!avail()) {
        grow(64);
    }
    va_copy(cp, ap);
    int addLen = vsnprintf(buf + len, alloc - len, fmt, cp);
    va_end(cp);
    if (addLen < 0) {
        LOG(ERR, "BUG: your vsnprintf is broken (returned %d)", addLen);
        return;
    }
    if (addLen > avail()) {
        grow(addLen);
        addLen = vsnprintf(buf + len, alloc - len, fmt, ap);
        if (addLen > avail()) {
            LOG(ERR, "BUG: your vsnprintf is broken (insatiable)");
        }
    }
    setLen(len + addLen);
}

/*
 * Define HAVE_GETDELIM if your system has the getdelim() function.
 */
#ifdef HAVE_GETDELIM
int StringBuffer::strbuf_getwholeline(FILE* fp, int term)
{
    ssize_t r;

    if (feof(fp)) {
        return EOF;
    }

    reset();

    /* Translate slopbuf to NULL, as we cannot call realloc on it */
    if (!alloc) {
        buf = nullptr;
    }
    r = getdelim(&buf, &alloc, term, fp);

    if (r > 0) {
        len = r;
        return 0;
    }
    assert(r == -1);

    /*
     * Normally we would have called xrealloc, which will try to free
     * memory and recover. But we have no way to tell getdelim() to do so.
     * Worse, we cannot try to recover ENOMEM ourselves, because we have
     * no idea how many bytes were read by getdelim.
     *
     * Dying here is reasonable. It mirrors what xrealloc would do on
     * catastrophic memory failure. We skip the opportunity to free pack
     * memory and retry, but that's unlikely to help for a malloc small
     * enough to hold a single line of input, anyway.
     */
    if (errno == ENOMEM) {
        LOG(ERR, "Out of memory, getdelim failed");
        return -1;
    }

    /*
     * Restore strbuf invariants; if getdelim left us with a NULL pointer,
     * we can just re-init, but otherwise we should make sure that our
     * length is empty, and that the result is NUL-terminated.
     */
    if (!buf) {
        init(0);
    }
    else {
        reset();
    }
    return EOF;
}
#else
int StringBuffer::strbuf_getwholeline(FILE* fp, int term)
{
    int ch;

    if (feof(fp)) {
        return EOF;
    }

    reset();
    flockfile(fp);
    while ((ch = getc_unlocked(fp)) != EOF) {
        if (!avail()) {
            grow(1);
        }
        buf[len++] = ch;
        if (ch == term) {
            break;
        }
    }
    funlockfile(fp);
    if (ch == EOF && len == 0) {
        return EOF;
    }

    buf[len] = '\0';
    return 0;
}
#endif
}
}
