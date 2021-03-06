 /*
  * UAE - The Un*x Amiga Emulator
  *
  * Interface to the graphics system (X, SVGAlib)
  *
  * Copyright 1995-1997 Bernd Schmidt
  */

typedef uae_u32 xcolnr;

typedef int (*allocfunc_type)(int, int, int, xcolnr *);

extern xcolnr xcolors[4096];
extern xcolnr xcolors_16[4096];
extern xcolnr xcolors_32[4096];
extern uae_u32 p96_rgbx16[65536];

extern int graphics_setup (void);
extern int graphics_init (void);
extern void graphics_leave (void);
extern void handle_events (void);
extern int handle_msgpump (void);
extern void setup_brkhandler (void);
extern int isfullscreen (void);
extern void toggle_fullscreen (int);
extern void toggle_mousegrab (void);
extern void desktop_coords (int *dw, int *dh, int *x, int *y, int *w, int *h);
extern bool vsync_switchmode (int, int);
extern void doflashscreen (void);
extern int flashscreen;
extern void updatedisplayarea (void);

extern void flush_line (int);
extern void flush_block (int, int);
extern void flush_screen (int, int);
extern void flush_clear_screen (void);

extern int lockscr (int);
extern void unlockscr (void);

extern int debuggable (void);
extern void LED (int);
extern void screenshot (int,int);

extern int bits_in_mask (unsigned long mask);
extern int mask_shift (unsigned long mask);
extern unsigned int doMask (int p, int bits, int shift);
extern unsigned int doMask256 (int p, int bits, int shift);
extern void setup_maxcol (int);
extern void alloc_colors256 (int (*)(int, int, int, xcolnr *));
extern void alloc_colors64k (int, int, int, int, int, int, int, int, int, int);
extern void alloc_colors_rgb (int rw, int gw, int bw, int rs, int gs, int bs, int aw, int as, int alpha, int byte_swap,
			      uae_u32 *rc, uae_u32 *gc, uae_u32 *bc);
extern void alloc_colors_picasso (int rw, int gw, int bw, int rs, int gs, int bs, int rgbfmt);
extern void setup_greydither (int bits, allocfunc_type allocfunc);
extern void setup_greydither_maxcol (int maxcol, allocfunc_type allocfunc);
extern void setup_dither (int bits, allocfunc_type allocfunc);
extern void DitherLine (uae_u8 *l, uae_u16 *r4g4b4, int x, int y, uae_s16 len, int bits) ASM_SYM_FOR_FUNC("DitherLine");
extern int getvsyncrate (int hz);

struct vidbuf_description
{
    /* Function implemented by graphics driver */
    void (*flush_line)         (struct vidbuf_description *gfxinfo, int line_no);
    void (*flush_block)        (struct vidbuf_description *gfxinfo, int first_line, int end_line);
    void (*flush_screen)       (struct vidbuf_description *gfxinfo, int first_line, int end_line);
    void (*flush_clear_screen) (struct vidbuf_description *gfxinfo);
    int  (*lockscr)            (struct vidbuf_description *gfxinfo);
    void (*unlockscr)          (struct vidbuf_description *gfxinfo);

    /* The graphics code has a choice whether it wants to use a large buffer
     * for the whole display, or only a small buffer for a single line.
     * If you use a large buffer:
     *   - set bufmem to point at it
     *   - set linemem to 0
     *   - if memcpy within bufmem would be very slow, i.e. because bufmem is
     *     in graphics card memory, also set emergmem to point to a buffer
     *     that is large enough to hold a single line.
     *   - implement flush_line to be a no-op.
     * If you use a single line buffer:
     *   - set bufmem and emergmem to 0
     *   - set linemem to point at your buffer
     *   - implement flush_line to copy a single line to the screen
     */
    uae_u8 *bufmem, *bufmemend;
    uae_u8 *realbufmem;
    uae_u8 *linemem;
    uae_u8 *emergmem;
    int rowbytes; /* Bytes per row in the memory pointed at by bufmem. */
    int pixbytes; /* Bytes per pixel. */
    int width;
    int height;
    int maxblocklines; /* Set to 0 if you want calls to flush_line after each drawn line, or the number of
			* lines that flush_block wants to/can handle (it isn't really useful to use another
			* value than maxline here). */
};

extern struct vidbuf_description gfxvidinfo;

/* For ports using tui.c, this should be built by graphics_setup(). */
extern struct bstring *video_mode_menu;
extern void vidmode_menu_selected(int);

