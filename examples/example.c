// This little example program renders to a Wayland window with an OpenGL 3.2
// core profile. The program does not include any GL headers and does not need
// to link to libGL. It declares the needed GL symbols itself and obtains the
// GL functions with waffle_dl_sym().

#include <waffle/waffle.h>

enum {
    GL_COLOR_BUFFER_BIT = 0x00004000,
};

static void (*glClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
static void (*glClear)(GLbitfield mask);

int
main()
{
    const int32_t init_attrs[] = {
        WAFFLE_PLATFORM, WAFFLE_PLATFORM_WAYLAND,
        0,
    };

    const int32_t config_attrs[] = {
        WAFFLE_CONTEXT_API,                 WAFFLE_CONTEXT_OPENGL,
        WAFFLE_CONTEXT_MAJOR_VERSION,       3,
        WAFFLE_CONTEXT_MINOR_VERSION,       2,
        WAFFLE_CONTEXT_PROFILE,             WAFFLE_CONTEXT_CORE_PROFILE,

        WAFFLE_RED_SIZE,            8,
        WAFFLE_BLUE_SIZE,           8,
        WAFFLE_GREEN_SIZE,          8,

        0,
    };

    // Setup waffle objects.
    waffle_init(init_attrs);
    struct waffle_display *dpy = waffle_display_connect(NULL);
    struct waffle_config *config = waffle_config_choose(dpy, config_attrs);
    struct waffle_window *window = waffle_window_create(dpy, width, height);
    struct waffle_context *ctx = waffle_context_create(config, NULL);

    glClearColor = waffle_dl_sym(WAFFLE_DL_OPENGL, "glClearColor");
    glClear = waffle_dl_sym(WAFFLE_DL_OPENGL, "glClear");

    waffle_make_current(dpy, window, ctx);

    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap before showing window so the window content is not garbage.
    waffle_window_swap_buffers(window);
    waffle_window_show(window);

    // Teardown waffle.
    waffle_make_current(dpy, NULL, NULL);
    waffle_window_destroy(window);
    waffle_context_destroy(ctx);
    waffle_config_destroy(config);
    waffle_display_disconnect(dpy);

    return 0;
}
