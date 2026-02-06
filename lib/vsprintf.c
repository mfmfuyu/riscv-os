#include "lib.h"
#include "stdio.h"

int vsprintf(char *buf, const char *fmt, va_list args)
{
	int pos = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case '\0':
                    buf[pos++] = '%';
                    goto end;
                case '%':
                    buf[pos++] = '%';
                    break;
                case 's': {
                    const char *s = va_arg(args, const char *);
                    while (*s) {
                        buf[pos++] = *s;
                        s++;
                    }
                    break;
                }
                case 'd': {
                    int value = va_arg(args, int);
                    unsigned magnitude = value;
                    if (value < 0) {
                        buf[pos++] = '-';
                        magnitude = -magnitude;
                    }

                    unsigned divisor = 1;
                    while (magnitude / divisor > 9)
                        divisor *= 10;

                    while (divisor > 0) {
                        buf[pos++] = '0' + magnitude / divisor;
                        magnitude %= divisor;
                        divisor /= 10;
                    }

                    break;
                }
                case 'x': {
                    unsigned value = va_arg(args, unsigned);
                    for (int i = 7; i >= 0; i--) {
                        unsigned nibble = (value >> (i * 4)) & 0xf;
                        buf[pos++] = "0123456789abcdef"[nibble];
                    }
                    break;
                }
            }
        } else {
            buf[pos++] = *fmt;
        }

        fmt++;
    }

end:
    buf[pos] = '\0';
    return pos;
}
