#include "common.h"
#include "sbi.h"

int vsprintf(char *str, const char *fmt, va_list vargs) {
    int pos = 0;

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case '\0':
                    str[pos++] = '%';
                    goto end;
                case '%':
                    str[pos++] = '%';
                    break;
                case 's': {
                    const char *s = va_arg(vargs, const char *);
                    while (*s) {
                        str[pos++] = *s;
                        s++;
                    }
                    break;
                }
                case 'd': {
                    int value = va_arg(vargs, int);
                    unsigned magnitude = value;
                    if (value < 0) {
                        str[pos++] = '-';
                        magnitude = -magnitude;
                    }

                    unsigned divisor = 1;
                    while (magnitude / divisor > 9)
                        divisor *= 10;

                    while (divisor > 0) {
                        str[pos++] = '0' + magnitude / divisor;
                        magnitude %= divisor;
                        divisor /= 10;
                    }

                    break;
                }
                case 'x': {
                    unsigned value = va_arg(vargs, unsigned);
                    for (int i = 7; i >= 0; i--) {
                        unsigned nibble = (value >> (i * 4)) & 0xf;
                        str[pos++] = "0123456789abcdef"[nibble];
                    }
                    break;
                }
            }
        } else {
            str[pos++] = *fmt;
        }

        fmt++;
    }

end:
    str[pos] = '\0';
    return pos;
}

int sprintf(char *str, const char *fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);

    int n = vsprintf(str, fmt, vargs);

    va_end(vargs);
    return n;
}

void printf(const char *fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case '\0':
                    sbi_putchar('%');
                    goto end;
                case '%':
                    sbi_putchar('%');
                    break;
                case 's': {
                    const char *s = va_arg(vargs, const char *);
                    while (*s) {
                        sbi_putchar(*s);
                        s++;
                    }
                    break;
                }
                case 'd': {
                    int value = va_arg(vargs, int);
                    unsigned magnitude = value;
                    if (value < 0) {
                        sbi_putchar('-');
                        magnitude = -magnitude;
                    }

                    unsigned divisor = 1;
                    while (magnitude / divisor > 9)
                        divisor *= 10;

                    while (divisor > 0) {
                        sbi_putchar('0' + magnitude / divisor);
                        magnitude %= divisor;
                        divisor /= 10;
                    }

                    break;
                }
                case 'x': {
                    unsigned value = va_arg(vargs, unsigned);
                    for (int i = 7; i >= 0; i--) {
                        unsigned nibble = (value >> (i * 4)) & 0xf;
                        sbi_putchar("0123456789abcdef"[nibble]);
                    }
                    break;
                }
            }
        } else {
            sbi_putchar(*fmt);
        }

        fmt++;
    }

end:
    va_end(vargs);
}
