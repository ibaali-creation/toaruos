#include <string.h>
#include <stdio.h>

#include "text.h"

static unsigned short * textmemptr = (unsigned short *)0xB8000;
static void placech(unsigned char c, int x, int y, int attr) {
	unsigned short *where;
	unsigned att = attr << 8;
	where = textmemptr + (y * 80 + x);
	*where = c | att;
}


int txt_debug = 0;
int x = 0;
int y = 0;
int attr = 0x07;
int scroll_disabled = 0;

static void scroll_everything(void) {
	if (scroll_disabled) {
		y = 0;
		return;
	}
	memmove(textmemptr, textmemptr + 80, 80 * 24 * 2);
	for (int x = 0; x < 80; ++x) {
		placech(' ', x, 24, attr);
	}
}

void print_(char * str) {
	while (*str) {
		if (*str == '\n') {
			for (; x < 80; ++x) {
				placech(' ', x, y, attr);
			}
			x = 0;
			y += 1;
			if (y == 25) {
				y = 24;
				scroll_everything();
			}
		} else {
			placech(*str, x, y, attr);
			x++;
			if (x == 80) {
				x = 0;
				y += 1;
				if (y == 25) {
					y = 24;
					scroll_everything();
				}
			}
		}
		str++;
	}
}

void move_cursor(int _x, int _y) {
	x = _x;
	y = _y;
}

void move_cursor_rel(int _x, int _y) {
	x += _x; if (x < 0) x = 0;
	y += _y; if (y < 0) y = 0;
}

void set_attr(int _attr) {
	attr = _attr;
}

void print_banner(char * str) {
	if (!str) {
		for (int i = 0; i < 80; ++i) {
			placech(' ', i, y, attr);
		}
		y++;
		return;
	}
	int len = 0;
	char *c = str;
	while (*c) {
		len++;
		c++;
	}
	int off = (80 - len) / 2;

	for (int i = 0; i < 80; ++i) {
		placech(' ', i, y, attr);
	}
	for (int i = 0; i < len; ++i) {
		placech(str[i], i + off, y, attr);
	}

	y++;
}

void print_hex_(unsigned int value) {
	char out[9] = {0};
	for (int i = 7; i > -1; i--) {
		out[i] = "0123456789abcdef"[(value >> (4 * (7 - i))) & 0xF];
	}
	print_(out);
}

void clear_() {
	x = 0;
	y = 0;
	for (int y = 0; y < 25; ++y) {
		for (int x = 0; x < 80; ++x) {
			placech(' ', x, y, attr);
		}
	}
}

