#include <psxetc.h>
#include <psxgpu.h>
#include <psxpad.h>
#include <psxapi.h>
#include <psxgte.h>
#include <stdint.h>

#define SCREEN_W 320
#define SCREEN_H 240
#define OT_LEN   1024
#define PRIM_BUF 32768

static DISPENV disp[2];
static DRAWENV draw[2];
static uint32_t ot[2][OT_LEN];
static char     primbuf[2][PRIM_BUF];
static char    *nextpri;
static uint8_t  padbuf[2][34];

static void *newprim(int size) {
    void *p = nextpri;
    nextpri += size;
    return p;
}

static void rect(int db, int x, int y, int w, int h, uint8_t r, uint8_t g, uint8_t b) {
    TILE *t = (TILE *)newprim(sizeof(TILE));
    setTile(t);
    setXY0(t, x, y);
    setWH(t, w, h);
    setRGB0(t, r, g, b);
    addPrim(&ot[db][0], t);
}

int main(void) {
    int db = 0;
    int ux = 60, uy = 100;

    ResetGraph(0);

    SetDefDispEnv(&disp[0], 0, 0, SCREEN_W, SCREEN_H);
    SetDefDispEnv(&disp[1], 0, SCREEN_H, SCREEN_W, SCREEN_H);
    SetDefDrawEnv(&draw[0], 0, SCREEN_H, SCREEN_W, SCREEN_H);
    SetDefDrawEnv(&draw[1], 0, 0, SCREEN_W, SCREEN_H);

    setRGB0(&draw[0], 90, 180, 255);
    setRGB0(&draw[1], 90, 180, 255);
    draw[0].isbg = 1;
    draw[1].isbg = 1;

    InitPAD(padbuf[0], 34, padbuf[1], 34);
    StartPAD();
    ChangeClearPAD(0);

    SetDispMask(1);

    while (1) {
        uint32_t buttons = PadRead(1);

        if (buttons & PAD_UP)    uy -= 2;
        if (buttons & PAD_DOWN)  uy += 2;
        if (buttons & PAD_LEFT)  ux -= 2;
        if (buttons & PAD_RIGHT) ux += 2;

        if (ux < 0) ux = 0;
        if (ux > SCREEN_W - 60) ux = SCREEN_W - 60;
        if (uy < 0) uy = 0;
        if (uy > SCREEN_H - 40) uy = SCREEN_H - 40;

        ClearOTagR(ot[db], OT_LEN);
        nextpri = primbuf[db];

        rect(db, 0,  10, SCREEN_W, 6, 255,  60,  60);
        rect(db, 0,  16, SCREEN_W, 6, 255, 160,  30);
        rect(db, 0,  22, SCREEN_W, 6, 255, 230,  60);
        rect(db, 0,  28, SCREEN_W, 6,  60, 255, 120);
        rect(db, 0,  34, SCREEN_W, 6,  60, 160, 255);
        rect(db, 0,  40, SCREEN_W, 6, 160,  80, 255);

        rect(db, ux,      uy + 12, 40, 16, 245, 245, 245);
        rect(db, ux + 36, uy,      14, 14, 245, 245, 245);
        rect(db, ux + 46, uy - 8,   3,  8, 255, 215,   0);
        rect(db, ux + 30, uy + 2,   8,  6, 255,  80, 200);

        DrawSync(0);
        VSync(0);

        PutDispEnv(&disp[db]);
        PutDrawEnv(&draw[db]);
        DrawOTag(&ot[db][OT_LEN - 1]);

        db = !db;
    }

    return 0;
}
