#include <stdint.h>

// Decode one UTF-8 codepoint into cp, return number of bytes to next one.
// On invalid UTF-8, return -1, and do not change cp.
// Invalid codepoints are not checked.
//
// This code is meant to be inlined, if cp is unused it can be optimized away.
static int
u8decode(const char *cs, uint32_t *cp)
{
	const uint8_t *s = (uint8_t *)cs;

	if (*s == 0)   { *cp = 0; return 0; }
	if (*s < 0x80) { *cp = *s; return 1; }
	if (*s < 0xc2) { return -1; }  //cont+overlong
	if (*s < 0xe0) { *cp = *s & 0x1f; goto u2; }
	if (*s < 0xf0) {
		if (*s == 0xe0 && (s[1] & 0xe0) == 0x80) return -1; //overlong
		if (*s == 0xed && (s[1] & 0xe0) == 0xa0) return -1; //surrogate
		*cp = *s & 0x0f; goto u3;
	}
	if (*s < 0xf5) {
		if (*s == 0xf0 && (s[1] & 0xf0) == 0x80) return -1; //overlong
		if (*s == 0xf4 && (s[1] > 0x8f)) return -1; //too high
		*cp = *s & 0x07; goto u4;
	}
	return -1;

u4:	if ((*++s & 0xc0) != 0x80) return -1;  *cp = (*cp << 6) | (*s & 0x3f);
u3:	if ((*++s & 0xc0) != 0x80) return -1;  *cp = (*cp << 6) | (*s & 0x3f);
u2:	if ((*++s & 0xc0) != 0x80) return -1;  *cp = (*cp << 6) | (*s & 0x3f);
	return s - (uint8_t *)cs + 1;
}
