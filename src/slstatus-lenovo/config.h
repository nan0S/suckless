/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
// static const char unknown_str[] = "n/a";
static const char unknown_str[] = "..";

/* maximum output string length */
#define MAXLEN 2048
/* maximum output string of single command/function */
#define COMMAND_MAXLEN 128
/* maximum number of failures to print output */
#define MAX_FAILURES 10

/*
 * function            description                     argument (example)
 *
 * battery_perc        battery percentage              battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_state       battery charging state          battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * battery_remaining   battery remaining HH:MM         battery name (BAT0)
 *                                                     NULL on OpenBSD/FreeBSD
 * cpu_perc            cpu usage in percent            NULL
 * cpu_freq            cpu frequency in MHz            NULL
 * datetime            date and time                   format string (%F %T)
 * disk_free           free disk space in GB           mountpoint path (/)
 * disk_perc           disk usage in percent           mountpoint path (/)
 * disk_total          total disk space in GB          mountpoint path (/")
 * disk_used           used disk space in GB           mountpoint path (/)
 * entropy             available entropy               NULL
 * gid                 GID of current user             NULL
 * hostname            hostname                        NULL
 * ipv4                IPv4 address                    interface name (eth0)
 * ipv6                IPv6 address                    interface name (eth0)
 * kernel_release      `uname -r`                      NULL
 * keyboard_indicators caps/num lock indicators        format string (c?n?)
 *                                                     see keyboard_indicators.c
 * keymap              layout (variant) of current     NULL
 *                     keymap
 * load_avg            load average                    NULL
 * netspeed_rx         receive network speed           interface name (wlan0)
 * netspeed_tx         transfer network speed          interface name (wlan0)
 * num_files           number of files in a directory  path
 *                                                     (/home/foo/Inbox/cur)
 * ram_free            free memory in GB               NULL
 * ram_perc            memory usage in percent         NULL
 * ram_total           total memory size in GB         NULL
 * ram_used            used memory in GB               NULL
 * run_command         custom shell command            command (echo foo)
 * swap_free           free swap in GB                 NULL
 * swap_perc           swap usage in percent           NULL
 * swap_total          total swap size in GB           NULL
 * swap_used           used swap in GB                 NULL
 * temp                temperature in degree celsius   sensor file
 *                                                     (/sys/class/thermal/...)
 *                                                     NULL on OpenBSD
 *                                                     thermal zone on FreeBSD
 *                                                     (tz0, tz1, etc.)
 * uid                 UID of current user             NULL
 * uptime              system uptime                   NULL
 * username            username of current user        NULL
 * vol_perc            OSS/ALSA volume in percent      mixer file (/dev/mixer)
 * wifi_perc           WiFi signal in percent          interface name (wlan0)
 * wifi_essid          WiFi ESSID                      interface name (wlan0)
 */

static const char eth_dev[] = "enp0s25";
static const char wifi_dev[] = "wlp3s0";

#define  RESET       "^c#FFFFFF^"
#define  VIOLET      "^c#7A62DE^"
#define  LIGHT_BLUE  "^c#46CDD0^"
#define  BLUE        "^c#46A6D0^"
#define  GREEN       "^c#3599A0^"
#define  YELLOW      "^c#F4DFA0^"
#define  ORANGE      "^c#F39C7B^"
#define  RED         "^c#F04250^"
#define  PREF        "| "
#define  SUFF        " " RESET
#define  S           *1000
#define  MS          *1

static struct arg args[] = {
  /* function        format                          argument                interval */
   { run_command,    " " VIOLET "Mail: %s" SUFF,     "new-mails",            1 S },
   { ipv4,           PREF LIGHT_BLUE "ETH %s" SUFF,  eth_dev,                1 S },
   { wifi_essid,     PREF BLUE "WIFI %s" SUFF,       wifi_dev,               1 S },
   { wifi_perc,      BLUE "%3s%%" SUFF,              wifi_dev,               1 S },
   { run_command,    PREF GREEN "%s ",               "get-speaker-name -s",  1 S },
   { run_command,    PREF GREEN "VOL %s%%" SUFF,     "volctl --get-volume",  0 },
   { run_command,    PREF GREEN "%s" SUFF,           "muctl time",           100 MS },
   { cpu_perc,       PREF YELLOW "CPU %2s%%" SUFF,   NULL,                   1 S },
   { ram_perc,       PREF YELLOW "RAM %2s%%" SUFF,   NULL,                   1 S },
   { battery_state,  PREF ORANGE "BAT %s",           "BAT0",                 15 S },
   { battery_perc,   "%s%%/",                        "BAT0",                 15 S },
   { battery_state,  "%s",                           "BAT1",                 15 S },
   { battery_perc,   "%s%%" SUFF,                    "BAT1",                 15 S },
   { datetime,       PREF RED "%s" SUFF,             "%b %d (%a) %T",        100 MS},
};
