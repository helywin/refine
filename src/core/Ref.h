//
// Created by jiang.wenqiang on 2018/8/15.
//

#ifndef REFINE_REF_H
#define REFINE_REF_H


class Ref {
public:
    struct Header {
        unsigned char magic[8];                 // 8 byte
        long crc32;                             // 4 byte
        char user_info[32];                     // 32 byte
        long long time;                         // 8 byte
        unsigned char file_type;                // 1 byte
        unsigned long seg_index[8];             // 32 byte
        unsigned char reserved[128];            // 128 byte
    };
private:
    Header _header;

public:
    Ref();

    void setFile();

    bool startWrite(const char *codec, bool is_append = false);

    void writeLine(const double *data) const;

    bool finishWrite();

    bool startRead(const char *codec);

    void readLine(const double *data) const;

    bool finishRead();

};


#endif //REFINE_REF_H
