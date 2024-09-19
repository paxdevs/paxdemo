
#include "pax/sound.h"
#include "opusfile.h"

opus_int16 pcm[5760*16];
opus_int16 pcm_play[5760*16];
void audio_thread() {
    PAXSound snd;
    OggOpusFile *of = op_open_file("/data/app/MAINAPP/lib/audio.opus", 0);
    if (of == NULL) {
        return;
    }
    int ret;
    while (1) {
        ret = op_read(of, pcm, 5760*8, 0);
        if (ret < 0) {
            break;
        }
        for(int i = 0;i < ret;i++) {
            pcm[i] = pcm[i] / 4;
            pcm_play[i*2] = pcm[i];
            pcm_play[i*2+1] = pcm[i];
        }
        void* ptr = pcm_play;
        while (ret > 0) {
            int written = snd.playSound(ptr, ret * 4);
            ret -= written / 2;
            ptr = (void*)((uint8_t*)ptr + written);
        }
    }
    op_free(of);
}