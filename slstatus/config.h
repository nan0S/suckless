/* See LICENSE file for copyright and license details. */

/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
// static const char unknown_str[] = "n/a";
static const char unknown_str[] = "...";

/* maximum output string length */
#define MAXLEN 2048

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

static const struct arg args[] = {
	/* function 	format	argument */
	{ ipv4, "  ^c#46CDD0^ETH %s ^c#FFFFFF^| ", eth_dev },
	{ wifi_essid, "^c#46A6D0^WIFI %3s ", wifi_dev },
	{ wifi_perc, "%2s%% ^c#FFFFFF^| ", wifi_dev },
	{ run_command, "^c#3599A0^VOL %3s%% ^c#FFFFFF^| ", "amixer sget Master | egrep -om1 \"\\[[[:digit:]]{1,3}\" | tr -d [" },
	{ cpu_perc, "^c#F4DFA0^CPU %2s%% ^c#FFFFFF^| ", NULL  },
	{ ram_perc, "^c#F4DFA0^RAM %2s%% ^c#FFFFFF^| ", NULL  },
	{ battery_perc, "^c#F39C7B^BAT %2s%%/", "BAT0" },
	{ battery_perc, "%2s%% ^c#FFFFFF^| ", "BAT1" },
	{ datetime, "^c#F04250^%15s  ", "%a %b %d %T" },
};
