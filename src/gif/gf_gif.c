/*
 * This file is part of gifree.
 *
 * gifree is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gifree is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gifree.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "gf_gif.h"

#define CHECK_EOF if (feof(fin)) {                \
        free(gif);                                \
        gf_error_state = GF_ERROR_UNEXPECTED_EOF; \
        return NULL;                              \
    }

enum gf_error_code gf_error_state = GF_ERROR_NONE;

static uint8_t read_8(FILE *fin);
static uint16_t read_16(FILE *fin);
static void read_packed_field(struct gf_gif *dest, uint8_t src);
static void read_global_color_table(struct gf_gif *dest, FILE *fin);

struct gf_gif *gf_read_gif(const char *filename) {
    FILE *fin = fopen(filename, "rb");
    struct gf_gif *gif = (struct gf_gif*)malloc(sizeof(struct gf_gif));

    // First 3 bytes of header should read "GIF"
    int header_match = read_8(fin) == 0x47
        && read_8(fin) == 0x49
        && read_8(fin) == 0x46;
    if (!header_match) {
        free(gif);
        gf_error_state = GF_ERROR_BAD_HEADER;
        return NULL;
    }

    // Next three bytes are the gif version
    fread(&gif->version[0], 1, 3, fin);
    gif->version[3] = '\0';
    CHECK_EOF;

    gif->canvas_width = read_16(fin);
    gif->canvas_height = read_16(fin);
    CHECK_EOF;

    read_packed_field(gif, read_8(fin));
    CHECK_EOF;

    gif->background_color_index = read_8(fin);
    gif->pixel_aspect_ratio = read_8(fin);
    CHECK_EOF;

    if (gif->color_table_size != 0) {
        read_global_color_table(gif, fin);
    }
    CHECK_EOF;

    fclose(fin);
    return gif;
}

static void read_packed_field(struct gf_gif *dest, uint8_t src) {
    dest->color_table_size = 0x02 << (src & 0x07);
    src >>= 3;
    dest->sort_flag = src & 0x01;
    src >>= 1;
    dest->color_resolution = 0x02 << (src & 0x07);
    src >>= 3;
    if (!src) {
        dest->color_table_size = 0;
    }
}

static void read_global_color_table(struct gf_gif *dest, FILE *fin) {
    int i = 0;
    dest->color_table = (uint32_t*)malloc(
        sizeof(uint32_t) * dest->color_table_size
    );
    for (i = 0; i < dest->color_table_size; i++) {
        uint32_t *color = dest->color_table + i;
        *color = read_8(fin);
        *color <<= 8;
        *color |= read_8(fin);
        *color <<= 8;
        *color |= read_8(fin);
        *color <<= 8;
        *color |= 0x000000ff;
    }
}

static uint8_t read_8(FILE *fin) {
    uint8_t cin;
    fread(&cin, 1, 1, fin);
    return cin;
}

static uint16_t read_16(FILE *fin) {
    uint8_t lo = read_8(fin);
    uint8_t hi = read_8(fin);
    uint16_t retval = hi;
    retval <<= 8;
    retval |= lo;
    return retval;
}
