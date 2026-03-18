/*
 * ============================================================
 * AL-KHAIN (الخائن) - Among Us Clone for PS3 HEN
 * Platform : PS3 HEN / CFW
 * SDK      : PSL1GHT (open-source)
 * Author   : Generated for PS3 homebrew
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <malloc.h>
#include <rsx/rsx.h>
#include <rsx/gcm_sys.h>
#include <sysutil/video.h>
#include <sysutil/sysutil.h>
#include <io/pad.h>
#include <sys/time.h>
#include <unistd.h>

/* ============================================================
 * SCREEN / MEMORY
 * ============================================================ */
#define HOST_SIZE    (32*1024*1024)
#define NUM_BUFFERS  2

static gcmContextData  *rsx_ctx  = NULL;
static void            *host_addr= NULL;
static u32             *fb[NUM_BUFFERS];
static u32              fb_off[NUM_BUFFERS];
static u32              cur_buf  = 0;
static u32              SCR_W    = 1280;
static u32              SCR_H    = 720;

/* ============================================================
 * COLORS  (0xAARRGGBB)
 * ============================================================ */
#define C_BG        0xFF0A0E1A
#define C_PANEL     0xFF131E30
#define C_BORDER    0xFF2A4870
#define C_WHITE     0xFFEEEEEE
#define C_LGRAY     0xFF9AAABB
#define C_MGRAY     0xFF445566
#define C_DGRAY     0xFF1E2840
#define C_RED       0xFFEE3344
#define C_BLUE      0xFF2255EE
#define C_GREEN     0xFF22CC66
#define C_YELLOW    0xFFFFDD22
#define C_PURPLE    0xFFAA44CC
#define C_CYAN      0xFF22CCEE
#define C_ORANGE    0xFFFF8822
#define C_IMP       0xFFFF2233
#define C_CREW      0xFF33FF88
#define C_DEAD      0xFF444455
#define C_BLACK     0xFF000000
#define C_HLITE     0xFF4A8FD9

static const u32 PLAYER_COLORS[6] = {
    C_RED, C_BLUE, C_GREEN, C_YELLOW, C_PURPLE, C_CYAN
};
static const char *PLAYER_NAMES[6] = {
    "RED", "BLUE", "GREEN", "YELLOW", "PURPLE", "CYAN"
};

/* ============================================================
 * 8x8 BITMAP FONT  (ASCII 32-127)
 * ============================================================ */
