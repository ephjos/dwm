/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10", "JoyPixels:pixelsize=10:antialias=true:autohint=true"  };
static char dmenufont[]             = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#770000";
static char selbgcolor[]            = "#0B0c10";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {"st", "-n", "sppython", "-g", "120x34", "-e", "python3", "-i", NULL };
const char *spcmd3[] = {"st", "-n", "splf", "-g", "120x34", "-e", "lf", NULL };
static Sp scratchpads[] = {
  /* name          cmd  */
  {"spterm",      spcmd1},
  {"sppython",    spcmd2},
  {"splf",    spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
  */
  /* class    instance      title          tags mask    isfloating   isterminal  noswallow  monitor */
  { "Gimp",     NULL,       NULL,             1 << 8,       0,           0,         0,        -1 },
  { "zoom",     NULL,       NULL,             1 << 8,       0,           0,         0,        -1 },
  { "St",       NULL,       NULL,             0,            0,           1,         0,        -1 },
  { NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
  { NULL,      "spterm",    NULL,             SPTAG(0),     1,           1,         0,        -1 },
  { NULL,      "sppython",  NULL,             SPTAG(1),     1,           1,         0,        -1 },
  { NULL,      "splf",      NULL,             SPTAG(2),     1,           1,         0,        -1 },
  { NULL,       NULL,       "cfx",            0,            1,           0,         1,        -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",  tile },     /* Default: Master on left, slaves on right */
  { "TTT",  bstack },   /* Master on top, slaves on bottom */

  { "[@]",  spiral },   /* Fibonacci spiral */
  { "[\\]", dwindle },    /* Decreasing in size right and leftward */

  { "H[]",  deck },     /* Master on left, slaves in monocle-like mode on right */
  { "[M]",  monocle },    /* All windows on top of eachother */

  { "|M|",  centeredmaster },   /* Master in middle, slaves on sides */
  { ">M>",  centeredfloatingmaster }, /* Same but master floats */

  { "><>",  NULL },     /* no layout function means floating behavior */
  { NULL,   NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
  { MOD,  XK_j, ACTION##stack,  {.i = INC(+1) } }, \
  { MOD,  XK_k, ACTION##stack,  {.i = INC(-1) } }, \
  { MOD,  XK_v,   ACTION##stack,  {.i = 0 } }, \
  /* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
  /* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
  /* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
  /* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* Commands bound to more than one key */
#define VOLUME_MUTE "pamixer -t; kill -44 $(pidof dwmblocks)"
#define VOLUME_UP "pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)"
#define VOLUME_DOWN "pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)"
#define BRIGHTNESS_UP "sudo light -A 3"
#define BRIGHTNESS_DOWN "sudo light -U 3"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-show", "run", NULL};
static const char *termcmd[]  = { "st", NULL };

#include <X11/XF86keysym.h>
#include "shiftview.c"
static Key keys[] = {
  /* modifier                     key        function        argument */
  STACKKEYS(MODKEY,                          focus)
  STACKKEYS(MODKEY|ShiftMask,                push)
  /* { MODKEY|ShiftMask,    XK_Escape,  spawn,  SHCMD("") }, */
  { MODKEY,     XK_grave, spawn,  SHCMD("sd rofi emoji") },
  /* { MODKEY|ShiftMask,    XK_grave, togglescratch,  SHCMD("") }, */
  TAGKEYS(      XK_1,   0)
  TAGKEYS(      XK_2,   1)
  TAGKEYS(      XK_3,   2)
  TAGKEYS(      XK_4,   3)
  TAGKEYS(      XK_5,   4)
  TAGKEYS(      XK_6,   5)
  TAGKEYS(      XK_7,   6)
  TAGKEYS(      XK_8,   7)
  TAGKEYS(      XK_9,   8)
  { MODKEY,     XK_0,   view,   {.ui = ~0 } },
  { MODKEY|ShiftMask,   XK_0,   tag,    {.ui = ~0 } },
  { MODKEY,     XK_minus, spawn,    SHCMD(VOLUME_DOWN) },
  { MODKEY,     XK_equal, spawn,    SHCMD(VOLUME_UP) },
  { MODKEY|ShiftMask,   XK_minus, spawn,    SHCMD(BRIGHTNESS_DOWN) },
  { MODKEY|ShiftMask,   XK_equal, spawn,    SHCMD(BRIGHTNESS_UP) },
  //{ MODKEY,     XK_BackSpace, spawn,    SHCMD("sysact") },
  //{ MODKEY|ShiftMask,   XK_BackSpace, spawn,    SHCMD("sysact") },

  { MODKEY,     XK_Tab,   view,   {0} },
  { MODKEY,     XK_q,   killclient, {0} },
  //{ MODKEY|ShiftMask,   XK_q,   spawn,    SHCMD("sysact") },
  { MODKEY,     XK_w,   spawn,    SHCMD("$BROWSER") },
  { MODKEY|ShiftMask,   XK_w,   spawn,    SHCMD("st -e sudo nmtui") },
  //{ MODKEY,     XK_e,   spawn,    SHCMD("st -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook") },
  //{ MODKEY|ShiftMask,   XK_e,   spawn,    SHCMD("st -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") },
  { MODKEY,     XK_r,   spawn,    SHCMD("st -e lf") },
  { MODKEY|ShiftMask,   XK_r,   spawn,    SHCMD("st -e htop") },
  { MODKEY,     XK_t,   setlayout,  {.v = &layouts[0]} }, /* tile */
  { MODKEY|ShiftMask,   XK_t,   setlayout,  {.v = &layouts[1]} }, /* bstack */
  { MODKEY,     XK_y,   setlayout,  {.v = &layouts[2]} }, /* spiral */
  { MODKEY|ShiftMask,   XK_y,   setlayout,  {.v = &layouts[3]} }, /* dwindle */
  { MODKEY,     XK_u,   setlayout,  {.v = &layouts[4]} }, /* deck */
  { MODKEY|ShiftMask,   XK_u,   setlayout,  {.v = &layouts[5]} }, /* monocle */
  { MODKEY,     XK_i,   setlayout,  {.v = &layouts[6]} }, /* centeredmaster */
  { MODKEY|ShiftMask,   XK_i,   setlayout,  {.v = &layouts[7]} }, /* centeredfloatingmaster */
  { MODKEY,     XK_o,   incnmaster,     {.i = +1 } },
  { MODKEY|ShiftMask,   XK_o,   incnmaster,     {.i = -1 } },
  //{ MODKEY,     XK_p,     spawn,    SHCMD("mpc toggle") },
  //{ MODKEY|ShiftMask,   XK_p,     spawn,    SHCMD("mpc pause ; pauseallmpv") },
  //{ MODKEY,     XK_bracketleft,   spawn,    SHCMD("mpc seek -10") },
  //{ MODKEY|ShiftMask,   XK_bracketleft,   spawn,    SHCMD("mpc seek -60") },
  //{ MODKEY,     XK_bracketright,  spawn,    SHCMD("mpc seek +10") },
  //{ MODKEY|ShiftMask,   XK_bracketright,  spawn,    SHCMD("mpc seek +60") },
  { MODKEY,     XK_backslash,   view,   {0} },

  { MODKEY,     XK_a,   togglegaps, {0} },
  { MODKEY|ShiftMask,   XK_a,   defaultgaps,  {0} },
  { MODKEY,     XK_s,   togglesticky, {0} },
  { MODKEY,     XK_d,   spawn,          {.v = dmenucmd } },
  { MODKEY|ShiftMask,     XK_d,   spawn,          SHCMD("rofi -show ssh") },
  { MODKEY,     XK_f,   togglefullscr,  {0} },
  { MODKEY|ShiftMask,   XK_f,   setlayout,  {.v = &layouts[8]} },
  { MODKEY,     XK_g,   shiftview,  { .i = -1 } },
  { MODKEY|ShiftMask,   XK_g,   shifttag, { .i = -1 } },
  { MODKEY,     XK_h,   setmfact, {.f = -0.05} },
  /* J and K are automatically bound above in STACKEYS */
  { MODKEY,     XK_l,   setmfact,       {.f = +0.05} },
  { MODKEY,     XK_semicolon, shiftview,  { .i = 1 } },
  { MODKEY|ShiftMask,   XK_semicolon, shifttag, { .i = 1 } },
  { MODKEY,     XK_Return,  spawn,    {.v = termcmd } },
  { MODKEY|ShiftMask,   XK_Return,  togglescratch,  {.ui = 0} },
  { MODKEY,     XK_apostrophe,  togglescratch,  {.ui = 1} },
  { MODKEY|ShiftMask,     XK_apostrophe,  togglescratch,  {.ui = 2} },

  { MODKEY,     XK_z,   incrgaps, {.i = +3 } },
  { MODKEY|ShiftMask,     XK_z,   incrgaps, {.i = -3 } },
  { MODKEY,     XK_b,   togglebar,  {0} },
  { MODKEY|ShiftMask,   XK_m,   spawn,    SHCMD(VOLUME_MUTE) },
  //{ MODKEY,     XK_comma, spawn,    SHCMD("mpc prev") },
  //{ MODKEY|ShiftMask,   XK_comma, spawn,    SHCMD("mpc seek 0%") },
  //{ MODKEY,     XK_period,  spawn,    SHCMD("mpc next") },
  //{ MODKEY|ShiftMask,   XK_period,  spawn,    SHCMD("mpc repeat") },

  { MODKEY,     XK_Left,  focusmon, {.i = -1 } },
  { MODKEY|ShiftMask,   XK_Left,  tagmon,   {.i = -1 } },
  { MODKEY,     XK_Right, focusmon, {.i = +1 } },
  { MODKEY|ShiftMask,   XK_Right, tagmon,   {.i = +1 } },

  { MODKEY,     XK_Page_Up, shiftview,  { .i = -1 } },
  { MODKEY|ShiftMask,   XK_Page_Up, shifttag, { .i = -1 } },
  { MODKEY,     XK_Page_Down, shiftview,  { .i = +1 } },
  { MODKEY|ShiftMask,   XK_Page_Down, shifttag, { .i = +1 } },
  { MODKEY,     XK_Insert,  spawn,    SHCMD("notify-send \"📋 Clipboard contents:\" \"$(xclip -o -selection clipboard)\"") },

  { MODKEY,     XK_F1,    spawn,    SHCMD("sd record start") },
  { MODKEY,     XK_F2,    spawn,    SHCMD("sd record stop") },
  { MODKEY,     XK_F3,    spawn,    SHCMD("st -e newsboat") },
  { MODKEY,     XK_F4,    spawn,    SHCMD("st -e pulsemixer; kill -44 $(pidof dwmblocks)") },
  //{ MODKEY,     XK_F5,    xrdb,   {.v = NULL } },
  //{ MODKEY,     XK_F6,    spawn,    SHCMD("torwrap") },
  //{ MODKEY,     XK_F7,    spawn,    SHCMD("td-toggle") },
  //{ MODKEY,     XK_F8,    spawn,    SHCMD("mailsync") },
  { MODKEY,     XK_F9,    spawn,    SHCMD("sd rofi mount") },
  { MODKEY,     XK_F10,   spawn,    SHCMD("sd rofi unmount") },
  { MODKEY,     XK_F11,   spawn,    SHCMD("mpv --demuxer-lavf-format=video4linux2 --demuxer-lavf-o-set=input_format=mjpeg av://v4l2:/dev/video0") },
  //{ MODKEY,     XK_F12,   spawn,    SHCMD("notify") },
  { MODKEY,     XK_space, zoom,   {0} },
  { MODKEY|ShiftMask,   XK_space, togglefloating, {0} },

  { 0,      XK_Print, spawn,    SHCMD("sd rofi maim") },

  { MODKEY, XK_x, spawn,    SHCMD("slock") },
  { MODKEY|ShiftMask, XK_x, spawn,    SHCMD("sd rofi power") },
  //{ MODKEY, XK_Escape,  spawn,    SHCMD("") },
  { MODKEY|ShiftMask, XK_Escape,  spawn,    SHCMD("killall dwm") },

  { 0, XF86XK_AudioMute,    spawn,    SHCMD(VOLUME_MUTE) },
  { 0, XF86XK_AudioRaiseVolume, spawn,    SHCMD(VOLUME_UP) },
  { 0, XF86XK_AudioLowerVolume, spawn,    SHCMD(VOLUME_DOWN) },
  //{ 0, XF86XK_AudioPrev,    spawn,    SHCMD("mpc prev") },
  //{ 0, XF86XK_AudioNext,    spawn,    SHCMD("mpc next") },
  //{ 0, XF86XK_AudioPause,   spawn,    SHCMD("mpc pause") },
  //{ 0, XF86XK_AudioPlay,    spawn,    SHCMD("mpc play") },
  //{ 0, XF86XK_AudioStop,    spawn,    SHCMD("mpc stop") },
  //{ 0, XF86XK_AudioRewind,  spawn,    SHCMD("mpc seek -10") },
  //{ 0, XF86XK_AudioForward, spawn,    SHCMD("mpc seek +10") },
  //{ 0, XF86XK_AudioMedia,   spawn,    SHCMD("st -e ncmpcpp") },
  //{ 0, XF86XK_PowerOff,   spawn,    SHCMD("sysact") },
  { 0, XF86XK_Calculator,   spawn,    SHCMD("st -e bc -l") },
  //{ 0, XF86XK_Sleep,    spawn,    SHCMD("sudo -A zzz") },
  { 0, XF86XK_WWW,    spawn,    SHCMD("$BROWSER") },
  { 0, XF86XK_DOS,    spawn,    SHCMD("st") },
  { 0, XF86XK_ScreenSaver,  spawn,    SHCMD("slock") },
  { 0, XF86XK_TaskPane,   spawn,    SHCMD("st -e htop") },
  { 0, XF86XK_Mail,   spawn,    SHCMD("st -e neomutt ; pkill -RTMIN+12 dwmblocks") },
  { 0, XF86XK_MyComputer,   spawn,    SHCMD("st -e lf /") },
  /* { 0, XF86XK_Battery,   spawn,    SHCMD("") }, */
  { 0, XF86XK_Launch1,    spawn,    SHCMD("xset dpms force off") },
  { 0, XF86XK_TouchpadToggle, spawn,    SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
  { 0, XF86XK_TouchpadOff,  spawn,    SHCMD("synclient TouchpadOff=1") },
  { 0, XF86XK_TouchpadOn,   spawn,    SHCMD("synclient TouchpadOff=0") },
  { 0, XF86XK_MonBrightnessUp,  spawn,    SHCMD(BRIGHTNESS_UP) },
  { 0, XF86XK_MonBrightnessDown,  spawn,    SHCMD(BRIGHTNESS_DOWN) },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
};
