
#include "pax/sound.h"
#include "opusfile.h"
#include <future>
opus_int16 pcm[5760 * 16];
opus_int16 pcm_play[5760 * 16];
OggOpusFile *of = NULL;
std::promise<int> file_open_promise;
void audio_set_gain(int gain)
{
    op_set_gain_offset(of, OP_HEADER_GAIN, gain);
}
std::future<int> audio_get_file_open_promise()
{
    return file_open_promise.get_future();
}
void audio_thread()
{
    PAXSound snd;
    of = op_open_file("/data/app/MAINAPP/lib/audio.opus", 0);
    if (of == NULL)
    {
        return;
    }
    file_open_promise.set_value(0);
    int ret;
    while (1)
    {
        ret = op_read(of, pcm, 5760 * 8, 0);
        if (ret <= 0)
        {
            break;
        }
        for (int i = 0; i < ret; i++)
        {
            pcm_play[i * 2] = pcm[i];
            pcm_play[i * 2 + 1] = pcm[i];
        }
        void *ptr = pcm_play;
        while (ret > 0)
        {
            int written = snd.playSound(ptr, ret * 4);
            ret -= written / 2;
            ptr = (void *)((uint8_t *)ptr + written);
        }
    }
    op_free(of);
}