static const u8 FONT[96][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* 32 SP */
    {0x18,0x3C,0x3C,0x18,0x18,0x00,0x18,0x00}, /* 33  ! */
    {0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00}, /* 34  " */
    {0x36,0x36,0x7F,0x36,0x7F,0x36,0x36,0x00}, /* 35  # */
    {0x0C,0x3E,0x03,0x1E,0x30,0x1F,0x0C,0x00}, /* 36  $ */
    {0x00,0x63,0x33,0x18,0x0C,0x66,0x63,0x00}, /* 37  % */
    {0x1C,0x36,0x1C,0x6E,0x3B,0x33,0x6E,0x00}, /* 38  & */
    {0x06,0x06,0x03,0x00,0x00,0x00,0x00,0x00}, /* 39  ' */
    {0x18,0x0C,0x06,0x06,0x06,0x0C,0x18,0x00}, /* 40  ( */
    {0x06,0x0C,0x18,0x18,0x18,0x0C,0x06,0x00}, /* 41  ) */
    {0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00}, /* 42  * */
    {0x00,0x0C,0x0C,0x3F,0x0C,0x0C,0x00,0x00}, /* 43  + */
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x06}, /* 44  , */
    {0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00}, /* 45  - */
    {0x00,0x00,0x00,0x00,0x00,0x0C,0x0C,0x00}, /* 46  . */
    {0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00}, /* 47  / */
    {0x3E,0x63,0x73,0x7B,0x6F,0x67,0x3E,0x00}, /* 48  0 */
    {0x0C,0x0E,0x0C,0x0C,0x0C,0x0C,0x3F,0x00}, /* 49  1 */
    {0x1E,0x33,0x30,0x1C,0x06,0x33,0x3F,0x00}, /* 50  2 */
    {0x1E,0x33,0x30,0x1C,0x30,0x33,0x1E,0x00}, /* 51  3 */
    {0x38,0x3C,0x36,0x33,0x7F,0x30,0x78,0x00}, /* 52  4 */
    {0x3F,0x03,0x1F,0x30,0x30,0x33,0x1E,0x00}, /* 53  5 */
    {0x1C,0x06,0x03,0x1F,0x33,0x33,0x1E,0x00}, /* 54  6 */
    {0x3F,0x33,0x30,0x18,0x0C,0x0C,0x0C,0x00}, /* 55  7 */
    {0x1E,0x33,0x33,0x1E,0x33,0x33,0x1E,0x00}, /* 56  8 */
    {0x1E,0x33,0x33,0x3E,0x30,0x18,0x0E,0x00}, /* 57  9 */
    {0x00,0x0C,0x0C,0x00,0x00,0x0C,0x0C,0x00}, /* 58  : */
    {0x00,0x0C,0x0C,0x00,0x00,0x0C,0x0C,0x06}, /* 59  ; */
    {0x18,0x0C,0x06,0x03,0x06,0x0C,0x18,0x00}, /* 60  < */
    {0x00,0x00,0x3F,0x00,0x00,0x3F,0x00,0x00}, /* 61  = */
    {0x06,0x0C,0x18,0x30,0x18,0x0C,0x06,0x00}, /* 62  > */
    {0x1E,0x33,0x30,0x18,0x0C,0x00,0x0C,0x00}, /* 63  ? */
    {0x3E,0x63,0x7B,0x7B,0x7B,0x03,0x1E,0x00}, /* 64  @ */
    {0x0C,0x1E,0x33,0x33,0x3F,0x33,0x33,0x00}, /* 65  A */
    {0x3F,0x66,0x66,0x3E,0x66,0x66,0x3F,0x00}, /* 66  B */
    {0x3C,0x66,0x03,0x03,0x03,0x66,0x3C,0x00}, /* 67  C */
    {0x1F,0x36,0x66,0x66,0x66,0x36,0x1F,0x00}, /* 68  D */
    {0x7F,0x46,0x16,0x1E,0x16,0x46,0x7F,0x00}, /* 69  E */
    {0x7F,0x46,0x16,0x1E,0x16,0x06,0x0F,0x00}, /* 70  F */
    {0x3C,0x66,0x03,0x03,0x73,0x66,0x7C,0x00}, /* 71  G */
    {0x33,0x33,0x33,0x3F,0x33,0x33,0x33,0x00}, /* 72  H */
    {0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00}, /* 73  I */
    {0x78,0x30,0x30,0x30,0x33,0x33,0x1E,0x00}, /* 74  J */
    {0x67,0x66,0x36,0x1E,0x36,0x66,0x67,0x00}, /* 75  K */
    {0x0F,0x06,0x06,0x06,0x46,0x66,0x7F,0x00}, /* 76  L */
    {0x63,0x77,0x7F,0x7F,0x6B,0x63,0x63,0x00}, /* 77  M */
    {0x63,0x67,0x6F,0x7B,0x73,0x63,0x63,0x00}, /* 78  N */
    {0x1C,0x36,0x63,0x63,0x63,0x36,0x1C,0x00}, /* 79  O */
    {0x3F,0x66,0x66,0x3E,0x06,0x06,0x0F,0x00}, /* 80  P */
    {0x1E,0x33,0x33,0x33,0x3B,0x1E,0x38,0x00}, /* 81  Q */
    {0x3F,0x66,0x66,0x3E,0x36,0x66,0x67,0x00}, /* 82  R */
    {0x1E,0x33,0x07,0x0E,0x38,0x33,0x1E,0x00}, /* 83  S */
    {0x3F,0x2D,0x0C,0x0C,0x0C,0x0C,0x1E,0x00}, /* 84  T */
    {0x33,0x33,0x33,0x33,0x33,0x33,0x3F,0x00}, /* 85  U */
    {0x33,0x33,0x33,0x33,0x33,0x1E,0x0C,0x00}, /* 86  V */
    {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00}, /* 87  W */
    {0x63,0x63,0x36,0x1C,0x1C,0x36,0x63,0x00}, /* 88  X */
    {0x33,0x33,0x33,0x1E,0x0C,0x0C,0x1E,0x00}, /* 89  Y */
    {0x7F,0x63,0x31,0x18,0x4C,0x66,0x7F,0x00}, /* 90  Z */
    {0x1E,0x06,0x06,0x06,0x06,0x06,0x1E,0x00}, /* 91  [ */
    {0x03,0x06,0x0C,0x18,0x30,0x60,0x40,0x00}, /* 92  \ */
    {0x1E,0x18,0x18,0x18,0x18,0x18,0x1E,0x00}, /* 93  ] */
    {0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00}, /* 94  ^ */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF}, /* 95  _ */
    {0x0C,0x0C,0x18,0x00,0x00,0x00,0x00,0x00}, /* 96  ` */
    {0x00,0x00,0x1E,0x30,0x3E,0x33,0x6E,0x00}, /* 97  a */
    {0x07,0x06,0x06,0x3E,0x66,0x66,0x3B,0x00}, /* 98  b */
    {0x00,0x00,0x1E,0x33,0x03,0x33,0x1E,0x00}, /* 99  c */
    {0x38,0x30,0x30,0x3e,0x33,0x33,0x6E,0x00}, /* 100 d */
    {0x00,0x00,0x1E,0x33,0x3f,0x03,0x1E,0x00}, /* 101 e */
    {0x1C,0x36,0x06,0x0f,0x06,0x06,0x0F,0x00}, /* 102 f */
    {0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x1F}, /* 103 g */
    {0x07,0x06,0x36,0x6E,0x66,0x66,0x67,0x00}, /* 104 h */
    {0x0C,0x00,0x0E,0x0C,0x0C,0x0C,0x1E,0x00}, /* 105 i */
    {0x30,0x00,0x30,0x30,0x30,0x33,0x33,0x1E}, /* 106 j */
    {0x07,0x06,0x66,0x36,0x1E,0x36,0x67,0x00}, /* 107 k */
    {0x0E,0x0C,0x0C,0x0C,0x0C,0x0C,0x1E,0x00}, /* 108 l */
    {0x00,0x00,0x33,0x7F,0x7F,0x6B,0x63,0x00}, /* 109 m */
    {0x00,0x00,0x1F,0x33,0x33,0x33,0x33,0x00}, /* 110 n */
    {0x00,0x00,0x1E,0x33,0x33,0x33,0x1E,0x00}, /* 111 o */
    {0x00,0x00,0x3B,0x66,0x66,0x3E,0x06,0x0F}, /* 112 p */
    {0x00,0x00,0x6E,0x33,0x33,0x3E,0x30,0x78}, /* 113 q */
    {0x00,0x00,0x3B,0x6E,0x66,0x06,0x0F,0x00}, /* 114 r */
    {0x00,0x00,0x3E,0x03,0x1E,0x30,0x1F,0x00}, /* 115 s */
    {0x08,0x0C,0x3E,0x0C,0x0C,0x2C,0x18,0x00}, /* 116 t */
    {0x00,0x00,0x33,0x33,0x33,0x33,0x6E,0x00}, /* 117 u */
    {0x00,0x00,0x33,0x33,0x33,0x1E,0x0C,0x00}, /* 118 v */
    {0x00,0x00,0x63,0x6B,0x7F,0x7F,0x36,0x00}, /* 119 w */
    {0x00,0x00,0x63,0x36,0x1C,0x36,0x63,0x00}, /* 120 x */
    {0x00,0x00,0x33,0x33,0x33,0x3E,0x30,0x1F}, /* 121 y */
    {0x00,0x00,0x3F,0x19,0x0C,0x26,0x3F,0x00}, /* 122 z */
    {0x38,0x0C,0x0C,0x07,0x0C,0x0C,0x38,0x00}, /* 123 { */
    {0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00}, /* 124 | */
    {0x07,0x0C,0x0C,0x38,0x0C,0x0C,0x07,0x00}, /* 125 } */
    {0x6E,0x3B,0x00,0x00,0x00,0x00,0x00,0x00}, /* 126 ~ */
    {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}  /* 127 filled block */
};

/* ============================================================
 * DRAWING PRIMITIVES
 * ============================================================ */
static inline void putpixel(int x, int y, u32 color) {
    if (x < 0 || y < 0 || (u32)x >= SCR_W || (u32)y >= SCR_H) return;
    fb[cur_buf][y * SCR_W + x] = color;
}

static void fillRect(int x, int y, int w, int h, u32 color) {
    int x2 = x + w, y2 = y + h;
    if (x  < 0)  x  = 0;
    if (y  < 0)  y  = 0;
    if (x2 > (int)SCR_W) x2 = (int)SCR_W;
    if (y2 > (int)SCR_H) y2 = (int)SCR_H;
    for (int row = y; row < y2; row++) {
        u32 *line = fb[cur_buf] + row * SCR_W;
        for (int col = x; col < x2; col++)
            line[col] = color;
    }
}

static void drawRect(int x, int y, int w, int h, u32 color) {
    fillRect(x,     y,     w, 1, color);
    fillRect(x,     y+h-1, w, 1, color);
    fillRect(x,     y,     1, h, color);
    fillRect(x+w-1, y,     1, h, color);
}

static void drawChar(int x, int y, char c, u32 color, int scale) {
    if (c < 32 || c > 127) c = '?';
    const u8 *bm = FONT[(u8)c - 32];
    for (int row = 0; row < 8; row++) {
        u8 bits = bm[row];
        for (int col = 0; col < 8; col++) {
            if (bits & (0x80 >> col))
                fillRect(x + col*scale, y + row*scale, scale, scale, color);
        }
    }
}

static void drawText(int x, int y, const char *str, u32 color, int scale) {
    int cx = x;
    while (*str) {
        if (*str == '\n') { cx = x; y += 8*scale + 2; }
        else { drawChar(cx, y, *str, color, scale); cx += 8*scale + scale; }
        str++;
    }
}

