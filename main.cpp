
#include "pax/fb.h"
#include "pax/keypad.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <utility>

__attribute__((constructor))
int app_start()
{
    PAXFramebuffer fb;
    PAXKeypad kp;

    int videofd = open("/data/app/MAINAPP/lib/video.frames", O_RDONLY);
    if (videofd == -1)
    {
        return 0;
    }
    // mmap the video file
    struct stat st;
    fstat(videofd, &st);
    uint8_t *video = (uint8_t*)mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, videofd, 0);
    if (video == (uint8_t*)-1)
    {
        return 0;
    }

    // px per frame is 240*180
    int ptr = 0;
    int cur_pixels = 0;
    struct timeval start_ts;
    gettimeofday(&start_ts, 0);
    uint64_t target_us = 1000000 / 60;
    while(ptr < st.st_size && kp.getKey() != 223) {
        uint8_t cur_pixel_group = video[ptr];
        uint8_t cur_pixel = cur_pixel_group >> 7;
        uint8_t cur_pixel_cnt = cur_pixel_group & 0x7F;
        for(int i = 0;i < cur_pixel_cnt;i++) {
            int x = cur_pixels % 240;
            int y = cur_pixels / 240;
            fb.drawPixel(x, 320-70-y, cur_pixel * 0xFF, cur_pixel * 0xFF, cur_pixel * 0xFF);
            cur_pixels++;
        }
        ptr++;
        if (cur_pixels == 240*180) {
            struct timeval end_ts;
            gettimeofday(&end_ts, 0);
            uint64_t elapsed_us = (uint64_t)(end_ts.tv_sec - start_ts.tv_sec) * 1000000 + (end_ts.tv_usec - start_ts.tv_usec);
            if (elapsed_us < target_us) {
                struct timespec ts;
                ts.tv_sec = 0;
                ts.tv_nsec = (target_us - elapsed_us) * 1000;
                nanosleep(&ts, 0);
            }
            gettimeofday(&start_ts, 0);
            cur_pixels = 0;
        }
    }
    return 0;
}
