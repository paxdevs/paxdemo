
#include "pax/sound.h"
#include "opusfile.h"

void audio_thread() {
    PAXSound snd;
    snd.setSamplerate(24000);
    OggOpusFile *of = op_open_file("/data/app/MAINAPP/lib/audio.opus", 0);
    if (of == NULL) {
        return;
    }
    int ret;
    opus_int16 pcm[5760];
    while (1) {
        ret = op_read(of, pcm, 5760, NULL);
        if (ret < 0) {
            break;
        }
        for(int i = 0;i < ret;i++) {
            pcm[i] = pcm[i] / 4;
        }

        void* ptr = pcm;
        while (ret > 0) {
            int written = snd.playSound(ptr, ret * 2);
            ret -= written / 2;
            ptr = (void*)((uint8_t*)ptr + written);
        }
    }
    op_free(of);
}