/* printf-style drawText */
static void drawTextF(int x, int y, u32 color, int scale, const char *fmt, ...) {
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    drawText(x, y, buf, color, scale);
}

static void drawFilledCircle(int cx, int cy, int r, u32 color) {
    for (int dy = -r; dy <= r; dy++) {
        int dx_max = (int)( r * r - dy * dy );
        if (dx_max < 0) continue;
        /* integer sqrt */
        int dx = 0;
        while ((dx+1)*(dx+1) <= dx_max) dx++;
        fillRect(cx - dx, cy + dy, 2*dx+1, 1, color);
    }
}

/* ============================================================
 * SCREEN FLIP
 * ============================================================ */
static void waitFlip() {
    while (gcmGetFlipStatus() != 0) usleep(200);
    gcmResetFlipStatus();
}

static void doFlip() {
    rsxFlushBuffer(rsx_ctx);
    gcmFlip(rsx_ctx, cur_buf);
    cur_buf ^= 1;
    waitFlip();
}

/* ============================================================
 * SCREEN INIT
 * ============================================================ */
static void initScreen() {
    host_addr = memalign(1024*1024, HOST_SIZE);
    rsx_ctx   = rsxInit(0x10000, HOST_SIZE, host_addr);

    videoState  vstate;
    videoGetState(0, 0, &vstate);
    videoResolution res;
    videoGetResolution(vstate.displayMode.resolution, &res);
    SCR_W = res.width;
    SCR_H = res.height;

    videoConfiguration vcfg;
    memset(&vcfg, 0, sizeof(vcfg));
    vcfg.resolution = vstate.displayMode.resolution;
    vcfg.format     = VIDEO_BUFFER_FORMAT_XRGB;
    vcfg.pitch      = SCR_W * 4;
    videoSetupExtended(&vcfg);
    gcmSetFlipMode(GCM_FLIP_VSYNC);

    for (int i = 0; i < NUM_BUFFERS; i++) {
        fb[i] = (u32*)rsxMemalign(64, SCR_W * SCR_H * 4);
        rsxAddressToOffset(fb[i], &fb_off[i]);
        gcmSetDisplayBuffer(i, fb_off[i], vcfg.pitch, SCR_W, SCR_H);
        memset(fb[i], 0, SCR_W * SCR_H * 4);
    }
    gcmResetFlipStatus();
    waitFlip();
    gcmFlip(rsx_ctx, 0);
    waitFlip();
}

/* ============================================================
 * INPUT
 * ============================================================ */
static u32 g_btns      = 0;  /* currently held */
static u32 g_btns_prev = 0;  /* last frame */

static void updateInput() {
    g_btns_prev = g_btns;
    padInfo pi;
    ioPadGetInfo(&pi);
    if (pi.status[0]) {
        padData pd;
        ioPadGetData(0, &pd);
        g_btns = pd.BTN_CROSS   ? PAD_CTRL_CROSS    : 0;
        g_btns|= pd.BTN_CIRCLE  ? PAD_CTRL_CIRCLE   : 0;
        g_btns|= pd.BTN_TRIANGLE? PAD_CTRL_TRIANGLE : 0;
        g_btns|= pd.BTN_SQUARE  ? PAD_CTRL_SQUARE   : 0;
        g_btns|= pd.BTN_UP      ? PAD_CTRL_UP       : 0;
        g_btns|= pd.BTN_DOWN    ? PAD_CTRL_DOWN     : 0;
        g_btns|= pd.BTN_LEFT    ? PAD_CTRL_LEFT     : 0;
        g_btns|= pd.BTN_RIGHT   ? PAD_CTRL_RIGHT    : 0;
        g_btns|= pd.BTN_L1      ? PAD_CTRL_L1       : 0;
        g_btns|= pd.BTN_R1      ? PAD_CTRL_R1       : 0;
        g_btns|= pd.BTN_START   ? PAD_CTRL_START    : 0;
        g_btns|= pd.BTN_SELECT  ? PAD_CTRL_SELECT   : 0;
    } else {
        g_btns = 0;
    }
}

static inline bool btnPressed(u32 mask) { return (g_btns & mask) && !(g_btns_prev & mask); }
static inline bool btnHeld(u32 mask)    { return (g_btns & mask) != 0; }

/* ============================================================
 * SYSUTIL CALLBACK
 * ============================================================ */
static volatile bool g_running = true;

static void sysutil_cb(u64 status, u64 param, void *ud) {
    (void)param; (void)ud;
    if (status == SYSUTIL_EXIT_GAME) g_running = false;
}

/* ============================================================
 * GAME DATA
 * ============================================================ */
#define MAX_PLAYERS   6
#define MIN_PLAYERS   4
#define NUM_ROOMS     6
#define NUM_TASKS     3
#define MAX_TASKS_PP  3   /* tasks per crewmate */

typedef enum {
    ROLE_CREWMATE,
    ROLE_IMPOSTOR
} Role;

typedef enum {
    STATE_MENU,
    STATE_SETUP,
    STATE_REVEAL,
    STATE_PLAY,
    STATE_TASK,
    STATE_MEETING,
    STATE_VOTE_RESULT,
    STATE_WIN_CREW,
    STATE_WIN_IMP
} GameState;

/* Room connection table */
static const char *ROOM_NAMES[NUM_ROOMS] = {
    "CAFETERIA", "NAV ROOM", "WEAPONS",
    "ADMIN",     "ENGINE",   "STORAGE"
};

/*  Adjacency (bidirectional):
 *  CAFE(0) <-> NAV(1)
 *  CAFE(0) <-> ADMIN(3)
 *  NAV(1)  <-> WEAPONS(2)
 *  WEAPONS(2) <-> STORAGE(5)
 *  ADMIN(3) <-> ENGINE(4)
 *  ENGINE(4) <-> STORAGE(5)
 */
static const int ROOM_ADJ[NUM_ROOMS][4] = {
    {1, 3,-1,-1}, /* CAFE:  NAV, ADMIN */
    {0, 2,-1,-1}, /* NAV:   CAFE, WEAPONS */
    {1, 5,-1,-1}, /* WEAP:  NAV, STORAGE */
    {0, 4,-1,-1}, /* ADMIN: CAFE, ENGINE */
    {3, 5,-1,-1}, /* ENG:   ADMIN, STORAGE */
    {2, 4,-1,-1}  /* STOR:  WEAP, ENG */
};

typedef struct {
    int  room;
    Role role;
    bool alive;
    bool tasks[NUM_ROOMS]; /* task done in each room? (1 task per room for crewmates) */
    bool voted;
    int  vote_for;         /* -1 = skip */
} Player;

