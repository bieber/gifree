#ifndef GF_GIF_H
#define GF_GIF_H
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

#include <stdint.h>

enum gf_error_code {
    GF_ERROR_NONE,
    GF_ERROR_BAD_HEADER
};

struct gf_gif {
    char version[4];

    uint16_t canvas_width;
    uint16_t canvas_height;

    unsigned int global_color_table_size;
};

extern enum gf_error_code gf_error_state;

enum gf_error_code gif_error();
struct gf_gif *gf_read_gif(const char *filename);

#endif
