
#include "string.h"

#define NGX_INT32_LEN   (sizeof("-2147483648") - 1)
#define NGX_INT64_LEN   (sizeof("-9223372036854775808") - 1)

#if ((__GNU__ == 2) && (__GNUC_MINOR__ < 8))
#define NGX_MAX_UINT32_VALUE  (uint32_t) 0xffffffffLL
#else
#define NGX_MAX_UINT32_VALUE  (uint32_t) 0xffffffff
#endif

#define NGX_MAX_INT32_VALUE   (uint32_t) 0x7fffffff

#define ngx_cpymem(dst, src, n)   (((uint8_t *) memcpy(dst, src, n)) + (n))

static uint8_t *
sprintf_num(uint8_t *buf, uint8_t *last, uint64_t ui64, uint8_t zero,
    unsigned int hexadecimal, unsigned int width)
{
    uint8_t         *p, temp[NGX_INT64_LEN + 1];
    /*
    * we need temp[NGX_INT64_LEN] only,
    * but icc issues the warning
    */
    size_t          len;
    uint32_t        ui32;
    static uint8_t   hex[] = "0123456789abcdef";
    static uint8_t   HEX[] = "0123456789ABCDEF";

    p = temp + NGX_INT64_LEN;

    if (hexadecimal == 0) {

        if (ui64 <= (uint64_t)NGX_MAX_UINT32_VALUE) {

            /*
            * To divide 64-bit numbers and to find remainders
            * on the x86 platform gcc and icc call the libc functions
            * [u]divdi3() and [u]moddi3(), they call another function
            * in its turn.  On FreeBSD it is the qdivrem() function,
            * its source code is about 170 lines of the code.
            * The glibc counterpart is about 150 lines of the code.
            *
            * For 32-bit numbers and some divisors gcc and icc use
            * a inlined multiplication and shifts.  For example,
            * unsigned "i32 / 10" is compiled to
            *
            *     (i32 * 0xCCCCCCCD) >> 35
            */

            ui32 = (uint32_t)ui64;

            do {
                *--p = (uint8_t)(ui32 % 10 + '0');
            } while (ui32 /= 10);

        }
        else {
            do {
                *--p = (uint8_t)(ui64 % 10 + '0');
            } while (ui64 /= 10);
        }

    }
    else if (hexadecimal == 1) {

        do {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = hex[(uint32_t)(ui64 & 0xf)];

        } while (ui64 >>= 4);

    }
    else { /* hexadecimal == 2 */

        do {

            /* the "(uint32_t)" cast disables the BCC's warning */
            *--p = HEX[(uint32_t)(ui64 & 0xf)];

        } while (ui64 >>= 4);
    }

    /* zero or space padding */

    len = (temp + NGX_INT64_LEN) - p;

    while (len++ < width && buf < last) {
        *buf++ = zero;
    }

    /* number safe copy */

    len = (temp + NGX_INT64_LEN) - p;

    if (buf + len > last) {
        len = last - buf;
    }

    return ngx_cpymem(buf, p, len);
}

