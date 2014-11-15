#include "SoundFile.h"

SoundFile::SoundFile()
{
    m_file = nullptr;
}

SoundFile::~SoundFile()
{
    close();
}

void SoundFile::close()
{
    if (m_file != nullptr) sf_close(m_file);
}

bool SoundFile::load(const char* path)
{
    close();
    m_file = sf_open(path, SFM_READ, &m_info);
    return m_file != nullptr;
}

const char* SoundFile::getString(int type)
{
    if (m_file == nullptr) return "?";
    const char* res = sf_get_string(m_file, type);
    return res == nullptr ? "?" : res;
}

int SoundFile::read(float* samples, int pos, int nb)
{
    if (m_file == nullptr) return 0;
    sf_seek(m_file, pos, SEEK_SET);
    return sf_readf_float(m_file, samples, nb);
}
