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
    struct gf_gif *gif = gf_read_gif(argv[1]);
    printf("%s\n", gif->version);
    printf("(%d, %d)\n", gif->canvas_width, gif->canvas_height);

    return 0;
    GtkApplication *app;
    int status;
    app = gtk_application_new("org.bieber.gifree",
                              G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app,
                     "activate",
                     G_CALLBACK(activate_callback),
                     NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return (status);
}
