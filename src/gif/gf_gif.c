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

#include "gf_gif.h"

enum gf_error_code gf_error_state = GF_ERROR_NONE;

void gf_gif_free(struct gf_gif *gif) {
    if (gif->color_table) {
        free(gif->color_table);
    }
    free(gif);
}