int
jstr_vslprintf(uint8_t *buf, uint8_t *last, size_t maxn, jstr_vslp_cb cb, void *ctx, const char *fmt, va_list args)
{
    uint8_t                *p, zero, *bufbak;
    int                    d;
    double                 f;
    size_t                 len, slen;
    int64_t                i64;
    uint64_t               ui64, frac;
    //ngx_msec_t             ms;
    unsigned             width, sign, hex, max_width, frac_width, scale, n;
    //ngx_str_t             *v;
    //ngx_variable_value_t  *vv;

    bufbak = buf;
    while (*fmt && buf < last) {

        /*
        * "buf < last" means that we could copy at least one character:
        * the plain character, "%%", "%c", and minus without the checking
        */

        if (*fmt == '%') {

            i64 = 0;
            ui64 = 0;

            zero = (uint8_t)((*++fmt == '0') ? '0' : ' ');
            width = 0;
            sign = 1;
            hex = 0;
            max_width = 0;
            frac_width = 0;
            slen = (size_t)-1;

            while (*fmt >= '0' && *fmt <= '9') {
                width = width * 10 + *fmt++ - '0';
            }

            for (;;) {
                switch (*fmt) {

                case 'u':
                    sign = 0;
                    fmt++;
                    continue;

                case 'm':
                    max_width = 1;
                    fmt++;
                    continue;

                case 'X':
                    hex = 2;
                    sign = 0;
                    fmt++;
                    continue;

                case 'x':
                    hex = 1;
                    sign = 0;
                    fmt++;
                    continue;

                case '.':
                    fmt++;

                    while (*fmt >= '0' && *fmt <= '9') {
                        frac_width = frac_width * 10 + *fmt++ - '0';
                    }

                    break;

                case '*':
                    slen = va_arg(args, size_t);
                    fmt++;
                    continue;

                default:
                    break;
                }

                break;
            }


            switch (*fmt) {

            //case 'V':
            //    v = va_arg(args, ngx_str_t *);

            //    len = ngx_min(((size_t)(last - buf)), v->len);
            //    buf = ngx_cpymem(buf, v->data, len);
            //    fmt++;

            //    continue;

            //case 'v':
            //    vv = va_arg(args, ngx_variable_value_t *);

            //    len = ngx_min(((size_t)(last - buf)), vv->len);
            //    buf = ngx_cpymem(buf, vv->data, len);
            //    fmt++;

            //    continue;

            case 's':
                p = va_arg(args, uint8_t *);

                if (slen == (size_t)-1) {
                    while (*p && buf < last) {
                        *buf++ = *p++;
                    }

                }
                else {
                    len = min(((size_t)(last - buf)), slen);
                    buf = memcpy(buf, p, len);
                    buf += len;
                }

                fmt++;

                continue;

            //case 'O':
            //    i64 = (int64_t)va_arg(args, off_t);
            //    sign = 1;
            //    break;

            //case 'P':
            //    i64 = (int64_t)va_arg(args, ngx_pid_t);
            //    sign = 1;
            //    break;

            case 'T':
                i64 = (int64_t)va_arg(args, time_t);
                sign = 1;
                break;

            //case 'M':
            //    ms = (ngx_msec_t)va_arg(args, ngx_msec_t);
            //    if ((ngx_msec_int_t)ms == -1) {
            //        sign = 1;
            //        i64 = -1;
            //    }
            //    else {
            //        sign = 0;
            //        ui64 = (uint64_t)ms;
            //    }
            //    break;

            case 'z':
                if (sign) {
                    i64 = (int64_t)va_arg(args, ssize_t);
                }
                else {
                    ui64 = (uint64_t)va_arg(args, size_t);
                }
                break;

            //case 'i':
            //    if (sign) {
            //        i64 = (int64_t)va_arg(args, int);
            //    }
            //    else {
            //        ui64 = (uint64_t)va_arg(args, unsigned int);
            //    }

            //    if (max_width) {
            //        width = NGX_INT_T_LEN;
            //    }

            //    break;

            case 'd':
                if (sign) {
                    i64 = (int64_t)va_arg(args, int);
                }
                else {
                    ui64 = (uint64_t)va_arg(args, unsigned int);
                }
                break;

            case 'l':
                if (sign) {
                    i64 = (int64_t)va_arg(args, long);
                }
                else {
                    ui64 = (uint64_t)va_arg(args, unsigned long);
                }
                break;

            case 'D':
                if (sign) {
                    i64 = (int64_t)va_arg(args, int32_t);
                }
                else {
                    ui64 = (uint64_t)va_arg(args, uint32_t);
                }
                break;

            case 'L':
                if (sign) {
                    i64 = va_arg(args, int64_t);
                }
                else {
                    ui64 = va_arg(args, uint64_t);
                }
                break;

            //case 'A':
            //    if (sign) {
            //        i64 = (int64_t)va_arg(args, ngx_atomic_int_t);
            //    }
            //    else {
            //        ui64 = (uint64_t)va_arg(args, ngx_atomic_uint_t);
            //    }

            //    if (max_width) {
            //        width = NGX_ATOMIC_T_LEN;
            //    }

            //    break;

            case 'f':
                f = va_arg(args, double);

                if (f < 0) {
                    *buf++ = '-';
                    f = -f;
                }

                ui64 = (int64_t)f;
                frac = 0;

                if (frac_width) {

                    scale = 1;
                    for (n = frac_width; n; n--) {
                        scale *= 10;
                    }

                    frac = (uint64_t)((f - (double)ui64) * scale + 0.5);

                    if (frac == scale) {
                        ui64++;
                        frac = 0;
                    }
                }

                buf = sprintf_num(buf, last, ui64, zero, 0, width);

                if (frac_width) {
                    if (buf < last) {
                        *buf++ = '.';
                    }

                    buf = sprintf_num(buf, last, frac, '0', 0, frac_width);
                }

                fmt++;

                continue;

//#if !(NGX_WIN32)
//            case 'r':
//                i64 = (int64_t)va_arg(args, rlim_t);
//                sign = 1;
//                break;
//#endif

            //case 'p':
            //    ui64 = (uintptr_t)va_arg(args, void *);
            //    hex = 2;
            //    sign = 0;
            //    zero = '0';
            //    width = NGX_PTR_SIZE * 2;
            //    break;

            case 'c':
                d = va_arg(args, int);
                *buf++ = (uint8_t)(d & 0xff);
                fmt++;

                continue;

            case 'Z':
                *buf++ = '\0';
                fmt++;

                continue;

//            case 'N':
//#if (NGX_WIN32)
//                *buf++ = CR;
//#endif
//                *buf++ = LF;
//                fmt++;
//
//                continue;

            case '%':
                *buf++ = '%';
                fmt++;

                continue;

            default:
                *buf++ = *fmt++;

                continue;
            }

            if (sign) {
                if (i64 < 0) {
                    *buf++ = '-';
                    ui64 = (uint64_t)-i64;

                }
                else {
                    ui64 = (uint64_t)i64;
                }
            }

            buf = sprintf_num(buf, last, ui64, zero, hex, width);

            fmt++;

        }
        else {
            *buf++ = *fmt++;
        }
    }

    return buf - bufbak;
}