static int       g_num_players   = 4;
static Player    g_players[MAX_PLAYERS];
static int       g_dead_body_room[MAX_PLAYERS]; /* room of body or -1 */
static GameState g_state         = STATE_MENU;
static int       g_active_player = 0;   /* current player index in PLAY */
static int       g_reveal_idx    = 0;   /* index in reveal sequence */
static int       g_meeting_caller= -1;
static bool      g_body_reported = false;
static int       g_sel           = 0;   /* generic menu selection */
static int       g_task_room     = 0;   /* which room task is in */
static int       g_task_type     = 0;   /* 0=wires 1=upload 2=fuel */
static int       g_task_step     = 0;   /* progress in task */
static int       g_hold_ticks    = 0;
static int       g_fuel_val      = 0;
static int       g_vote_result_timer = 0;
static int       g_ejected       = -1;  /* who was ejected */
static bool      g_ejected_was_imp = false;
static int       g_msg_timer     = 0;
static char      g_message[64]   = "";

/* Task sequence for wire fix */
static const u32 WIRE_SEQ[3] = {PAD_CTRL_CROSS, PAD_CTRL_CIRCLE, PAD_CTRL_TRIANGLE};
static const char *WIRE_BTN_NAMES[3] = {"[X]", "[O]", "[TRI]"};

/* ============================================================
 * GAME HELPERS
 * ============================================================ */
static int  countAlive(bool onlyCrew) {
    int n = 0;
    for (int i = 0; i < g_num_players; i++) {
        if (!g_players[i].alive) continue;
        if (onlyCrew && g_players[i].role == ROLE_IMPOSTOR) continue;
        n++;
    }
    return n;
}
static int countImpostorsAlive() {
    int n = 0;
    for (int i = 0; i < g_num_players; i++)
        if (g_players[i].alive && g_players[i].role == ROLE_IMPOSTOR) n++;
    return n;
}
static int crewTasksDone() {
    int done=0, total=0;
    for (int i = 0; i < g_num_players; i++) {
        if (g_players[i].role != ROLE_CREWMATE) continue;
        for (int r = 0; r < NUM_ROOMS; r++) {
            total++;
            if (g_players[i].tasks[r]) done++;
        }
    }
    if (total == 0) return 100;
    return done * 100 / total;
}
static bool anyBodyIn(int room) {
    for (int i = 0; i < g_num_players; i++)
        if (g_dead_body_room[i] == room) return true;
    return false;
}
static void checkWin() {
    /* Crew wins if all tasks done */
    if (crewTasksDone() >= 100) { g_state = STATE_WIN_CREW; return; }
    /* Impostor wins if impostors >= crew */
    int ai = countImpostorsAlive();
    int ac = countAlive(true);
    if (ai >= ac && ai > 0) { g_state = STATE_WIN_IMP; return; }
    /* Crew wins if no impostors */
    if (ai == 0) { g_state = STATE_WIN_CREW; return; }
}

/* Simple PRNG */
static u32 g_seed = 12345;
static u32 myrand() { g_seed ^= g_seed << 13; g_seed ^= g_seed >> 17; g_seed ^= g_seed << 5; return g_seed; }

static void initGame() {
    /* seed from "time" */
    g_seed = 98765 + g_num_players * 31337;

    /* assign roles: 1 impostor for 4-5, 2 for 6 */
    int num_impostors = (g_num_players >= 6) ? 2 : 1;

    for (int i = 0; i < g_num_players; i++) {
        g_players[i].role  = ROLE_CREWMATE;
        g_players[i].alive = true;
        g_players[i].room  = (int)(myrand() % NUM_ROOMS);
        g_players[i].voted = false;
        g_players[i].vote_for = -1;
        for (int r = 0; r < NUM_ROOMS; r++)
            g_players[i].tasks[r] = false;
        g_dead_body_room[i] = -1;
    }

    /* pick impostor(s) - Fisher-Yates partial shuffle */
    int order[MAX_PLAYERS];
    for (int i = 0; i < g_num_players; i++) order[i] = i;
    for (int i = 0; i < num_impostors; i++) {
        int j = i + (int)(myrand() % (g_num_players - i));
        int tmp = order[i]; order[i] = order[j]; order[j] = tmp;
        g_players[order[i]].role = ROLE_IMPOSTOR;
    }

    /* impostors have no tasks */
    for (int i = 0; i < g_num_players; i++) {
        if (g_players[i].role == ROLE_IMPOSTOR) {
            for (int r = 0; r < NUM_ROOMS; r++)
                g_players[i].tasks[r] = true; /* mark done so doesn't count */
        }
    }

    g_reveal_idx    = 0;
    g_active_player = 0;
    g_meeting_caller= -1;
    g_body_reported = false;
    g_task_step     = 0;
    g_hold_ticks    = 0;
    g_fuel_val      = 0;
    g_sel           = 0;
    g_msg_timer     = 0;
    g_state         = STATE_REVEAL;
}

/* ============================================================
 * DRAW HELPERS
 * ============================================================ */
static void drawPanel(int x, int y, int w, int h) {
    fillRect(x, y, w, h, C_PANEL);
    drawRect(x, y, w, h, C_BORDER);
}

static void drawProgressBar(int x, int y, int w, int h, int pct, u32 fillColor) {
    fillRect(x,   y, w, h, C_DGRAY);
    drawRect(x,   y, w, h, C_BORDER);
    if (pct > 0) {
        int fw = w * pct / 100;
        if (fw < 1) fw = 1;
        fillRect(x+1, y+1, fw-2, h-2, fillColor);
    }
}

static void drawHeader(const char *title) {
    fillRect(0, 0, (int)SCR_W, 50, C_DGRAY);
    drawRect(0, 0, (int)SCR_W, 50, C_BORDER);
    int tx = ((int)SCR_W - (int)strlen(title) * 10) / 2;
    drawText(tx, 13, title, C_WHITE, 2);
}

/* mini player icon */
static void drawPlayerIcon(int cx, int cy, int r, u32 color, bool dead, bool selected) {
    if (selected) drawFilledCircle(cx, cy, r+4, C_HLITE);
    drawFilledCircle(cx, cy, r, dead ? C_DEAD : color);
    if (dead) {
        /* X mark */
        int s = r/2;
        for (int d = -s; d <= s; d++) {
            putpixel(cx+d, cy+d, C_WHITE);
            putpixel(cx+d, cy-d, C_WHITE);
        }
    }
}

static void setMessage(const char *msg, int ticks) {
    strncpy(g_message, msg, sizeof(g_message)-1);
    g_msg_timer = ticks;
}

/* ============================================================
 * STATE: MENU
 * ============================================================ */
static void updateMenu() {
    if (btnPressed(PAD_CTRL_CROSS)) {
        g_num_players = MIN_PLAYERS;
        g_state = STATE_SETUP;
        g_sel   = g_num_players - MIN_PLAYERS;
    }
}
static void drawMenu() {
    fillRect(0, 0, (int)SCR_W, (int)SCR_H, C_BG);
    drawHeader("  AL-KHAIN  -  THE TRAITOR  ");
    /* Title art */
    drawText(SCR_W/2 - 120, 100, "AMONG US CLONE", C_ORANGE, 2);
    drawText(SCR_W/2 - 180, 130, "for PS3 HEN  -  Local Multiplayer", C_LGRAY, 1);

    /* big icon */
    drawFilledCircle(SCR_W/2, 280, 50, C_RED);
    drawFilledCircle(SCR_W/2-15, 260, 14, C_WHITE);

    drawPanel(SCR_W/2-180, 370, 360, 80);
    drawText(SCR_W/2 - 100, 390, "Press  [X]  to  START", C_CREW, 2);

    drawText(50, 660, "4 to 6 players  |  Pass controller  |  Local only", C_MGRAY, 1);
}

