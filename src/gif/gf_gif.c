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

enum gf_error_code gf_error_state = GF_ERROR_NONE;

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

    gif->canvas_width = read_16(fin);
    gif->canvas_height = read_16(fin);

    uint8_t packed_field = read_8(fin);
    

    fclose(fin);
    return gif;
}
