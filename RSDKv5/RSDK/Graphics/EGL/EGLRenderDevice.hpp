#if RETRO_PLATFORM == RETRO_PS3
#include <PSGL/psgl.h>
#include <PSGL/psglu.h>

#ifndef GL_ARGB_SCE
#define GL_ARGB_SCE 0x8A52
#endif
#ifndef GL_BGRA
#define GL_BGRA 0x80E1
#endif
#ifndef GL_UNSIGNED_INT_8_8_8_8_REV
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

typedef void *EGLDisplay;
typedef void *EGLContext;
typedef void *EGLSurface;
typedef void *EGLConfig;
#endif

class RenderDevice : public RenderDeviceBase
{
public:
    struct WindowInfo {
        // WindowInfo mad useless in EGL so we cheat
        typedef struct {
            uint32 width;
            uint32 height;
            uint32 refresh_rate;
        } BasicWindowInfo;
        union {
            struct {
                uint32 width;
                uint32 height;
                uint32 refresh_rate;
            };
            BasicWindowInfo internal;
        } * displays;
    };

    static WindowInfo displayInfo;

    static bool Init();
    static void CopyFrameBuffer();
    static void FlipScreen();
    static void Release(bool32 isRefresh);

    static void RefreshWindow();
    static void GetWindowSize(int32 *width, int32 *height);

    static void SetupImageTexture(int32 width, int32 height, uint8 *imagePixels);
    static void SetupVideoTexture_YUV420(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                         int32 strideV);
    static void SetupVideoTexture_YUV422(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                         int32 strideV);
    static void SetupVideoTexture_YUV444(int32 width, int32 height, uint8 *yPlane, uint8 *uPlane, uint8 *vPlane, int32 strideY, int32 strideU,
                                         int32 strideV);

    static bool ProcessEvents();

    static void InitFPSCap();
    static bool CheckFPSCap();
    static void UpdateFPSCap();

    static bool InitShaders();
    static void LoadShader(const char *fileName, bool32 linear);

    static inline void ShowCursor(bool32 shown) {}
    static inline bool GetCursorPos(Vector2 *pos) { return false; }
    static inline void SetWindowTitle() {}

    static EGLDisplay display;
    static EGLContext context;
    static EGLSurface surface;
    static EGLConfig config;

#if RETRO_PLATFORM == RETRO_SWITCH
    static NWindow *window;
#elif RETRO_PLATFORM == RETRO_ANDROID
    static ANativeWindow *window;
#elif RETRO_PLATFORM == RETRO_PS3
    static PSGLdevice *psglDevice;
    static PSGLcontext *psglContext;
#endif

    static GLuint screenTextures[SCREEN_COUNT];
    static GLuint imageTexture;

    static bool32 isInitialized;

private:
    static bool SetupRendering();
    static void InitVertexBuffer();
    static bool InitGraphicsAPI();

    static void GetDisplays();

    static void SetLinear(bool32 linear);

    static int32 monitorIndex;

    static GLuint VAO;
    static GLuint VBO;

#if RETRO_PLATFORM == RETRO_PS3
    static RenderVertex vertexBuffer[60];
#endif

    static uint32 *videoBuffer;
};

struct ShaderEntry : public ShaderEntryBase {
    GLuint programID;
};
