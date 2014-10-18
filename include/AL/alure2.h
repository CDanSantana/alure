#ifndef AL_ALURE2_H
#define AL_ALURE2_H

#include <vector>
#include <string>

#include "alc.h"
#include "al.h"

namespace alure {

class DeviceManager;
class Device;
class Context;
class Buffer;
class Decoder;
class Source;


inline ALCuint MakeVersion(ALCushort major, ALCushort minor)
{ return (major<<16) | minor; }

inline ALCuint MajorVersion(ALCuint version)
{ return version>>16; }
inline ALCuint MinorVersion(ALCuint version)
{ return version&0xffff; }


enum DeviceEnumeration {
    DevEnum_Basic = ALC_DEVICE_SPECIFIER,
    DevEnum_Complete = ALC_ALL_DEVICES_SPECIFIER,
    DevEnum_Capture = ALC_CAPTURE_DEVICE_SPECIFIER
};

enum DefaultDeviceType {
    DefaultDevType_Basic = ALC_DEFAULT_DEVICE_SPECIFIER,
    DefaultDevType_Complete = ALC_DEFAULT_ALL_DEVICES_SPECIFIER,
    DefaultDevType_Capture = ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER
};

class DeviceManager {
protected:
    virtual ~DeviceManager() { }

public:
    static DeviceManager *get();

    virtual bool queryExtension(const char *extname) = 0;

    virtual std::vector<std::string> enumerate(DeviceEnumeration type) = 0;
    virtual std::string defaultDeviceName(DefaultDeviceType type) = 0;

    virtual Device *openPlayback(const std::string &name=std::string()) = 0;
};


enum PlaybackDeviceType {
    PlaybackDevType_Basic = ALC_DEVICE_SPECIFIER,
    PlaybackDevType_Complete = ALC_ALL_DEVICES_SPECIFIER
};

class Device {
protected:
    virtual ~Device() { }

public:
    virtual std::string getName(PlaybackDeviceType type) = 0;
    virtual bool queryExtension(const char *extname) = 0;

    virtual ALCuint getALCVersion() = 0;
    virtual ALCuint getEFXVersion() = 0;

    virtual ALCuint getFrequency() = 0;

    virtual Context *createContext(ALCint *attribs=0) = 0;

    virtual void close() = 0;
};


class Context {
protected:
    virtual ~Context() { }

public:
    static void MakeCurrent(Context *context);
    static Context *GetCurrent();

    static void MakeThreadCurrent(Context *context);
    static Context *GetThreadCurrent();

    virtual void destroy() = 0;

    virtual Device *getDevice() = 0;

    virtual void startBatch() = 0;
    virtual void endBatch() = 0;

    virtual Decoder *createDecoder(const std::string &name) = 0;

    // Functions below require the context to be current
    virtual Buffer *getBuffer(const std::string &name) = 0;
    virtual Buffer *getBuffer(Decoder *decoder) = 0;
    virtual void removeBuffer(const std::string &name) = 0;
    virtual void removeBuffer(Buffer *buffer) = 0;

    virtual Source *getSource() = 0;
    virtual void finalize(Source *source) = 0;

    virtual void setPosition(ALfloat x, ALfloat y, ALfloat z) = 0;
    virtual void setPosition(const ALfloat *pos) = 0;

    virtual void setVelocity(ALfloat x, ALfloat y, ALfloat z) = 0;
    virtual void setVelocity(const ALfloat *vel) = 0;

    virtual void setOrientation(ALfloat x1, ALfloat y1, ALfloat z1, ALfloat x2, ALfloat y2, ALfloat z2) = 0;
    virtual void setOrientation(const ALfloat *at, const ALfloat *up) = 0;
    virtual void setOrientation(const ALfloat *ori) = 0;

    virtual void update() = 0;
};


class Buffer {
protected:
    virtual ~Buffer() { }

public:
    virtual ALuint getLength() = 0;

    virtual ALuint getFrequency() = 0;
    virtual ALuint getSize() = 0;

    virtual bool isRemovable() const = 0;
};


enum SampleType {
    SampleType_UInt8,
    SampleType_Int16,
    SampleType_Float32,
    SampleType_Mulaw
};

enum SampleConfig {
    SampleConfig_Mono,
    SampleConfig_Stereo,
    SampleConfig_Rear,
    SampleConfig_Quad,
    SampleConfig_X51,
    SampleConfig_X61,
    SampleConfig_X71,
    SampleConfig_BFmt_WXY,
    SampleConfig_BFmt_WXYZ
};

class Decoder {
public:
    virtual ~Decoder() { }

    virtual ALuint getFrequency() = 0;
    virtual SampleConfig getSampleConfig() = 0;
    virtual SampleType getSampleType() = 0;

    virtual ALuint getLength() = 0;
    virtual ALuint getPosition() = 0;
    virtual bool seek(ALuint pos) = 0;

    virtual ALuint read(ALvoid *ptr, ALuint count) = 0;
};


class Source {
protected:
    virtual ~Source() { }

public:
    virtual void setLooping(bool looping) = 0;
    virtual bool getLooping() const = 0;

    virtual void play(Buffer *buffer) = 0;
    virtual void play(Decoder *decoder, ALuint updatelen, ALuint queuesize) = 0;
    virtual void stop() = 0;

    virtual bool isPlaying() const = 0;

    virtual ALuint getOffset() const = 0;

    virtual void setPosition(ALfloat x, ALfloat y, ALfloat z) = 0;
    virtual void setPosition(const ALfloat *pos) = 0;

    virtual void setVelocity(ALfloat x, ALfloat y, ALfloat z) = 0;
    virtual void setVelocity(const ALfloat *vel) = 0;

    virtual void setDirection(ALfloat x, ALfloat y, ALfloat z) = 0;
    virtual void setDirection(const ALfloat *dir) = 0;

    virtual void update() = 0;
};

} // namespace alure

#endif /* AL_ALURE2_H */
