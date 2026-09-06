#define LockAudioDevice()
#define UnlockAudioDevice()

#if RETRO_PLATFORM == RETRO_PS3
#include <sys/ppu_thread.h>
#else
#include <thread>
#endif

namespace RSDK
{
class AudioDevice : public AudioDeviceBase
{
public:
    static ma_device device;

    static bool32 Init();
    static void Release();

    static void FrameInit() {}

#if RETRO_PLATFORM == RETRO_PS3
    static void LoadStreamPPUThread(uint64_t arg)
    {
        LoadStream((ChannelInfo *)(uintptr_t)arg);
        sys_ppu_thread_exit(0);
    }
#endif

    inline static void HandleStreamLoad(ChannelInfo *channel, bool32 async)
    {
        if (async) {
#if RETRO_PLATFORM == RETRO_PS3
            sys_ppu_thread_t thread_id;
            sys_ppu_thread_create(&thread_id, LoadStreamPPUThread, (uint64_t)(uintptr_t)channel, 1000, 0x4000, SYS_PPU_THREAD_CREATE_JOINABLE, "AudioStreamLoad");
#else
            std::thread thread(LoadStream, channel);
            thread.detach();
#endif
        }
        else
            LoadStream(channel);
    }

private:
    static uint8 contextInitialized;

    static void InitAudioChannels();

    static void AudioCallback(ma_device* device, void *output, const void *input, ma_uint32 frameCount);
};
} // namespace RSDK