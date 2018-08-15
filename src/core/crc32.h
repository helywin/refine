//
// Created by jiang.wenqiang on 2018/8/14.
//

#ifndef REFINE_CRC32_H
#define REFINE_CRC32_H

unsigned int crc32(unsigned char *buf, int len);

static unsigned int CRC32[256];
static bool init = false;

void init_table() {
    int i, j;
    unsigned int crc;
    for (i = 0; i < 256; ++i) {
        crc = i;
        for (j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
        CRC32[i] = crc;
    }
    init = true;
}

unsigned int crc32(unsigned char *buf, int len) {
    unsigned int ret = 0xFFFFFFFF;
    int i;
    if (!init) {
        init_table();
    }
    for (i = 0; i < len; ++i) {
        ret = CRC32[((ret & 0xFF) ^ buf[i])] ^ (ret >> 8);
    }
    ret = ~ret;
    return ret;
}

#endif //REFINE_CRC32_H
