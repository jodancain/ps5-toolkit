/* Restricted read-only maintenance registry reader.
 * Key identifiers/types from ps5-payload-dev/regdump/regmgr.h.
 * No credential fields, setters, network calls or persistent service. */
#include <stdio.h>
extern int sceRegMgrGetInt(long, int *);
extern int sceRegMgrGetStr(long, char *, int);
struct entry { const char *name; long key; int type; };
static const struct entry entries[] = {
    {"SYSTEM_language", 33685504L, 0},
    {"SYSTEM_UPDATE_auto_download", 41944320L, 0},
    {"SYSTEM_UPDATE_auto_install", 41946624L, 0},
    {"SYSTEM_UPDATE_auto_update", 41945088L, 0},
    {"SYSTEM_POWER_charge", 42074368L, 0},
    {"SYSTEM_POWER_remote", 42074880L, 0},
    {"SYSTEM_POWER_controller_off", 42075136L, 0},
    {"SYSTEM_POWER_auto_off_media", 42075392L, 0},
    {"SYSTEM_POWER_auto_off_other", 42075648L, 0},
    {"SYSTEM_POWER_auto_power_down", 42075904L, 0},
    {"SYSTEM_POWER_suspend_to_ram", 42076160L, 0},
    {"SYSTEM_POWER_charge_minutes", 42106880L, 0},
    {"NET_COMMON_device", 337182976L, 0},
    {"NET_COMMON_mtu", 337184768L, 0},
    {"NET_COMMON_current_profile", 337185536L, 0},
    {"NET_COMMON_inet_flag", 337186048L, 0},
    {"NET_COMMON_dns6_flag", 337186304L, 0},
    {"NET_COMMON_primary_dns6", 337186560L, 1},
    {"NET_IP_dns_flag", 336856064L, 0},
    {"NET_IP_primary_dns", 336856320L, 1},
    {"NET_IP_secondary_dns", 336856576L, 1},
    {"NET_IP_ip_config", 336856832L, 0},
    {"NET_WIFI_IP_dns_flag", 337534984L, 0},
    {"NET_WIFI_IP_primary_dns", 337534985L, 1},
    {"NET_WIFI_IP_secondary_dns", 337534986L, 1},
    {"NET_WIFI_IP_ip_config", 337534977L, 0},
    {"NET_WIFI_freq_band", 337511680L, 0},
    {"NET_APP_http_proxy_flag", 338166016L, 0},
    {"NET_WIFI_APP_http_proxy_flag", 337543169L, 0},
};
int main(void) {
    FILE *f = fopen("/data/maintenance-status.txt", "w");
    if (!f) return 1;
    for (unsigned i = 0; i < sizeof(entries)/sizeof(entries[0]); i++) {
        const struct entry *e = &entries[i];
        int value = -1, rc;
        char text[2048] = {0};
        if (e->type == 0) {
            rc = sceRegMgrGetInt(e->key, &value);
            fprintf(f, "%s rc=0x%x value=%d\n", e->name, (unsigned)rc, value);
        } else if (e->type == 1) {
            rc = sceRegMgrGetStr(e->key, text, sizeof(text));
            text[sizeof(text)-1] = 0;
            fprintf(f, "%s rc=0x%x value=%s\n", e->name, (unsigned)rc, rc ? "<unreadable>" : text);
        } else {
            fprintf(f, "%s skipped_unsupported_type=%d\n", e->name, e->type);
        }
    }
    int profile = 0;
    if (sceRegMgrGetInt(337185536L, &profile) == 0 && profile >= 1 && profile <= 10) {
        long base = (profile - 1) * 65536L;
        const long keys[] = {339494921L, 339494922L};
        const char *names[] = {"ACTIVE_PROFILE_primary_dns", "ACTIVE_PROFILE_secondary_dns"};
        for (int i = 0; i < 2; ++i) {
            char value[2048] = {0};
            int rc = sceRegMgrGetStr(base + keys[i], value, sizeof(value));
            value[sizeof(value)-1] = 0;
            fprintf(f, "%s rc=0x%x value=%s\n", names[i], (unsigned)rc, rc ? "<unreadable>" : value);
        }
        int flag = -1;
        int rc = sceRegMgrGetInt(base + 339494920L, &flag);
        fprintf(f, "ACTIVE_PROFILE_dns_flag rc=0x%x value=%d\n", (unsigned)rc, flag);
    }
    return fclose(f) ? 1 : 0;
}
