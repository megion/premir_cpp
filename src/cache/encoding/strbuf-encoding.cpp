#include "cache/encoding/strbuf-encoding.h"

namespace cache {
	namespace encoding {

		int reencode(StringBuffer *buf, const char *from, const char *to) {
			if (same_encoding(from, to)) {
				return 0;
			}

			int inlen;
			char *out = reencode_string_len(buf->getBuf(), buf->getLen(), to, from, &inlen);
			if (!out) {
				return -1;
			}

			buf->attach(out, inlen, inlen);
			return 0;
		}

		/*
		 * Align the string given and store it into a strbuf as per the
		 * 'position' and 'width'. If the given string length is larger than
		 * 'width' than then the input string is not truncated and no
		 * alignment is done.
		 */
		void utf8Align(StringBuffer *buf, align_type position, unsigned int width, const char *s) {
			int slen = strlen(s);
			int display_len = utf8_strnwidth(s, slen, 0);
			int utf8_compensation = slen - display_len;

			if (display_len >= width) {
				strbuf_addstr(buf, s);
				return;
			}

			if (position == ALIGN_LEFT)
				strbuf_addf(buf, "%-*s", width + utf8_compensation, s);
			else if (position == ALIGN_MIDDLE) {
				int left = (width - display_len) / 2;
				strbuf_addf(buf, "%*s%-*s", left, "", width - left + utf8_compensation, s);
			} else if (position == ALIGN_RIGHT)
				strbuf_addf(buf, "%*s", width + utf8_compensation, s);
		}

		/*
		 * Wrap the text, if necessary. The variable indent is the indent for the
		 * first line, indent2 is the indent for all other lines.
		 * If indent is negative, assume that already -indent columns have been
		 * consumed (and no extra indent is necessary for the first line).
		 */
		void addWrappedText(StringBuffer *buf, const char *text, int indent1, int indent2, int width) {
			int indent, w, assume_utf8 = 1;
			const char *bol, *space, *start = text;
			size_t orig_len = buf->getLen();

			if (width <= 0) {
				buf->addIndentedText(text, indent1, indent2);
				return;
			}

retry:
			bol = text;
			w = indent = indent1;
			space = NULL;
			if (indent < 0) {
				w = -indent;
				space = text;
			}

			for (;;) {
				char c;
				size_t skip;

				while ((skip = display_mode_esc_sequence_len(text))) {
					text += skip;
				}

				c = *text;
				if (!c || isspace(c)) {
					if (w <= width || !space) {
						const char *start = bol;
						if (!c && text == start) {
							return;
						}
						if (space) {
							start = space;
						} else {
							strbuf_addchars(buf, ' ', indent);
						}
						strbuf_add(buf, start, text - start);
						if (!c) {
							return;
						}
						space = text;
						if (c == '\t') {
							w |= 0x07;
						} else if (c == '\n') {
							space++;
							if (*space == '\n') {
								strbuf_addch(buf, '\n');
								goto new_line;
							} else if (!isalnum(*space)) {
								goto new_line;
							} else {
								strbuf_addch(buf, ' ');
							}
						}
						w++;
						text++;
					}
					else {
new_line:
						strbuf_addch(buf, '\n');
						text = bol = space + isspace(*space);
						space = NULL;
						w = indent = indent2;
					}
					continue;
				}
				if (assume_utf8) {
					w += utf8_width(&text, NULL);
					if (!text) {
						assume_utf8 = 0;
						text = start;
						strbuf_setlen(buf, orig_len);
						goto retry;
					}
				} else {
					w++;
					text++;
				}
			}
		}

		void addWrappedBytes(StringBuffer *buf, const char *data, int len, int indent, int indent2, int width) {
			char *tmp = xstrndup(data, len);
			addWrappedText(buf, tmp, indent, indent2, width);
			free(tmp);
		}

		void utf8Replace(StringBuffer *buf, int pos, int width, const char *subst) {
			StringBuffer sb_dst;
			char *src = buf;
			char *end = src + len;
			int w = 0, subst_len = 0;

			if (subst) {
				subst_len = strlen(subst);
			}
			sb_dst.grow(len + subst_len);
			char *dst = sb_dst.buf;

			while (src < end) {
				char *old;
				size_t n;

				while ((n = encoding::display_mode_esc_sequence_len(src))) {
					std::memcpy(dst, src, n);
					src += n;
					dst += n;
				}

				if (src >= end)
					break;

				old = src;
				n = utf8_width((const char**)&src, NULL);
				if (!src) 	/* broken utf-8, do nothing */
					return;
				if (n && w >= pos && w < pos + width) {
					if (subst) {
						memcpy(dst, subst, subst_len);
						dst += subst_len;
						subst = NULL;
					}
					w += n;
					continue;
				}
				memcpy(dst, old, src - old);
				dst += src - old;
				w += n;
			}
			strbuf_setlen(&sb_dst, dst - sb_dst.buf);
			strbuf_swap(sb_src, &sb_dst);
			//strbuf_release(&sb_dst);
		}
	
	}
}