/* ============================================================
 * STATE: SETUP
 * ============================================================ */
static void updateSetup() {
    if (btnPressed(PAD_CTRL_RIGHT)) { if (g_num_players < MAX_PLAYERS) { g_num_players++; g_sel++; } }
    if (btnPressed(PAD_CTRL_LEFT))  { if (g_num_players > MIN_PLAYERS) { g_num_players--; g_sel--; } }
    if (btnPressed(PAD_CTRL_CROSS)) initGame();
    if (btnPressed(PAD_CTRL_CIRCLE)) g_state = STATE_MENU;
}
static void drawSetup() {
    fillRect(0, 0, (int)SCR_W, (int)SCR_H, C_BG);
    drawHeader("  PLAYER  COUNT  ");
    drawText(SCR_W/2 - 200, 150, "Use  LEFT/RIGHT  to  choose  number  of  players", C_WHITE, 1);

    /* number display */
    char buf[8]; sprintf(buf, "%d", g_num_players);
    drawPanel(SCR_W/2 - 60, 220, 120, 80);
    drawText(SCR_W/2 - 10, 243, buf, C_YELLOW, 3);

    /* player color dots */
    int startX = SCR_W/2 - (g_num_players * 40) / 2;
    for (int i = 0; i < g_num_players; i++)
        drawFilledCircle(startX + i*40 + 20, 380, 16, PLAYER_COLORS[i]);

    drawPanel(SCR_W/2-160, 460, 320, 50);
    drawText(SCR_W/2 - 110, 473, "[X] = START    [O] = BACK", C_WHITE, 1);
}

/* ============================================================
 * STATE: REVEAL
 * ============================================================ */
static void updateReveal() {
    if (btnPressed(PAD_CTRL_CROSS)) {
        g_reveal_idx++;
        if (g_reveal_idx >= g_num_players) {
            g_state = STATE_PLAY;
            g_active_player = 0;
        }
    }
}
static void drawReveal() {
    fillRect(0, 0, (int)SCR_W, (int)SCR_H, C_BG);
    int idx = g_reveal_idx;
    if (idx >= g_num_players) return;

    char buf[64];
    sprintf(buf, "  PLAYER %s - YOUR ROLE  ", PLAYER_NAMES[idx]);
    drawHeader(buf);

    drawText(SCR_W/2 - 250, 100,
        "PRIVATE: Shield screen from others, then press [X]", C_YELLOW, 1);

    drawFilledCircle(SCR_W/2, 280, 60, PLAYER_COLORS[idx]);

    if (g_players[idx].role == ROLE_IMPOSTOR) {
        drawPanel(SCR_W/2-200, 370, 400, 80);
        drawText(SCR_W/2 - 160, 383, "!!! IMPOSTOR !!!", C_IMP, 2);
        drawText(SCR_W/2 - 220, 415, "Kill crew, don't get caught!", C_LGRAY, 1);
    } else {
        drawPanel(SCR_W/2-200, 370, 400, 80);
        drawText(SCR_W/2 - 100, 383, "CREWMATE", C_CREW, 2);
        drawText(SCR_W/2 - 220, 415, "Complete tasks, find the impostor!", C_LGRAY, 1);
    }

    drawText(SCR_W/2 - 100, 500, "Press  [X]  to  confirm", C_WHITE, 1);
    sprintf(buf, "Player %d of %d", idx+1, g_num_players);
    drawText(SCR_W/2 - 50, 530, buf, C_MGRAY, 1);
}

/* ============================================================
 * STATE: PLAY
 * ============================================================ */
static void updatePlay() {
    Player &p = g_players[g_active_player];
    if (!p.alive) {
        /* skip dead players */
        g_active_player = (g_active_player + 1) % g_num_players;
        return;
    }

    int num_adj = 0;
    while (ROOM_ADJ[p.room][num_adj] >= 0 && num_adj < 4) num_adj++;

    /* Navigate rooms */
    if (btnPressed(PAD_CTRL_UP))   g_sel = ((g_sel - 1) + num_adj) % num_adj;
    if (btnPressed(PAD_CTRL_DOWN)) g_sel = (g_sel + 1) % num_adj;

    /* Move to selected room */
    if (btnPressed(PAD_CTRL_CROSS)) {
        if (num_adj > 0) {
            p.room = ROOM_ADJ[p.room][g_sel];
            g_sel  = 0;
            /* next player */
            int next = (g_active_player + 1) % g_num_players;
            int count = 0;
            while (!g_players[next].alive && count < g_num_players) {
                next = (next + 1) % g_num_players;
                count++;
            }
            g_active_player = next;
        }
    }

    /* Complete task (crewmate) */
    if (btnPressed(PAD_CTRL_SQUARE) && p.role == ROLE_CREWMATE) {
        if (!p.tasks[p.room]) {
            g_task_room = p.room;
            g_task_type = p.room % NUM_TASKS;
            g_task_step = 0;
            g_hold_ticks= 0;
            g_fuel_val  = 0;
            g_state     = STATE_TASK;
        } else {
            setMessage("Task already done here!", 60);
        }
    }

    /* Kill (impostor) */
    if (btnPressed(PAD_CTRL_SQUARE) && p.role == ROLE_IMPOSTOR) {
        /* find crewmate in same room */
        bool killed = false;
        for (int i = 0; i < g_num_players && !killed; i++) {
            if (i == g_active_player) continue;
            if (g_players[i].alive && g_players[i].role == ROLE_CREWMATE && g_players[i].room == p.room) {
                g_players[i].alive = false;
                g_dead_body_room[i] = p.room;
                char buf[64]; sprintf(buf, "%s killed %s!", PLAYER_NAMES[g_active_player], PLAYER_NAMES[i]);
                setMessage(buf, 90);
                killed = true;
            }
        }
        if (!killed) setMessage("No crewmate here to kill!", 60);
        /* next player */
        int next = (g_active_player + 1) % g_num_players;
        int cnt  = 0;
        while (!g_players[next].alive && cnt < g_num_players) { next=(next+1)%g_num_players; cnt++; }
        g_active_player = next;
    }

    /* Report body */
    if (btnPressed(PAD_CTRL_TRIANGLE)) {
        if (anyBodyIn(p.room)) {
            g_meeting_caller= g_active_player;
            g_body_reported = true;
            for (int i = 0; i < g_num_players; i++) {
                /* clear bodies in caller's room */
                if (g_dead_body_room[i] == p.room) g_dead_body_room[i] = -1;
            }
            /* reset votes */
            for (int i = 0; i < g_num_players; i++) { g_players[i].voted=false; g_players[i].vote_for=-1; }
            g_sel   = 0;
            g_state = STATE_MEETING;
        } else {
            setMessage("No body here!", 60);
        }
    }

    /* Emergency meeting */
    if (btnPressed(PAD_CTRL_R1)) {
        g_meeting_caller = g_active_player;
        g_body_reported  = false;
        for (int i = 0; i < g_num_players; i++) { g_players[i].voted=false; g_players[i].vote_for=-1; }
        g_sel   = 0;
        g_state = STATE_MEETING;
    }

    if (g_msg_timer > 0) g_msg_timer--;
    checkWin();
}

