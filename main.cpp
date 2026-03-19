#include <ppu-lv2.h>
#include <sysutil/sysutil.h>
#include <io/pad.h>
#include <stdio.h>

int main() {
    ioPadInit(7);

    while (1) {
        ioPadInfo2 padinfo;
        ioPadGetInfo2(&padinfo);

        for (int i = 0; i < MAX_PADS; i++) {
            if (padinfo.port_status[i]) {
                ioPadData paddata;
                ioPadGetData(i, &paddata);

                if (paddata.BTN_CROSS) {
                    printf("Pressed X\n");
                }
            }
        }

        sysUtilCheckCallback();
    }

    return 0;
}
