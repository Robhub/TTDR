#include "ttdr.h"

vector<DRDAT> TTDR::vecDRDAT = vector<DRDAT>();

TTDR TTDR::process(const char *path)
{
    vecDRDAT.clear();
    processSamples(path);
    return processDR();
}

void TTDR::processSamples(const char *path)
{
    SoundFile sf;
    if (!sf.load(path)) { cerr << "Cannot open file " << path << endl; return; }
    cout << "Metadata: " << sf.getString(SF_STR_ARTIST) << " - " << sf.getString(SF_STR_ALBUM) << " - " << sf.getString(SF_STR_TITLE) << endl;

    int framesPerWindow = 3 * sf.samplerate(); // 3sec
    if (framesPerWindow == 132300) framesPerWindow = 132480; // To get a multiple of 128 maybe?

    vector<float> samples;
    int chans = sf.channels();
    samples.resize(framesPerWindow * chans);

    vecDRDAT.resize(chans);

    int numWindows = 0;
    for (int p = 0; p < sf.frames(); p += framesPerWindow)
    {
        int nread = sf.read(&samples[0], p, framesPerWindow);

        // Zero-padding for the last window
        for (int i = nread * chans; i < (int)samples.size(); i++)
        {
            samples[i] = 0;
        }

        // Initial values
        for (int ch = 0; ch < chans; ch++)
        {
            vecDRDAT[ch].sumRMS = 0;
            vecDRDAT[ch].peak = 0;
        }

        // Process samples
        for (int i = 0; i < framesPerWindow; i++)
        {
            for (int ch = 0; ch < chans; ch++)
            {
                float x = samples[i*chans+ch];
                vecDRDAT[ch].sumRMS += x*x;
                vecDRDAT[ch].peak = max(vecDRDAT[ch].peak, abs(x));
            }
        }

        // Final values
        for (int ch = 0; ch < chans; ch++)
        {
            vecDRDAT[ch].vecPeaks.emplace_back(vecDRDAT[ch].peak);
            vecDRDAT[ch].vecRMS.emplace_back(sqrt(2*vecDRDAT[ch].sumRMS/framesPerWindow));
        }
        numWindows++;
    }
}

TTDR TTDR::processDR()
{
    TTDR ttdr;
    float sumDR = 0;
    for (DRDAT &drdat:vecDRDAT)
    {
        sort(drdat.vecRMS.begin(), drdat.vecRMS.end(), greater<float>());
        sort(drdat.vecPeaks.begin(), drdat.vecPeaks.end(), greater<float>());

        float peak = drdat.vecPeaks.size() < 5 ? drdat.vecPeaks[0] : drdat.vecPeaks[1];

        DRRES res;
        res.peak = todb(drdat.vecPeaks[0]);
        res.avgRMS = todb(calcRMS(drdat.vecRMS));
        res.DR = todb(peak/calcRMS(drdat.vecRMS, 0.2));
        ttdr.res.emplace_back(res);

        sumDR += res.DR;
    }
    ttdr.dr = sumDR/vecDRDAT.size();
    return ttdr;
}

float TTDR::calcRMS(vector<float> vec, float percent)
{
    int n = vec.size() * percent;
    float res = 0;
    for (int i = 0; i < n; i++)
    {
        float val = vec[i];
        res += val * val;
    }
    return sqrt(res/n);
}
