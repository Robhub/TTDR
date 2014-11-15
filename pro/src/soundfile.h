#ifndef SOUNDFILE_H
#define SOUNDFILE_H

#include "sndfile.h"

class SoundFile
{
public:
    SoundFile();
    ~SoundFile();
    void close();
    const char* getString(int type);
    bool load(const char* path);
    int read(float* samples, int pos, int nb);
    int channels()      { return m_info.channels; }
    int samplerate()    { return m_info.samplerate; }
    int frames()        { return m_info.frames; }

private:
    SNDFILE*    m_file;
    SF_INFO     m_info;
};

#endif // SOUNDFILE_H
