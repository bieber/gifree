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
#include <string.h>

#include "gf_gif.h"
#include "gf_gif_file_constants.h"

struct graphics_control {
    uint8_t transparent_flag;
    uint8_t transparent_color_index;

    uint8_t disposal_method;

    uint32_t delay_ms;
};

static void read_packed_field(struct gf_gif *dest, uint8_t src);
static void read_global_color_table(struct gf_gif *dest, FILE *fin);

static struct gf_gif_frame *read_frame(
    struct gf_gif *gif,
    FILE *fin,
    struct graphics_control *control,
    struct gf_gif_frame *last_frame,
    struct gf_gif_frame *last_last_frame
);

static void skip_extension(FILE *fin);

static uint8_t read_8(FILE *fin);
static uint16_t read_16(FILE *fin);

struct gf_gif *gf_gif_new_from_file(const char *filename) {
#define CHECK_EOF if (feof(fin)) {                \
        gf_gif_free(gif);                         \
        gf_error_state = GF_ERROR_UNEXPECTED_EOF; \
        return NULL;                              \
    }

    FILE *fin = fopen(filename, "rb");
    struct gf_gif *gif = (struct gf_gif*)malloc(sizeof(struct gf_gif));
    gf_error_state = GF_ERROR_NONE;

    char header[4];
    fread(header, 1, 3, fin);
    header[3] = '\0';
    if (strcmp(header, GF_GIF_HEADER) != 0) {
        gf_gif_free(gif);
        gf_error_state = GF_ERROR_BAD_HEADER;
        return NULL;
    }

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

    struct graphics_control control;
    control.transparent_flag = 0;
    control.transparent_color_index = 0;
    control.disposal_method = GF_GIF_DISPOSAL_METHOD_CLEAR_CANVAS;
    control.delay_ms = 0;
    enum gf_gif_block_type block_type;
    for (block_type = read_8(fin);
         block_type != GF_GIF_TRAILER;
         block_type = read_8(fin)) {

        enum gf_gif_extension_type ext_type;
        switch (block_type) {

        case GF_GIF_EXTENSION_BLOCK:
            ext_type = read_8(fin);
            switch (ext_type) {
            case GF_GIF_EXTENSION_GRAPHICS_CONTROL:
            default:
                printf("Skipping extension of type 0x%02x\n", ext_type);
                skip_extension(fin);
            }
            break;

        case GF_GIF_IMAGE_BLOCK:
            read_frame(gif, fin, &control, NULL, NULL);
            break;

        case GF_GIF_TRAILER:
            break;

        default:
            gf_error_state = GF_ERROR_UNSUPPORTED_BLOCK_TYPE;
            gf_gif_free(gif);
            return NULL;
        }
    }

    fclose(fin);
    return gif;
#undef CHECK_EOF
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

static struct gf_gif_frame *read_frame(
    struct gf_gif *gif,
    FILE *fin,
    struct graphics_control *control,
    struct gf_gif_frame *last_frame,
    struct gf_gif_frame *last_last_frame
) {
    struct gf_gif_frame *frame = (struct gf_gif_frame*)malloc(
        sizeof(struct gf_gif_frame)
    );
    printf("Skipping a frame\n");
    fseek(fin, 8, SEEK_CUR);
    uint8_t packed = read_8(fin);
    if (packed & 0x80) {
        printf("Frame has local color table\n");
        uint8_t table_size = 0x02 << (packed & 0x07);
        fseek(fin, table_size * 3, SEEK_CUR);
    }
    read_8(fin);
    uint8_t block_size;
    for (block_size = read_8(fin); block_size != 0; block_size = read_8(fin)) {
        fseek(fin, block_size, SEEK_CUR);
    }

    return frame;
}

static void skip_extension(FILE *fin) {
    uint8_t block_size;
    for (block_size = read_8(fin); block_size != 0; block_size = read_8(fin)) {
        fseek(fin, block_size, SEEK_CUR);
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