static void drawPlay() {
    fillRect(0, 0, (int)SCR_W, (int)SCR_H, C_BG);

    /* Header */
    char hbuf[64];
    sprintf(hbuf, "  TURN: %s  ", PLAYER_NAMES[g_active_player]);
    drawHeader(hbuf);

    Player &p = g_players[g_active_player];

    /* ---- MAP (left panel) ---- */
    drawPanel(20, 60, 500, 400);
    drawText(30, 70, "MAP", C_LGRAY, 1);

    /* Room layout positions */
    const int RX[NUM_ROOMS] = {80,  280, 430, 80,  80,  430};
    const int RY[NUM_ROOMS] = {120, 120, 120, 260, 370, 260};
    const int RW = 120, RH = 60;

    /* draw connections */
    for (int r = 0; r < NUM_ROOMS; r++) {
        for (int a = 0; a < 4; a++) {
            int nb = ROOM_ADJ[r][a];
            if (nb < 0 || nb < r) continue;
            int x1 = RX[r]+RW/2, y1 = RY[r]+RH/2;
            int x2 = RX[nb]+RW/2, y2 = RY[nb]+RH/2;
            /* draw line as series of dots */
            int steps = 20;
            for (int s = 0; s <= steps; s++) {
                int lx = x1 + (x2-x1)*s/steps;
                int ly = y1 + (y2-y1)*s/steps;
                fillRect(lx-1, ly-1, 3, 3, C_MGRAY);
            }
        }
    }

    /* draw rooms */
    for (int r = 0; r < NUM_ROOMS; r++) {
        u32 rc = (r == p.room) ? C_HLITE : C_PANEL;
        fillRect(RX[r], RY[r], RW, RH, rc);
        drawRect(RX[r], RY[r], RW, RH, C_BORDER);
        drawText(RX[r]+4, RY[r]+8, ROOM_NAMES[r], C_WHITE, 1);

        /* players in room */
        int px = RX[r]+4, ic = 0;
        for (int i = 0; i < g_num_players; i++) {
            if (g_players[i].room == r) {
                drawFilledCircle(px + 8 + ic*18, RY[r]+42, 7,
                    g_players[i].alive ? PLAYER_COLORS[i] : C_DEAD,
                    false, (i == g_active_player));
                ic++;
            }
        }
        /* dead body indicator */
        if (anyBodyIn(r)) {
            drawText(RX[r]+RW-20, RY[r]+4, "!", C_IMP, 2);
        }
    }

    /* ---- ACTIONS (right panel) ---- */
    drawPanel(540, 60, 380, 260);
    drawText(550, 70, "ACTIONS", C_LGRAY, 1);

    int num_adj = 0;
    while (num_adj < 4 && ROOM_ADJ[p.room][num_adj] >= 0) num_adj++;

    drawText(550, 95, "Move to room (UP/DOWN + X):", C_LGRAY, 1);
    for (int a = 0; a < num_adj; a++) {
        int nb = ROOM_ADJ[p.room][a];
        u32 ac = (a == g_sel) ? C_HLITE : C_MGRAY;
        if (a == g_sel) drawRect(550, 115 + a*24, 340, 22, C_HLITE);
        char rbuf[32]; sprintf(rbuf, "  %s", ROOM_NAMES[nb]);
        drawText(555, 118 + a*24, rbuf, ac, 1);
    }

    if (p.role == ROLE_CREWMATE) {
        drawText(550, 260, "[SQR] Do Task in current room", C_GREEN, 1);
    } else {
        drawText(550, 260, "[SQR] Kill in current room", C_IMP, 1);
    }
    drawText(550, 278, "[TRI] Report body here", C_YELLOW, 1);
    drawText(550, 296, "[R1]  Emergency Meeting", C_ORANGE, 1);

    /* ---- INFO (right lower panel) ---- */
    drawPanel(540, 330, 380, 130);
    char tb[32];
    sprintf(tb, "Tasks: %d%%", crewTasksDone());
    drawText(550, 340, tb, C_CREW, 1);
    drawProgressBar(550, 360, 350, 16, crewTasksDone(), C_CREW);

    /* player legend */
    for (int i = 0; i < g_num_players; i++) {
        int lx = 550 + (i%3)*120;
        int ly = 385 + (i/3)*22;
        drawFilledCircle(lx+8, ly+6, 7, PLAYER_COLORS[i], false, false);
        drawText(lx+20, ly, PLAYER_NAMES[i], g_players[i].alive ? PLAYER_COLORS[i] : C_DEAD, 1);
        if (!g_players[i].alive) drawText(lx+70, ly, "(X)", C_DEAD, 1);
    }

    /* current room info */
    drawPanel(20, 470, 500, 80);
    sprintf(hbuf, "Current Room: %s", ROOM_NAMES[p.room]);
    drawText(30, 480, hbuf, C_WHITE, 1);
    if (p.role == ROLE_CREWMATE) {
        drawText(30, 500, p.tasks[p.room] ? "Task: DONE" : "Task: PENDING", p.tasks[p.room] ? C_GREEN : C_YELLOW, 1);
    } else {
        drawText(30, 500, "IMPOSTOR: no tasks", C_IMP, 1);
    }

    /* message */
    if (g_msg_timer > 0) {
        drawPanel(SCR_W/2-280, SCR_H-80, 560, 50);
        drawText(SCR_W/2 - (int)(strlen(g_message)*5), SCR_H-65, g_message, C_YELLOW, 1);
    }

    drawText(20, SCR_H-30, "[X]=Move  [SQR]=Task/Kill  [TRI]=Report  [R1]=Meeting", C_MGRAY, 1);
}

/* ============================================================
 * STATE: TASK
 * ============================================================ */
static void updateTask() {
    Player &p = g_players[g_active_player];

    if (btnPressed(PAD_CTRL_CIRCLE)) {
        /* abort */
        g_state = STATE_PLAY;
        return;
    }

    if (g_task_type == 0) {
        /* Wire Fix: press X, O, TRI in order */
        u32 expected = WIRE_SEQ[g_task_step];
        if (btnPressed(expected)) {
            g_task_step++;
            if (g_task_step >= 3) {
                p.tasks[g_task_room] = true;
                setMessage("Task complete! Wires fixed!", 60);
                g_state = STATE_PLAY;
                checkWin();
            }
        } else if (btnPressed(PAD_CTRL_CROSS) || btnPressed(PAD_CTRL_CIRCLE) || btnPressed(PAD_CTRL_TRIANGLE)) {
            /* wrong button */
            g_task_step = 0;
            setMessage("Wrong button! Start over.", 40);
        }
    }
    else if (g_task_type == 1) {
        /* Upload: hold L1 for 180 ticks */
        if (btnHeld(PAD_CTRL_L1)) {
            g_hold_ticks++;
            if (g_hold_ticks >= 180) {
                p.tasks[g_task_room] = true;
                setMessage("Upload complete!", 60);
                g_state = STATE_PLAY;
                checkWin();
            }
        } else {
            if (g_hold_ticks > 0) g_hold_ticks = 0;
        }
    }
    else {
        /* Fuel: press LEFT/RIGHT to fill bar to 100 */
        if (btnPressed(PAD_CTRL_RIGHT)) g_fuel_val += 10;
        if (btnPressed(PAD_CTRL_LEFT))  g_fuel_val -= 5;
        if (g_fuel_val < 0)   g_fuel_val = 0;
        if (g_fuel_val > 100) g_fuel_val = 100;
        if (g_fuel_val >= 100) {
            p.tasks[g_task_room] = true;
            setMessage("Engines fueled!", 60);
            g_state = STATE_PLAY;
            checkWin();
        }
    }
}

