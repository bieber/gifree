#ifndef GF_GIF_FILE_CONSTANTS_H
#define GF_GIF_FILE_CONSTANTS_H
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

#define GF_GIF_HEADER "GIF"

enum gf_gif_block_type {
    GF_GIF_EXTENSION_BLOCK = 0x21,
    GF_GIF_IMAGE_BLOCK = 0x2c,
    GF_GIF_TRAILER = 0x3b
};

enum gf_gif_extension_type {
  GF_GIF_EXTENSION_GRAPHICS_CONTROL = 0xf9
};

enum gf_gif_disposal_method {
    GF_GIF_DISPOSAL_METHOD_IN_PLACE = 0x01,
    GF_GIF_DISPOSAL_METHOD_CLEAR_CANVAS = 0x02,
    GF_GIF_DISPOSAL_METHOD_PREVIOUS_FRAME = 0x03
};

#endif
