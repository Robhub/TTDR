#ifndef TTDR_H
#define TTDR_H

#include "soundfile.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

struct DRDAT
{
    float sumRMS;
    float peak;
    vector<float> vecRMS;
    vector<float> vecPeaks;
};

struct DRRES
{
    float peak;
    float avgRMS;
    float DR;
};

/// Implementation of the TTDR (TT Dynamic Range) algorithm of Pleasurize Music Foundation.
/// Might be ±0.01 dB inaccurate (and thus ±1DR inaccurate for the rounded value if you are unlucky)
/// Thus, it is recommanded to keep at least one decimal digit precision with this implementation.
/// Inspired by:
/// https://github.com/adiblol/dr_meter/blob/master/main.c
/// https://github.com/Janwillhaus/DRmeter-for-Matlab/blob/master/drmeter.m
/// https://github.com/rtsurik/audacious-dr-meter-plugin/blob/master/dr_meter.c
/// https://github.com/simon-r/dr14_t.meter/blob/master/dr14tmeter/compute_dr14.py
/// https://github.com/x42/meters.lv2/blob/master/src/dr14.c
class TTDR
{
public:
    static TTDR process(const char *path);
    float getDR() { return dr; }
    vector<DRRES> getRes() { return res; }

private:
    static void processSamples(const char *path);
    static TTDR processDR();
    static float calcRMS(vector<float> vec, float percent = 1.f);
    static inline float todb(float x) { return 20 * log10(x); }

    static vector<DRDAT> vecDRDAT;
    vector<DRRES> res;
    float dr;
};

#endif // TTDR_H
