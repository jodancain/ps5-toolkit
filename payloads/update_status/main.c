/* Read-only registry status. IDs: ps5-payload-dev/regdump/regmgr.h.
 * No registry writes, update checks, network access, or credentials. */
#include <stdio.h>
extern int sceRegMgrGetInt(long key, int *value);
int main(void) {
    const long keys[] = {41944320, 41946624, 41945088};
    const char *names[] = {"auto_download", "auto_install", "auto_update"};
    FILE *out = fopen("/data/update-status.txt", "w");
    if (!out) return 1;
    int failed = 0;
    for (int i = 0; i < 3; ++i) {
        int value = -1;
        int rc = sceRegMgrGetInt(keys[i], &value);
        fprintf(out, "%s key=0x%lx rc=0x%x value=%d\n", names[i], keys[i], (unsigned)rc, value);
        if (rc != 0) failed = 1;
    }
    if (fclose(out) != 0) return 1;
    return failed;
}
