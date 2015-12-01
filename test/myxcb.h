#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <xcb/xcb.h>

#include <X11/Xlib.h>

#include "graphics/Chart.h"

int xcbtest(void) {
	xcb_connection_t *c;
	xcb_screen_t *s;
	xcb_window_t w;
	xcb_gcontext_t g;
	xcb_generic_event_t *e;
	uint32_t mask;
	uint32_t values[2];
	int done = 0;
	xcb_rectangle_t r = { 20, 20, 160, 160 };

	/* открыть соединение с сервером */
	c = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(c)) {
		printf("Cannot open display\n");
		exit(1);
	}
	/* get the first screen */
	s = xcb_setup_roots_iterator(xcb_get_setup(c)).data;

	/* create black graphics context */
	g = xcb_generate_id(c);
	w = s->root;
	mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
	values[0] = s->black_pixel;
	values[1] = 0;
	xcb_create_gc(c, g, w, mask, values);

	/* создать окно */
	w = xcb_generate_id(c);
	mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	values[0] = s->white_pixel;
	values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS;
	xcb_create_window(c, s->root_depth, w, s->root, 10, 10, 200, 200, 1,
			XCB_WINDOW_CLASS_INPUT_OUTPUT, s->root_visual, mask, values);

	/* отобразить окно */
	xcb_map_window(c, w);

	xcb_flush(c);

	/* event loop */
	while (!done && (e = xcb_wait_for_event(c))) {
		switch (e->response_type & ~0x80) {
		case XCB_EXPOSE: /* отрисовать или перерисовать окно */
			xcb_poly_fill_rectangle(c, w, g, 1, &r);
			xcb_flush(c);
			break;
		case XCB_KEY_PRESS: /* выйти по нажатию клавиши */
			done = 1;
			break;
		}
		free(e);
	}
	/* закрыть соединение с сервером */
	xcb_disconnect(c);

	return 0;
}

double get_time(void) {
	struct timeval timev;

	gettimeofday(&timev, NULL);

	return (double) timev.tv_sec + (((double) timev.tv_usec) / 1000000);
}

int xcbtest2() {
	xcb_connection_t *c;
	xcb_atom_t *atoms;
	xcb_intern_atom_cookie_t *cs;
	char **names;
	int count;
	int i;
	double start;
	double end;
	double diff;

	/* Xlib */
	Display *disp;
	Atom *atoms_x;
	double diff_x;

	c = xcb_connect(NULL, NULL);

	count = 500;
	atoms = (xcb_atom_t *) malloc(count * sizeof(atoms));
	names = (char **) malloc(count * sizeof(char *));

	/* init names */
	for (i = 0; i < count; ++i) {
		char buf[100];

		sprintf(buf, "NAME%d", i);
		names[i] = strdup(buf);
	}

	/* bad use */
	start = get_time();

	for (i = 0; i < count; ++i)
		atoms[i] = xcb_intern_atom_reply(c,
				xcb_intern_atom(c, 0, strlen(names[i]), names[i]),
				NULL)->atom;

	end = get_time();
	diff = end - start;
	printf("bad use time  : %f\n", diff);

	/* good use */
	start = get_time();

	cs = (xcb_intern_atom_cookie_t *) malloc(
			count * sizeof(xcb_intern_atom_cookie_t));
	for (i = 0; i < count; ++i)
		cs[i] = xcb_intern_atom(c, 0, strlen(names[i]), names[i]);

	for (i = 0; i < count; ++i) {
		xcb_intern_atom_reply_t *r;

		r = xcb_intern_atom_reply(c, cs[i], 0);
		if (r)
			atoms[i] = r->atom;
		free(r);
	}

	end = get_time();
	printf("good use time : %f\n", end - start);
	printf("ratio         : %f\n", diff / (end - start));
	diff = end - start;

	/* free var */
	free(atoms);
	free(cs);

	xcb_disconnect(c);

	/* Xlib */
	disp = XOpenDisplay(getenv("DISPLAY"));

	atoms_x = (Atom *) malloc(count * sizeof(atoms_x));

	start = get_time();

	for (i = 0; i < count; ++i)
		atoms_x[i] = XInternAtom(disp, names[i], 0);

	end = get_time();
	diff_x = end - start;
	printf("Xlib use time : %f\n", diff_x);
	printf("ratio         : %f\n", diff_x / diff);

	free(atoms_x);
	for (i = 0; i < count; ++i)
		free(names[i]);
	free(names);

	XCloseDisplay(disp);

	return 0;
}

void test_chart() {
	graphics::Chart chart;
	chart.runChart();
	chart.addPoint(0.0, 0.0);
	chart.addPoint(4.0, 4.0);
	chart.addPoint(6.0, 6.0);
}