static void drawTask() {
    fillRect(0, 0, (int)SCR_W, (int)SCR_H, C_BG);
    char hbuf[64];
    const char *tnames[3] = {"FIX WIRES", "UPLOAD DATA", "FUEL ENGINES"};
    sprintf(hbuf, "  TASK: %s  in %s  ", tnames[g_task_type], ROOM_NAMES[g_task_room]);
    drawHeader(hbuf);

    drawPanel(SCR_W/2-300, 120, 600, 400);

    if (g_task_type == 0) {
        drawText(SCR_W/2 - 200, 150, "Press buttons in order:", C_WHITE, 1);
        for (int i = 0; i < 3; i++) {
            u32 bclr = (i < g_task_step) ? C_GREEN : (i == g_task_step ? C_YELLOW : C_MGRAY);
            drawText(SCR_W/2 - 30 + i*80, 210, WIRE_BTN_NAMES[i], bclr, 2);
        }
        drawText(SCR_W/2 - 150, 310,
            (g_task_step < 3) ? "Press the highlighted button!" : "Done!",
            C_LGRAY, 1);
    }
    else if (g_task_type == 1) {
        drawText(SCR_W/2 - 150, 160, "HOLD  [L1]  to  Upload:", C_WHITE, 1);
        drawProgressBar(SCR_W/2 - 200, 220, 400, 40, g_hold_ticks * 100 / 180, C_BLUE);
        char pct[16]; sprintf(pct, "%d%%", g_hold_ticks * 100 / 180);
        drawText(SCR_W/2 - 15, 230, pct, C_WHITE, 1);
        drawText(SCR_W/2 - 120, 300, "Keep holding L1...", C_LGRAY, 1);
    }
    else {
        drawText(SCR_W/2 - 200, 160, "Press RIGHT to add fuel, LEFT to remove:", C_WHITE, 1);
        drawProgressBar(SCR_W/2 - 200, 220, 400, 40, g_fuel_val, C_ORANGE);
        char pct[16]; sprintf(pct, "%d%%", g_fuel_val);
        drawText(SCR_W/2 - 15, 230, pct, C_WHITE, 1);
        drawText(SCR_W/2 - 100, 300, "Fill to 100% to complete!", C_LGRAY, 1);
    }

    drawText(SCR_W/2 - 60, 460, "[O] = Cancel", C_MGRAY, 1);
    drawText(20, 20, PLAYER_NAMES[g_active_player], PLAYER_COLORS[g_active_player], 1);
}

/* ============================================================
 * STATE: MEETING
 * ============================================================ */
static void updateMeeting() {
    /* find current voter: first un-voted alive player */
    int voter = -1;
    for (int i = 0; i < g_num_players; i++) {
        if (g_players[i].alive && !g_players[i].voted) { voter = i; break; }
    }

    if (voter == -1) {
        /* all voted, tally */
        int votes[MAX_PLAYERS+1] = {0}; /* +1 for skip */
        for (int i = 0; i < g_num_players; i++) {
            if (!g_players[i].alive || !g_players[i].voted) continue;
            int v = g_players[i].vote_for;
            if (v < 0 || v >= g_num_players) v = g_num_players; /* skip */
            votes[v]++;
        }
        /* find max */
        int max_v = 0, max_idx = -1; bool tie = false;
        for (int i = 0; i <= g_num_players; i++) {
            if (votes[i] > max_v) { max_v = votes[i]; max_idx = i; tie = false; }
            else if (votes[i] == max_v && max_v > 0) tie = true;
        }
        if (tie || max_idx < 0 || max_idx == g_num_players) {
            /* no ejection */
            g_ejected = -1;
        } else {
            g_ejected = max_idx;
            g_ejected_was_imp = (g_players[max_idx].role == ROLE_IMPOSTOR);
            g_players[max_idx].alive = false;
        }
        g_vote_result_timer = 180;
        g_state = STATE_VOTE_RESULT;
        return;
    }

    /* count alive targets */
    int n_alive = 0;
    for (int i = 0; i < g_num_players; i++)
        if (g_players[i].alive && i != voter) n_alive++;
    int max_sel = n_alive; /* +1 for skip */

    if (btnPressed(PAD_CTRL_UP))   g_sel = ((g_sel - 1) + max_sel + 1) % (max_sel + 1);
    if (btnPressed(PAD_CTRL_DOWN)) g_sel = (g_sel + 1) % (max_sel + 1);

    if (btnPressed(PAD_CTRL_CROSS)) {
        /* map g_sel to actual player */
        if (g_sel == max_sel) {
            g_players[voter].vote_for = -1; /* skip */
        } else {
            int cnt = 0;
            for (int i = 0; i < g_num_players; i++) {
                if (!g_players[i].alive || i == voter) continue;
                if (cnt == g_sel) { g_players[voter].vote_for = i; break; }
                cnt++;
            }
        }
        g_players[voter].voted = true;
        g_sel = 0;
    }
}

