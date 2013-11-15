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

#include <gtk/gtk.h>
#include <stdio.h>

#include "gf_main_window.h"
#include "gf_gif.h"

void activate_callback(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    window = gf_main_window_new(app);
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    int i;
    struct gf_gif *gif = gf_gif_new_from_file(argv[1]);
    printf("Version: %s\n", gif->version);
    printf("Dimensions: (%d, %d)\n", gif->canvas_width, gif->canvas_height);
    printf("Colors: (%d, %d)\n", gif->color_resolution, gif->color_table_size);
    printf("Sort: %d\n", gif->sort_flag);
    printf("Color Table:\n");
    for (i = 0; i < gif->color_table_size; i++) {
        uint8_t *base = (uint8_t*)(gif->color_table + i);
        printf(
            "%3d| (%d, %d, %d, %d)\n",
            i,
            base[3],
            base[2],
            base[1],
            base[0]
        );
    }

    return 0;
    GtkApplication *app;
    int status;
    app = gtk_application_new(
        "org.bieber.gifree",
        G_APPLICATION_FLAGS_NONE
    );
    g_signal_connect(
        app,
        "activate",
        G_CALLBACK(activate_callback),
        NULL
    );
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return (status);
}
