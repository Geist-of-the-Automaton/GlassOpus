#include "mainwindow.h"
#include <QApplication>
#include <Windows.h>

#pragma comment(lib, "urlmon.lib")

int main(int argc, char *argv[])
{
    /*
    mat4 rgb2lms({{0.3811, 0.5783, 0.0402},
                  {0.1967, 0.7244, 0.0782},
                  {0.0241, 0.1228, 0.8444}});
    float COMMON[3][3] = {{1.0f / sqrtf(3.0), 0.0, 0.0},
                          {0.0, 1.0f / sqrtf(6.0), 0.0},
                          {0.0, 0.0, 1.0f / sqrtf(2.0)}};
    // transpose for lab2lms
    float TRANS[3][3] = {{1.0, 1.0,  1.0},
                         {1.0, 1.0, -1.0},
                         {1.0, -2.0, 0.0}};
    mat4 common(COMMON);
    mat4 trans(TRANS);
    mat4 lms2lab = common * trans;
    float hl, ll, ha, la, hb, lb;
    hl = ha = hb = -1000000.0;
    ll = la = lb = 1000000.0;
    float ipol = 0.0001, stop = 1.0;
    for (float i = 0.0; i <= stop; i += ipol) {
        for (float j = 0.0; j <= stop; j += ipol)
            for (float k = 0.0; k <= stop; k += ipol) {
                vec4 v(i, j, k);
                v = rgb2lms * v;
                if (i == 0.0)
                    v._L = 0.00001;
                if (j == 0.0)
                    v._A = 0.00001;
                if (k == 0.0)
                    v._B = 0.00001;
                v._L = log10(v._L);
                v._A = log10(v._A);
                v._B = log10(v._B);
                v = lms2lab * v;
                hl = max(hl, v[0]);
                ll = min(ll, v[0]);

                ha = max(ha, v[1]);
                la = min(la, v[1]);
                hb = max(hb, v[2]);
                lb = min(lb, v[2]);

            }
        cout << i << endl;
    }
    cout << endl;
    cout << hl << endl;
    cout << ll << endl;
    cout << ha << endl;
    cout << la << endl;
    cout << hb << endl;
    cout << lb << endl;
    cout << endl;
    return 0;

*/

    int temp = argc;
    argc = 1;
    QApplication a(argc, argv);
    string startPath = argv[0];
    MainWindow w(startPath.substr(0, startPath.find_last_of("\\")), temp == 1 ? "" : argv[1]);
    w.show();
    return a.exec();
}
