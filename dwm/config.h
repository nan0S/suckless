/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 15;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "Source Code Pro-Bold:size=10" };
static const char dmenufont[]       = "Source Code Pro-Bold:size=10";
static const char normbgcolor[]     = "#222222";
static const char normbordercolor[] = "#444444";
static const char normfgcolor[]     = "#bbbbbb";
static const char selfgcolor[]      = "#eeeeee";
static const char selbgcolor[]      = "#005577";
static const char selbordercolor[]  = "#770000";
static const char *colors[][4]      = {
    /*               fg           bg           border           float*/
    [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor, selbgcolor },
    [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor,  selbgcolor },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     */
    /* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
    { "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
    { "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
    { "St",      NULL,     NULL,           0,         0,          1,           0,        -1 },
    { NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
    { "[D]",      deck },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* my terminal */
#define TERMINAL "st"
#define BROWSER "brave"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char *browsercmd[] = { BROWSER, NULL };
/* volumne controls */
static const char *voldowncmd[] = { "volctl", "--decrease", NULL };
static const char *volupcmd[] = { "volctl", "--increase", NULL };
static const char *voltogglecmd[] = { "volctl", "--toggle", NULL };
/* brightness controls */
static const char *brightnessdowncmd[] = { "light", "-U", "10", NULL };
static const char *brightnessupcmd[] = { "light", "-A", "10", NULL };
/* trackpoint and touchpad */ 
static const char *touchpadcmd[] = { "touchpad-toggle", NULL };
static const char *trackpointcmd[] = { "trackpoint-toggle", NULL };
/* custom program bindings */
static const char *xkillcmd[] = { "xkill", NULL };
static const char *bluetoothcmd[] = { "blueman-manager", NULL };
static const char *discordcmd[] = { "discord", NULL };
static const char *pavucontrolcmd[] = { "pavucontrol", NULL };
static const char *screenshotcmd[] = { "deepin-screenshot", NULL };
static const char *terminatecmd[] = { "shutdown", "now", NULL };
static const char *restartcmd[] = { "shutdown", "-r", "now", NULL };
static const char *msteamscmd[] = { "teams", NULL };
static const char *fbcmd[] = { BROWSER , "facebook.com", NULL };
/* scratchpad */
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

#include <X11/XF86keysym.h>

static Key keys[] = {
    /* modifier                     key             function        argument */
    { MODKEY,                       XK_p,           spawn,          {.v = dmenucmd } },
    { MODKEY|ShiftMask,             XK_Return,      spawn,          {.v = termcmd } },
    { MODKEY|ShiftMask,             XK_c,           spawn,          {.v = browsercmd } },
    { MODKEY|ShiftMask,             XK_b,           spawn,          {.v = bluetoothcmd } },
    { MODKEY|ShiftMask,             XK_d,           spawn,          {.v = discordcmd } },
    { MODKEY|ShiftMask,             XK_p,           spawn,          {.v = pavucontrolcmd } },
    { MODKEY|ShiftMask,             XK_t,           spawn,          {.v = terminatecmd } },
    { MODKEY|ShiftMask,             XK_r,           spawn,          {.v = restartcmd } },
    { MODKEY|ShiftMask,             XK_v,           spawn,          {.v = msteamscmd } },
    { MODKEY|ShiftMask,             XK_g,           spawn,          {.v = fbcmd } },
    { MODKEY,                       XK_x,           spawn,          {.v = xkillcmd } },
    { MODKEY|ShiftMask,             XK_m,           spawn,          SHCMD(TERMINAL " -e ncmpcpp") },
    { MODKEY|ShiftMask,             XK_e,           spawn,          SHCMD(TERMINAL " -e neomutt") },
    { MODKEY|ShiftMask,             XK_n,           spawn,          SHCMD(TERMINAL " -e bc -lq") },
    { MODKEY|ShiftMask,             XK_u,           spawn,          SHCMD(TERMINAL " -e top") },
    { MODKEY|ShiftMask,             XK_s,           spawn,          SHCMD(TERMINAL " -e pgrep spotifyd || spotifyd; " TERMINAL " -e spt") },
    { 0,                            XK_Print,       spawn,          {.v = screenshotcmd } },
    { MODKEY,                       XK_grave,       togglescratch,  {.v = scratchpadcmd } },
    { MODKEY,                       XK_b,           togglebar,      {0} },
    { MODKEY,                       XK_j,           focusstackvis,  {.i = +1 } },
    { MODKEY,                       XK_k,           focusstackvis,  {.i = -1 } },
    { MODKEY|ShiftMask,             XK_j,           focusstackhid,  {.i = +1 } },
    { MODKEY|ShiftMask,             XK_k,           focusstackhid,  {.i = -1 } },
    { MODKEY,                       XK_s,           swapfocus,      {.i = -1 } },
    { MODKEY,                       XK_i,           incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_d,           incnmaster,     {.i = -1 } },
    { MODKEY,                       XK_h,           setmfact,       {.f = -0.05} },
    { MODKEY,                       XK_l,           setmfact,       {.f = +0.05} },
    { MODKEY,                       XK_Return,      zoom,           {0} },
    { MODKEY,                       XK_Tab,         view,           {0} },
    { MODKEY,                       XK_q,           killclient,     {0} },
    { MODKEY,                       XK_t,           setlayout,      {.v = &layouts[0]} },
    { MODKEY|ShiftMask,             XK_f,           setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_m,           setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_c,           setlayout,      {.v = &layouts[3]} },
    { MODKEY,                       XK_f,           fullscreen,     {0} },
    { MODKEY,                       XK_space,       setlayout,      {0} },
    { MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },
    { MODKEY,                       XK_0,           view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,           tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,       focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period,      focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,       tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period,      tagmon,         {.i = +1 } },
    { MODKEY|ShiftMask,             XK_s,           show,           {0} },
    { MODKEY|ShiftMask,             XK_h,           hide,           {0} },
    { MODKEY,                       XK_minus,       setgaps,        {.i = -1 } },
    { MODKEY,                       XK_equal,       setgaps,        {.i = +1 } },
    { MODKEY|ShiftMask,             XK_equal,       setgaps,        {.i = 0  } },
    { MODKEY,                       XK_v,           focusmaster,    {0} },
    TAGKEYS(                        XK_1,                           0)
    TAGKEYS(                        XK_2,                           1)
    TAGKEYS(                        XK_3,                           2)
    TAGKEYS(                        XK_4,                           3)
    TAGKEYS(                        XK_5,                           4)
    TAGKEYS(                        XK_6,                           5)
    TAGKEYS(                        XK_7,                           6)
    TAGKEYS(                        XK_8,                           7)
    TAGKEYS(                        XK_9,                           8)
    { MODKEY|ShiftMask,             XK_q,           quit,           {0} },
    /* music player */
    { MODKEY,                       XK_comma,       spawn,          SHCMD("muctl prev") },
    { MODKEY,                       XK_period,      spawn,          SHCMD("muctl next") },
    { MODKEY,                       XK_slash,       spawn,          SHCMD("muctl toggle") },
    { MODKEY|ShiftMask,             XK_comma,       spawn,          SHCMD("muctl seek -5") },
    { MODKEY|ShiftMask,             XK_period,      spawn,          SHCMD("muctl seek +5") },
    { MODKEY|ShiftMask,             XK_semicolon,   spawn,          SHCMD("muctl seek 0%") },
    { MODKEY|ShiftMask,             XK_slash,       spawn,          SHCMD("muctl stop") },
    { MODKEY,                       XK_bracketleft, spawn,          SHCMD("muctl current") },
    { MODKEY,                       XK_bracketright,spawn,          SHCMD("muctl queued") },
    { MODKEY,                       XK_semicolon,   spawn,          SHCMD("muctl repeat") },
    { MODKEY,                       XK_apostrophe,  spawn,          SHCMD("muctl random") },
    { MODKEY,                       XK_backslash,   spawn,          SHCMD("muctl shuffle") },
    /* volume control */
    { 0,          XF86XK_AudioRaiseVolume,          spawn,          {.v  =  volupcmd           }  },
    { 0,          XF86XK_AudioMute,                 spawn,          {.v  =  voltogglecmd       }  },
    { 0,          XF86XK_AudioLowerVolume,          spawn,          {.v  =  voldowncmd         }  },
    /*  touchpad and trackpoint */
    { 0,          XF86XK_AudioMicMute,              spawn,          {.v  =  touchpadcmd        }  },
    { 0,          XF86XK_Tools,                     spawn,          {.v  =  trackpointcmd      }  },
    /*  brightness control */
    { 0,          XF86XK_MonBrightnessDown,         spawn,          {.v  =  brightnessdowncmd  }  },
    { 0,          XF86XK_MonBrightnessUp,           spawn,          {.v  =  brightnessupcmd    }  },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

