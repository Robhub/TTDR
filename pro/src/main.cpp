#include "ttdr.h"
#include <sstream>
#include <iomanip>
using namespace std;

//#define METAFLAC

void addValue(stringstream &stst, float val)
{
    if (!stst.str().empty()) stst << "   ---   ";
    char sval[12];
    sprintf(sval, "%7.2f", val);
    stst << sval << " db";
}

int main(int argc, char *argv[])
{
    for (int a = 1; a < argc; a++)
    {
        cout << "Processing file: " << argv[a] << endl;
        TTDR ttdr = TTDR::process(argv[a]);
        float dr = round(ttdr.getDR()*10)/10;
        vector<DRRES> res = ttdr.getRes();
        stringstream resPeak;
        stringstream resRMS;
        stringstream resDR;
        resPeak << fixed << setprecision(2);
        resRMS << fixed << setprecision(2);
        resDR << fixed << setprecision(2);
        for (DRRES &drres:res)
        {
            addValue(resPeak, drres.peak);
            addValue(resRMS, drres.avgRMS);
            addValue(resDR, drres.DR);
        }
        cout << "Peak Value:   " << resPeak.str() << endl;
        cout << "Avg RMS:      " << resRMS.str() << endl;
        cout << "DR channel:   " << resDR.str() << endl;
        cout << "DR: " << dr << endl;
        cout << endl;
#ifdef METAFLAC
        stringstream metaflac;
        metaflac << "metaflac --remove-tag=TTDR_TRACK --set-tag=\"TTDR_TRACK=" << dr << "\" " << path;
        system(metaflac.str().c_str());
#endif
    }
    //system("pause");
    return 0;
}
