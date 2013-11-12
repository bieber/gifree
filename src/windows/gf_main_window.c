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

#include "gf_main_window.h"

static gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
    guint width = gtk_widget_get_allocated_width(widget);
    guint height = gtk_widget_get_allocated_height(widget);
    cairo_arc(cr,
              width / 2.0,
              height / 2.0,
              MIN(width, height) / 2.0,
              0,
              2 * G_PI);

    cairo_set_source_rgba(cr, 1., .2, .2, .8);
    cairo_fill(cr);

    return FALSE;
}

GtkWidget *gf_main_window_new(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "gifree");

    GtkWidget *canvas = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), canvas);
    g_signal_connect(G_OBJECT(canvas),
                     "draw",
                     G_CALLBACK(draw_callback),
                     NULL);
return window;
}
