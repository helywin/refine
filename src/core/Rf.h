//
// Created by jiang.wenqiang on 2018/8/15.
//

#ifndef REFINE_RF_H
#define REFINE_RF_H


class Rf {
public:
    struct Header {
        unsigned char magic[8];
        long crc32;
        unsigned char user_len;
        char *user_info;
        long long time;
        unsigned char file_type;
        unsigned char seg_index[8];
        unsigned char reserved[128];
    };
private:
    Header _header;
};


#endif //REFINE_RF_H