static void drawMeeting() {
    fillRect(0, 0, (int)SCR_W, (int)SCR_H, C_BG);
    drawHeader(g_body_reported ? "  DEAD BODY REPORTED  " : "  EMERGENCY MEETING  ");

    if (g_meeting_caller >= 0) {
        char buf[64];
        sprintf(buf, "Called by: %s", PLAYER_NAMES[g_meeting_caller]);
        drawText(50, 60, buf, PLAYER_COLORS[g_meeting_caller], 1);
    }

    /* find current voter */
    int voter = -1;
    for (int i = 0; i < g_num_players; i++) {
        if (g_players[i].alive && !g_players[i].voted) { voter = i; break; }
    }

    if (voter == -1) {
        drawText(SCR_W/2 - 100, SCR_H/2, "Tallying votes...", C_WHITE, 2);
        return;
    }

    char buf[64];
    sprintf(buf, "VOTER: %s - choose who to eject (UP/DOWN + X)", PLAYER_NAMES[voter]);
    drawText(50, 85, buf, PLAYER_COLORS[voter], 1);

    /* voting targets */
    int sel_idx = 0, cnt = 0;
    for (int i = 0; i < g_num_players; i++) {
        if (!g_players[i].alive || i == voter) continue;
        bool sel = (cnt == g_sel);
        int ry = 120 + cnt * 44;
        if (sel) fillRect(120, ry, 700, 40, C_DGRAY);
        drawRect(120, ry, 700, 40, sel ? C_HLITE : C_MGRAY);
        drawFilledCircle(150, ry+20, 14, PLAYER_COLORS[i], false, false);
        drawText(175, ry+10, PLAYER_NAMES[i], PLAYER_COLORS[i], 1);

        /* show already voted */
        int vote_cnt = 0;
        for (int j = 0; j < g_num_players; j++)
            if (g_players[j].voted && g_players[j].vote_for == i) vote_cnt++;
        if (vote_cnt > 0) {
            sprintf(buf, "(%d votes)", vote_cnt);
            drawText(600, ry+10, buf, C_RED, 1);
        }
        cnt++;
    }
    /* skip option */
    int ry = 120 + cnt * 44;
    bool ssel = (g_sel == cnt);
    if (ssel) fillRect(120, ry, 700, 40, C_DGRAY);
    drawRect(120, ry, 700, 40, ssel ? C_HLITE : C_MGRAY);
    drawText(175, ry+10, "SKIP - No ejection", ssel ? C_WHITE : C_LGRAY, 1);

    drawText(50, SCR_H - 50, "Voters remaining:", C_LGRAY, 1);
    int rem = 0;
    for (int i = 0; i < g_num_players; i++) if (g_players[i].alive && !g_players[i].voted) rem++;
    sprintf(buf, "%d", rem);
    drawText(220, SCR_H - 50, buf, C_YELLOW, 1);
}

/* ============================================================
 * STATE: VOTE RESULT
 * ============================================================ */
static void updateVoteResult() {
    g_vote_result_timer--;
    if (g_vote_result_timer <= 0 || btnPressed(PAD_CTRL_CROSS)) {
        checkWin();
        if (g_state == STATE_VOTE_RESULT) {
            g_state = STATE_PLAY;
            /* advance to next alive player */
            int next = g_active_player;
            int cnt2 = 0;
            while (!g_players[next].alive && cnt2 < g_num_players) {
                next = (next+1) % g_num_players;
                cnt2++;
            }
            g_active_player = next;
        }
    }
}

static void drawVoteResult() {
    fillRect(0, 0, (int)SCR_W, (int)SCR_H, C_BG);
    drawHeader("  VOTE  RESULT  ");

    if (g_ejected < 0) {
        drawText(SCR_W/2 - 100, 200, "No one ejected!", C_LGRAY, 3);
        drawText(SCR_W/2 - 200, 320, "The votes were tied or skipped.", C_MGRAY, 1);
    } else {
        char buf[64];
        sprintf(buf, "%s was ejected!", PLAYER_NAMES[g_ejected]);
        drawText(SCR_W/2 - (int)(strlen(buf)*6), 180, buf, PLAYER_COLORS[g_ejected], 2);
        drawFilledCircle(SCR_W/2, 290, 50, PLAYER_COLORS[g_ejected]);
        /* X over */
        for (int d = -40; d <= 40; d++) {
            putpixel(SCR_W/2+d, 290+d, C_WHITE);
            putpixel(SCR_W/2+d, 290-d, C_WHITE);
        }
        if (g_ejected_was_imp) {
            drawText(SCR_W/2 - 180, 380, "That was The Impostor! CREW wins?", C_CREW, 1);
        } else {
            drawText(SCR_W/2 - 180, 380, "That was Innocent! IMPOSTOR lurks...", C_IMP, 1);
        }
    }
    drawText(SCR_W/2 - 100, 500, "Press [X] to continue", C_WHITE, 1);
}

/* ============================================================
 * STATE: WIN
 * ============================================================ */
static void updateWin() {
    if (btnPressed(PAD_CTRL_CROSS) || btnPressed(PAD_CTRL_CIRCLE)) {
        g_state = STATE_MENU;
    }
}
static void drawWin(bool crewWon) {
    fillRect(0, 0, (int)SCR_W, (int)SCR_H, crewWon ? 0xFF0A1E12 : 0xFF1E0A0A);
    drawHeader(crewWon ? "  CREWMATES  WIN!  " : "  IMPOSTORS  WIN!  ");

    if (crewWon) {
        drawText(SCR_W/2 - 200, 150, "The crew completed all tasks!", C_CREW, 2);
        for (int i = 0; i < g_num_players; i++) {
            if (g_players[i].role == ROLE_IMPOSTOR) {
                char buf[64];
                sprintf(buf, "The impostor was: %s", PLAYER_NAMES[i]);
                drawText(SCR_W/2 - 150, 240 + i*30, buf, C_IMP, 1);
            }
        }
    } else {
        drawText(SCR_W/2 - 250, 150, "The impostor outnumbered the crew!", C_IMP, 2);
        for (int i = 0; i < g_num_players; i++) {
            if (g_players[i].role == ROLE_IMPOSTOR) {
                char buf[64];
                sprintf(buf, "  The impostor: %s  - WINNER!", PLAYER_NAMES[i]);
                drawText(SCR_W/2 - 150, 240 + i*30, buf, C_IMP, 1);
            }
        }
    }

    /* show all players */
    for (int i = 0; i < g_num_players; i++) {
        drawFilledCircle(200 + i*120, 420, 30, PLAYER_COLORS[i], false, false);
        drawText(170 + i*120, 460, PLAYER_NAMES[i], PLAYER_COLORS[i], 1);
        drawText(170 + i*120, 475,
            g_players[i].role == ROLE_IMPOSTOR ? "IMP" : "CREW",
            g_players[i].role == ROLE_IMPOSTOR ? C_IMP : C_CREW, 1);
        if (!g_players[i].alive)
            drawText(170 + i*120, 490, "(dead)", C_DEAD, 1);
    }

    drawText(SCR_W/2 - 150, 560, "Press [X] to return to Menu", C_WHITE, 1);
}

/* ============================================================
 * MAIN
 * ============================================================ */
int main(void) {
    initScreen();
    ioPadInit(7);
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0, sysutil_cb, NULL);

    g_state = STATE_MENU;
    g_sel   = 0;

    while (g_running) {
        sysUtilCheckCallback();
        updateInput();

        /* Clear */
        fillRect(0, 0, (int)SCR_W, (int)SCR_H, C_BG);

        /* Update & Draw */
        switch (g_state) {
            case STATE_MENU:        updateMenu();        drawMenu();           break;
            case STATE_SETUP:       updateSetup();       drawSetup();          break;
            case STATE_REVEAL:      updateReveal();      drawReveal();         break;
            case STATE_PLAY:        updatePlay();        drawPlay();           break;
            case STATE_TASK:        updateTask();        drawTask();           break;
            case STATE_MEETING:     updateMeeting();     drawMeeting();        break;
            case STATE_VOTE_RESULT: updateVoteResult();  drawVoteResult();     break;
            case STATE_WIN_CREW:    updateWin();         drawWin(true);        break;
            case STATE_WIN_IMP:     updateWin();         drawWin(false);       break;
        }

        doFlip();
    }

    return 0;
}
