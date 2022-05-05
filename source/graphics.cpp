#include "graphics.h"


graphics::Filter::Filter(int str, string filterName) {
    strength = str;
    setFilter(filterName);
}

graphics::Filter::Filter(const Filter &filter) {
    strength = filter.strength;
    filterApplicator = filter.filterApplicator;
    filterIndex = filter.filterIndex;
}

graphics::Filter& graphics::Filter::operator=(const Filter &rhs) {
    strength = rhs.strength;
    filterIndex = rhs.filterIndex;
    filterApplicator = rhs.filterApplicator;
    return *this;
}
void graphics::Filter::applyTo(QImage *qi) {
    int h = qi->height(), w = qi->width();
    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(y));
        for (int x = 0; x < w; ++x)
            if (line[x] & 0xFF000000)
                line[x] = filterApplicator(QColor(line[x]), strength);
    }
}

QRgb graphics::Filter::applyTo(QColor qc) {
    return filterApplicator(qc, strength);
}

int graphics::Filter::getStrength() {
    return strength;
}

void graphics::Filter::setStrength(int value) {
    strength = stdFuncs::clamp(value, minColor, maxColor);
}

void graphics::Filter::strengthUp() {
    setStrength(strength + 1);
}

void graphics::Filter::strengthDown() {
    setStrength(strength - 1);
}

void graphics::Filter::setFilter(string filterName) {
    size_t i;
    for (i = 0; i < numFilters; ++i)
        if (filterName == filterNames[i])
            break;
    if (i == numFilters)
        i = 0;
    filterIndex = i;
    filterApplicator = filters[i];
    strength = filterPresets[i];
}

int graphics::Filter::getFilterIndex() {
    return static_cast<int>(filterIndex);
}

QRgb graphics::Filtering::toRGB(int a, int r, int g, int b) {
    QRgb color = static_cast<QRgb>(a) << 24;
    color += static_cast<QRgb>(r << 16);
    color += static_cast<QRgb>(g << 8);
    color += static_cast<QRgb>(b);
    return color;
}

QRgb graphics::Filtering::greyscale(QColor qc, int strength) {
    int combined = (qc.red() + qc.blue() + qc.green()) / 3;
    int red = (((255 - strength) * qc.red()) + (strength * combined)) / 255;
    int green = (((255 - strength) * qc.green()) + (strength * combined)) / 255;
    int blue = (((255 - strength) * qc.blue()) + (strength * combined)) / 255;
    return toRGB(qc.alpha(), red, green, blue);
}

QRgb graphics::Filtering::polarize(QColor qc, int strength) {
    int combined = (qc.red() + qc.blue() + qc.green()) / 3;
    return combined > strength ? toRGB(qc.alpha(), 255, 255, 255) : toRGB(qc.alpha(), 0, 0, 0);;
}

QRgb graphics::Filtering::negative(QColor qc, int strength) {
    return toRGB(qc.alpha(), 255 - qc.red(), 255 - qc.green(), 255 - qc.blue());
}

QRgb graphics::Filtering::enshadow(QColor qc, int strength) {
    int combined = (qc.red() + qc.blue() + qc.green()) / 3;
    return combined > strength ? qc.rgba() : (static_cast<QRgb>(qc.alpha()) << 24);
}

QRgb graphics::Filtering::redChannel(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.red(), qc.red(), qc.red());
}

QRgb graphics::Filtering::greenChannel(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.green(), qc.green(), qc.green());
}

QRgb graphics::Filtering::blueChannel(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.blue(), qc.blue(), qc.blue());
}

QRgb graphics::Filtering::yellowChannel(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.yellow(), qc.yellow(), qc.yellow());
}

QRgb graphics::Filtering::cyanChannel(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.cyan(), qc.cyan(), qc.cyan());
}

QRgb graphics::Filtering::magentaChannel(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.magenta(), qc.magenta(), qc.magenta());
}

QRgb graphics::Filtering::hueChannel(QColor qc, int strength) {
    int hue = static_cast<int>(255.0 * qc.hueF());
    return toRGB(qc.alpha(), hue, hue, hue);
}

QRgb graphics::Filtering::satChannel(QColor qc, int strength) {
    int sat = static_cast<int>(255.0 * qc.hslSaturationF());
    return toRGB(qc.alpha(), sat, sat, sat);
}

QRgb graphics::Filtering::valChannel(QColor qc, int strength) {
    int val = static_cast<int>(255.0 * qc.valueF());
    return toRGB(qc.alpha(), val, val, val);
}

QRgb graphics::Filtering::litChannel(QColor qc, int strength) {
    int val = static_cast<int>(255.0 * qc.lightnessF());
    return toRGB(qc.alpha(), val, val, val);
}

QRgb graphics::Filtering::redPass(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 4;
    if (hue < 0 + str && hue > 360 - str)
        return qc.rgba();
    else {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
}

QRgb graphics::Filtering::greenPass(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 4;
    if (hue < 120 + str && hue > 120 - str)
        return qc.rgba();
    else {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
}

QRgb graphics::Filtering::bluePass(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 4;
    if (hue < 240 + str && hue > 240 - str)
        return qc.rgba();
    else {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
}

QRgb graphics::Filtering::magentaPass(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 8;
    if (hue < 300 + str && hue > 300 - str)
        return qc.rgba();
    else {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
}

QRgb graphics::Filtering::yellowPass(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 8;
    if (hue < 60 + str && hue > 60 - str)
        return qc.rgba();
    else {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
}

QRgb graphics::Filtering::cyanPass(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 8;
    if (hue < 180 + str && hue > 180 - str)
        return qc.rgba();
    else {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
}

QRgb graphics::Filtering::redFilter(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 4;
    if (hue < 0 + str && hue > 360 - str) {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
    else
        return qc.rgba();
}

QRgb graphics::Filtering::greenFilter(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 4;
    if (hue < 120 + str && hue > 120 - str) {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
    else
        return qc.rgba();
}

QRgb graphics::Filtering::blueFilter(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 4;
    if (hue < 240 + str && hue > 240 - str) {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
    else
        return qc.rgba();
}

QRgb graphics::Filtering::yellowFilter(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 8;
    if (hue < 60 + str && hue > 60 - str) {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
    else
        return qc.rgba();
}

QRgb graphics::Filtering::cyanFilter(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 8;
    if (hue < 180 + str && hue > 180 - str) {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
    else
        return qc.rgba();
}

QRgb graphics::Filtering::magentaFilter(QColor qc, int strength) {
    int hue = qc.hslHue();
    int str = strength / 8;
    if (hue < 300 + str && hue > 300 - str) {
        int grey = (qc.red() + qc.green() + qc.blue()) / 3;
        return toRGB(qc.alpha(), grey, grey, grey);
    }
    else
        return qc.rgba();
}

QRgb graphics::Filtering::rgb(QColor qc, int strength) {
    return qc.rgba();
}

QRgb graphics::Filtering::rbg(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.red(), qc.blue(), qc.green());
}

QRgb graphics::Filtering::grb(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.green(), qc.red(), qc.blue());
}

QRgb graphics::Filtering::gbr(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.green(), qc.blue(), qc.red());
}

QRgb graphics::Filtering::brg(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.blue(), qc.red(), qc.green());
}

QRgb graphics::Filtering::bgr(QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.blue(), qc.green(), qc.red());
}

int graphics::Filtering::Burn(int color, int strength) {
    return (color - strength) < 0 ? 0 : color - strength;
}

QRgb graphics::Filtering::burn (QColor qc, int strength) {
    return toRGB(qc.alpha(), Burn(qc.red(), strength), Burn(qc.green(), strength), Burn(qc.blue(), strength));
}

QRgb graphics::Filtering::burnRed (QColor qc, int strength) {
    return toRGB(qc.alpha(), Burn(qc.red(), strength), qc.green(), qc.blue());
}

QRgb graphics::Filtering::burnGreen (QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.red(), Burn(qc.green(), strength), qc.blue());
}

QRgb graphics::Filtering::burnBlue (QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.red(), qc.green(), Burn(qc.blue(), strength));
}

QRgb graphics::Filtering::burnYellow (QColor qc, int strength) {
    return toRGB(qc.alpha(), Burn(qc.red(), strength), Burn(qc.green(), strength), qc.blue());
}

QRgb graphics::Filtering::burnCyan (QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.red(), Burn(qc.green(), strength), Burn(qc.blue(), strength));
}

QRgb graphics::Filtering::burnMagenta (QColor qc, int strength) {
    return toRGB(qc.alpha(), Burn(qc.red(), strength), qc.green(), Burn(qc.blue(), strength));
}

int graphics::Filtering::Dodge(int color, int strength) {
    return (color + strength) > 255 ? 255 : color + strength;
}

QRgb graphics::Filtering::dodge (QColor qc, int strength) {
    return toRGB(qc.alpha(), Dodge(qc.red(), strength), Dodge(qc.green(), strength), Dodge(qc.blue(), strength));
}
QRgb graphics::Filtering::dodgeRed (QColor qc, int strength) {
    return toRGB(qc.alpha(), Dodge(qc.red(), strength), qc.green(), qc.blue());
}

QRgb graphics::Filtering::dodgeGreen (QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.red(), Dodge(qc.green(), strength), qc.blue());
}

QRgb graphics::Filtering::dodgeBlue (QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.red(), qc.green(), Dodge(qc.blue(), strength));
}

QRgb graphics::Filtering::dodgeYellow (QColor qc, int strength) {
    return toRGB(qc.alpha(), Dodge(qc.red(), strength), Dodge(qc.green(), strength), qc.blue());
}

QRgb graphics::Filtering::dodgeCyan (QColor qc, int strength) {
    return toRGB(qc.alpha(), qc.red(), Dodge(qc.green(), strength), Dodge(qc.blue(), strength));
}

QRgb graphics::Filtering::dodgeMagenta (QColor qc, int strength) {
    return toRGB(qc.alpha(), Dodge(qc.red(), strength), qc.green(), Dodge(qc.blue(), strength));
}

QRgb graphics::Filtering::greyFilmGrain (QColor qc, int strength) {
    int grey = rand() % (strength + 1);
    if (rand() % 2)
        grey = -grey;
    return toRGB(qc.alpha(), stdFuncs::clamp(qc.red() + grey, minColor, maxColor), stdFuncs::clamp(qc.green() + grey, minColor, maxColor), stdFuncs::clamp(qc.blue() + grey, minColor, maxColor));
}

QRgb graphics::Filtering::colorFilmGrain (QColor qc, int strength) {
    ++strength;
    int grey = rand() % strength;
    if (rand() % 2)
        grey = -grey;
    int red = stdFuncs::clamp(qc.red() + grey, minColor, maxColor);
    grey = rand() % strength;
    if (rand() % 2)
        grey = -grey;
    int green = stdFuncs::clamp(qc.green() + grey, minColor, maxColor);
    grey = rand() % strength;
    if (rand() % 2)
        grey = -grey;
    int blue = stdFuncs::clamp(qc.blue() + grey, minColor, maxColor);
    return toRGB(qc.alpha(), red, green, blue);
}

void graphics::Filtering::applyKernel(QProgressDialog *qpd, QImage *qi, KernelData kernalInfo) {
    bool needGreyscale = kernalInfo.first;
    Kernel kernal = kernalInfo.second;
    int kernalSize = kernal.size();
    if (kernalSize == 0)
        return;
    if (qpd != nullptr) {
        qpd->setValue(0);
        qpd->setLabelText("Applying Kernal");
        qpd->setMaximum(qi->height());
        qpd->show();
    }
    if (needGreyscale) {
        qpd->setMaximum(qpd->maximum() * 2 + 1);
        Filter filter(255, "Greyscale");
        filter.applyTo(qi);
        if (qpd != nullptr) {
            qpd->setValue(qpd->value() + 1);
            QCoreApplication::processEvents();
        }
    }
    int boost = 1;
    int offset = kernalSize / 2;
    QImage image = qi->copy();
    // Apply kernal to the image.
    for (int j = 0; j < qi->height(); ++j) {
        if (qpd != nullptr) {
            qpd->setValue(qpd->value() + 1);
            QCoreApplication::processEvents();
        }
        QRgb *qiLine = reinterpret_cast<QRgb *>(qi->scanLine(j));
        for (int i = 0; i < qi->width(); ++i) {
            int xstart = i - offset, ystart = j - offset, xend = i + offset, yend = j + offset;
            float r = 0.0, g = 0.0, b = 0.0;
            for (int y = ystart; y <= yend; ++y) {
                int dy = y - ystart;
                QRgb *imgLine = reinterpret_cast<QRgb *>(image.scanLine(y));
                for (int x = xstart; x <= xend; ++x) {
                    int dx = x - xstart;
                    QColor qc = (x < 0 || y < 0 || y >= qi->height() || x >= qi->width()) ? qiLine[i] : imgLine[x];
                    r += kernal[dx][dy] * static_cast<float>(qc.red());
                    g += kernal[dx][dy] * static_cast<float>(qc.green());
                    b += kernal[dx][dy] * static_cast<float>(qc.blue());
                }
            }
            r = stdFuncs::clamp(r, 0.0, 255.0);
            g = stdFuncs::clamp(g, 0.0, 255.0);
            b = stdFuncs::clamp(b, 0.0, 255.0);
            if (needGreyscale)
                boost = max(boost, min(static_cast<int>(max(r, max(g, b))), 255));
            qiLine[i] = QColor(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b)).rgba();
        }
    }
    // Boost contrast with greyscale output so that the result is more visible / distinguishable.
    if (needGreyscale) {
        float boostF = 255.0 / static_cast<float>(boost);
        for (int j = 0; j < qi->height(); ++j) {
            QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
            if (qpd != nullptr) {
                qpd->setValue(qpd->value() + 1);
                QCoreApplication::processEvents();
            }
            for (int i = 0; i < qi->height(); ++i) {
                int color = static_cast<int>(static_cast<float>(line[i] & 0x000000FF) * boostF);
                line[i] = QColor(color, color, color).rgba();
            }
        }
    }
}

void graphics::Color::ditherBayer(QImage *qi, int bpp, int matrixSize) {
    float div = 1.0 / static_cast<float>(matrixSize * matrixSize - 1);
    vector <vector <float> > arr;
    arr.resize(matrixSize);
    for (int i = 0; i < matrixSize; ++i)
        arr[i].resize(matrixSize);
    float og[2][2];
    og[0][0] = arr[0][0] = 0;
    og[1][0] = arr[1][0] = 3;
    og[0][1] = arr[0][1] = 2;
    og[1][1] = arr[1][1] = 1;
    int size = 2;
    while (matrixSize != size) {
        for (int i = 1; i >= 0; --i) {
            int I = i * size;
            for (int j = 1; j >= 0; --j) {
                int J = j * size;
                for (int m = 0; m < size; ++m) {
                    for (int n = 0; n < size; ++n)
                        arr[m + I][n + J] = 4 * arr[m][n] + og[i][j];
                }
            }
        }
        size *= 2;
    }
    float BPP = static_cast<float>(bpp);
    for (int i = 0; i < matrixSize; ++i)
        for (int j = 0; j < matrixSize; ++j) {
            arr[i][j] *= div;
            arr[i][j] -= 0.5;
            arr[i][j] *= 255.0 / BPP;
        }
    int bitShift = 8 - bpp;
    for (int i = 0; i < qi->width(); ++i) {
        int I = i % matrixSize;
        for (int j = 0; j < qi->height(); ++j) {
            int J = j % matrixSize;
            QColor qp = qi->pixelColor(i, j);
            int r = qp.red() + arr[I][J], g = qp.green() + arr[I][J], b = qp.blue() + arr[I][J];
            r = stdFuncs::clamp(r, 0, 255);
            g = stdFuncs::clamp(g, 0, 255);
            b = stdFuncs::clamp(b, 0, 255);
            r = (r >> bitShift) << bitShift;
            g = (g >> bitShift) << bitShift;
            b = (b >> bitShift) << bitShift;
            qi->setPixelColor(i, j, QColor(r, g, b, qp.alpha()));
        }
    }
}

void graphics::Color::ditherRandom(QImage *qi, int bpp) {
    int bitShift;
    for (int i = 0; i < qi->width(); ++i)
        for (int j = 0; j < qi->height(); ++j) {
            bitShift = 8 - (bpp - rand() % bpp);
            QColor qc = qi->pixelColor(i, j);
            int r = (qc.red() >> bitShift) << bitShift;
            int g = (qc.green() >> bitShift) << bitShift;
            int b = (qc.blue() >> bitShift) << bitShift;
            qi->setPixelColor(i, j, QColor(r, g, b, qc.alpha()));
        }

}

void graphics::Color::paletteReduction(QImage *qi, int bpp) {
    int bitShift = 8 - bpp;
    for (int i = 0; i < qi->width(); ++i)
        for (int j = 0; j < qi->height(); ++j) {
            QColor qc = qi->pixelColor(i, j);
            int r = (qc.red() >> bitShift) << bitShift;
            int g = (qc.green() >> bitShift) << bitShift;
            int b = (qc.blue() >> bitShift) << bitShift;
            qi->setPixelColor(i, j, QColor(r, g, b, qc.alpha()));
        }
}

void graphics::Color::ditherFloydSteinberg(QImage *qi, int bpp) {
    int r, g, b, acc = 0;
    bpp = 8 - bpp;
    float rErr, gErr, bErr;
    float A = 7.0 / 16.0, B = 3.0 / 16.0, C = 5.0 / 16.0, D = 1.0 / 16.0;
    int w = qi->width(), h = qi->height(), size = w * h;
    vector <int> errorR(size, 0);
    vector <int> errorG(size, 0);
    vector <int> errorB(size, 0);
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            QColor qc = qi->pixelColor(i, j);
            r = ((qc.red() + errorR[acc]) >> bpp) << bpp;
            g = ((qc.green() + errorG[acc]) >> bpp) << bpp;
            b = ((qc.blue() + errorB[acc]) >> bpp) << bpp;
            r = stdFuncs::clamp(r, 0, 255);
            g = stdFuncs::clamp(g, 0, 255);
            b = stdFuncs::clamp(b, 0, 255);
            rErr = static_cast<float>(qc.red() - r);
            gErr = static_cast<float>(qc.green() - g);
            bErr = static_cast<float>(qc.blue() - b);
            qi->setPixelColor(i, j, QColor(r, g, b, qc.alpha()));
            int offset = acc + 1;
            if (i + 1 < w) {
                errorR[offset] += static_cast<int>(rErr * A);
                errorG[offset] += static_cast<int>(gErr * A);
                errorB[offset] += static_cast<int>(bErr * A);
            }
            if (j + 1 < h) {
                offset += w - 2;
                if (i - 1 >= 0) {
                    errorR[offset] += static_cast<int>(rErr * B);
                    errorG[offset] += static_cast<int>(gErr * B);
                    errorB[offset] += static_cast<int>(bErr * B);
                }
                ++offset;
                errorR[offset] += static_cast<int>(rErr * C);
                errorG[offset] += static_cast<int>(gErr * C);
                errorB[offset] += static_cast<int>(bErr * C);
                ++offset;
                if (i + 1 < w) {
                    errorR[offset] += static_cast<int>(rErr * D);
                    errorG[offset] += static_cast<int>(gErr * D);
                    errorB[offset] += static_cast<int>(bErr * D);
                }
            }
            ++acc;
        }
    }
}

void graphics::Color::ditherSierra(QImage *qi, int bpp) {
    int r, g, b, acc = 0;
    bpp = 8 - bpp;
    float rErr, gErr, bErr;
    float A = 2.0 / 32.0, B = 3.0 / 32.0, C = 4.0 / 32.0, D = 4.0 / 32.0;
    int w = qi->width(), h = qi->height(), size = w * h;
    vector <int> errorR(size, 0);
    vector <int> errorG(size, 0);
    vector <int> errorB(size, 0);
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            QColor qc = qi->pixelColor(i, j);
            r = ((qc.red() + errorR[acc]) >> bpp) << bpp;
            g = ((qc.green() + errorG[acc]) >> bpp) << bpp;
            b = ((qc.blue() + errorB[acc]) >> bpp) << bpp;
            r = stdFuncs::clamp(r, 0, 255);
            g = stdFuncs::clamp(g, 0, 255);
            b = stdFuncs::clamp(b, 0, 255);
            rErr = static_cast<float>(qc.red() - r);
            gErr = static_cast<float>(qc.green() - g);
            bErr = static_cast<float>(qc.blue() - b);
            qi->setPixelColor(i, j, QColor(r, g, b, qc.alpha()));
            int offset = acc + 1;
            if (i + 1 < w) {
                errorR[offset] += static_cast<int>(rErr * D);
                errorG[offset] += static_cast<int>(gErr * D);
                errorB[offset] += static_cast<int>(bErr * D);
            }
            ++offset;
            if (i + 2 < w) {
                errorR[offset] += static_cast<int>(rErr * B);
                errorG[offset] += static_cast<int>(gErr * B);
                errorB[offset] += static_cast<int>(bErr * B);
            }
            if (j + 1 < h) {
                offset += w - 4;
                if (i - 2 >= 0) {
                    errorR[offset] += static_cast<int>(rErr * A);
                    errorG[offset] += static_cast<int>(gErr * A);
                    errorB[offset] += static_cast<int>(bErr * A);
                }
                ++offset;
                if (i - 1 >= 0) {
                    errorR[offset] += static_cast<int>(rErr * C);
                    errorG[offset] += static_cast<int>(gErr * C);
                    errorB[offset] += static_cast<int>(bErr * C);
                }
                ++offset;
                errorR[offset] += static_cast<int>(rErr * D);
                errorG[offset] += static_cast<int>(gErr * D);
                errorB[offset] += static_cast<int>(bErr * D);
                ++offset;
                if (i + 1 < w) {
                    errorR[offset] += static_cast<int>(rErr * C);
                    errorG[offset] += static_cast<int>(gErr * C);
                    errorB[offset] += static_cast<int>(bErr * C);
                }
                ++offset;
                if (i + 2 < w) {
                    errorR[offset] += static_cast<int>(rErr * A);
                    errorG[offset] += static_cast<int>(gErr * A);
                    errorB[offset] += static_cast<int>(bErr * A);
                }
                if (j + 2 < h) {
                    offset += w - 3;
                    if (i - 1 >= 0) {
                        errorR[offset] += static_cast<int>(rErr * A);
                        errorG[offset] += static_cast<int>(gErr * A);
                        errorB[offset] += static_cast<int>(bErr * A);
                    }
                    ++offset;
                    errorR[offset] += static_cast<int>(rErr * B);
                    errorG[offset] += static_cast<int>(gErr * B);
                    errorB[offset] += static_cast<int>(bErr * B);
                    ++offset;
                    if (i + 1 < w) {
                        errorR[offset] += static_cast<int>(rErr * A);
                        errorG[offset] += static_cast<int>(gErr * A);
                        errorB[offset] += static_cast<int>(bErr * A);
                    }
                }
            }
            ++acc;
        }
    }
}

void graphics::Color::ditherSierraLite(QImage *qi, int bpp) {
    int r, g, b, acc = 0;
    bpp = 8 - bpp;
    float rErr, gErr, bErr;
    float A = 2.0 / 4.0, B = 1.0 / 4.0;
    int w = qi->width(), h = qi->height(), size = w * h;
    vector <int> errorR(size, 0);
    vector <int> errorG(size, 0);
    vector <int> errorB(size, 0);
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            QColor qc = qi->pixelColor(i, j);
            r = ((qc.red() + errorR[acc]) >> bpp) << bpp;
            g = ((qc.green() + errorG[acc]) >> bpp) << bpp;
            b = ((qc.blue() + errorB[acc]) >> bpp) << bpp;
            r = stdFuncs::clamp(r, 0, 255);
            g = stdFuncs::clamp(g, 0, 255);
            b = stdFuncs::clamp(b, 0, 255);
            rErr = static_cast<float>(qc.red() - r);
            gErr = static_cast<float>(qc.green() - g);
            bErr = static_cast<float>(qc.blue() - b);
            qi->setPixelColor(i, j, QColor(r, g, b, qc.alpha()));
            int offset = acc + 1;
            if (i + 1 < w) {
                errorR[offset] += static_cast<int>(rErr * A);
                errorG[offset] += static_cast<int>(gErr * A);
                errorB[offset] += static_cast<int>(bErr * A);
            }
            if (j + 1 < h) {
                offset += w - 1;
                errorR[offset] += static_cast<int>(rErr * B);
                errorG[offset] += static_cast<int>(gErr * B);
                errorB[offset] += static_cast<int>(bErr * B);
                ++offset;
                if (i + 1 < w) {
                    errorR[offset] += static_cast<int>(rErr * B);
                    errorG[offset] += static_cast<int>(gErr * B);
                    errorB[offset] += static_cast<int>(bErr * B);
                }
            }
            ++acc;
        }
    }
}

void graphics::Color::colorTransfer(QImage *to, QImage from, tType type) {
    float RGB2LMS[3][3] = {{0.3811, 0.5783, 0.0402},
                           {0.1967, 0.7244, 0.0782},
                           {0.0241, 0.1288, 0.8444}};
    float LMS2RGB[3][3] = {{ 4.4679, -3.5873,  0.1193},
                           {-1.2186,  2.3809, -0.1624},
                           { 0.0497, -0.2439, 1.2045}};
    float COMMON[3][3] = {{1.0f / sqrtf(3.0), 0.0, 0.0},
                          {0.0, 1.0f / sqrtf(6.0), 0.0},
                          {0.0, 0.0, 1.0f / sqrtf(2.0)}};
    // transpose for lab2lms
    float TRANS[3][3] = {{1.0, 1.0,  1.0},
                         {1.0, 1.0, -1.0},
                         {1.0, -2.0, 0.0}};
    mat4 rgb2lms(RGB2LMS);
    mat4 lms2rgb(LMS2RGB);
    mat4 common(COMMON);
    mat4 trans(TRANS);
    mat4 lms2lab = common * trans;
    trans.transpose();
    mat4 lab2lms = trans * common;
    vector< vector <vec4> > labImg;
    int wt = to->width(), ht = to->height();
    labImg.resize(wt);
    for (int i = 0; i < wt; ++i)
        labImg[i].resize(ht);
    float lmt = 0.0, amt = 0.0, bmt = 0.0;
    for (int i = 0; i < wt; ++i)
        for (int j = 0; j < ht; ++j) {
            QColor qc = to->pixelColor(i, j);
            vec4 color = vec4(qc.redF(), qc.greenF(), qc.blueF());
            if (type == LAB) {
                color = rgb2lms * color;
                if (color._L == 0.0)
                    color._L = 1.0 / 10000000.0;
                if (color._A == 0.0)
                    color._A = 1.0 / 10000000.0;
                if (color._B == 0.0)
                    color._B = 1.0 / 10000000.0;
                color._L = log10(color._L);
                color._A = log10(color._A);
                color._B = log10(color._B);
                color = lms2lab * color;
            }
            lmt += color._L;
            amt += color._A;
            bmt += color._B;
            labImg[i][j] = color;
        }
    float sizet = static_cast<float>(wt * ht);
    lmt /= sizet;
    amt /= sizet;
    bmt /= sizet;
    float lsdt = 0.0, asdt = 0.0, bsdt = 0.0;
    for (int i = 0; i < wt; ++i)
        for (int j = 0; j < ht; ++j) {
            lsdt += (labImg[i][j]._L - lmt) * (labImg[i][j]._L - lmt);
            asdt += (labImg[i][j]._A - amt) * (labImg[i][j]._A - amt);
            bsdt += (labImg[i][j]._B - bmt) * (labImg[i][j]._B - bmt);
        }
    lsdt = sqrt(lsdt / sizet);
    asdt = sqrt(asdt / sizet);
    bsdt = sqrt(bsdt / sizet);
    vector< vector <vec4> > labImg2;
    int ws = from.width(), hs = from.height();
    labImg2.resize(ws);
    for (int i = 0; i < ws; ++i)
        labImg2[i].resize(hs);
    float lms = 0.0, ams = 0.0, bms = 0.0;
    for (int i = 0; i < ws; ++i)
        for (int j = 0; j < hs; ++j) {
            QColor qc = from.pixelColor(i, j);
            vec4 color = vec4(qc.redF(), qc.greenF(), qc.blueF());
            if (type == LAB) {
                color = rgb2lms * color;
                if (color._L == 0.0)
                    color._L = 1.0 / 10000000.0;
                if (color._A == 0.0)
                    color._A = 1.0 / 10000000.0;
                if (color._B == 0.0)
                    color._B = 1.0 / 10000000.0;
                color._L = log10(color._L);
                color._A = log10(color._A);
                color._B = log10(color._B);
                color = lms2lab * color;
            }
            lms += color._L;
            ams += color._A;
            bms += color._B;
            labImg2[i][j] = color;
        }
    float sizes = static_cast<float>(ws * hs);
    lms /= sizes;
    ams /= sizes;
    bms /= sizes;
    float lsds = 0.0, asds = 0.0, bsds = 0.0;
    for (int i = 0; i < ws; ++i)
        for (int j = 0; j < hs; ++j) {
            lsds += (labImg2[i][j]._L - lms) * (labImg2[i][j]._L - lms);
            asds += (labImg2[i][j]._A - ams) * (labImg2[i][j]._A - ams);
            bsds += (labImg2[i][j]._B - bms) * (labImg2[i][j]._B - bms);
        }
    lsds = sqrt(lsds / sizes);
    asds = sqrt(asds / sizes);
    bsds = sqrt(bsds / sizes);
    double lr = lsds / lsdt, ar = asds / asdt, br = bsds / bsdt;
    for (int i = 0; i < wt; ++i)
        for (int j = 0; j < ht; ++j) {
            labImg[i][j]._L = (labImg[i][j]._L - lmt) * lr + lms;
            labImg[i][j]._A = (labImg[i][j]._A - amt) * ar + ams;
            labImg[i][j]._B = (labImg[i][j]._B - bmt) * br + bms;
            vec4 color = labImg[i][j];
            if (type == LAB) {
                color = lab2lms * color;
                color._L = pow(10.0, color._L);
                color._A = pow(10.0, color._A);
                color._B = pow(10.0, color._B);
                color = lms2rgb * color;
            }
            QColor qc = to->pixelColor(i, j);
            qc.setRedF(stdFuncs::clamp(color._R, 0.0, 1.0));
            qc.setGreenF(stdFuncs::clamp(color._G, 0.0, 1.0));
            qc.setBlueF(stdFuncs::clamp(color._B, 0.0, 1.0));
            to->setPixelColor(i, j, qc);
        }
}

graphics::ImgSupport::ImgSupport() {
    zoom = 1.0;
}

double graphics::ImgSupport::getZoom() {
    return zoom;
}

void graphics::ImgSupport::setZoom(double Zoom) {
    zoom = stdFuncs::clamp(Zoom, graphics::minZoom, graphics::maxZoom);
    zoom = Zoom;
}

void graphics::ImgSupport::zoomIn() {
    setZoom(stdFuncs::clamp(3.0 * zoom / 2.0, graphics::minZoom, graphics::maxZoom));
}

void graphics::ImgSupport::zoomOut() {
    setZoom(stdFuncs::clamp(2.0 * zoom / 3.0, graphics::minZoom, graphics::maxZoom));
}

int graphics::ImgSupport::getSize(double dim, double zoom) {
    return static_cast<int>(dim / zoom + zoom);
}

QImage graphics::ImgSupport::zoomImg(QImage qi) {
    int scale = static_cast<int>(static_cast<double>(qi.height()) * zoom);
    return qi.scaledToHeight(scale);
}

QPoint graphics::ImgSupport::getZoomCorrected(QPoint qp) {
    qp.setX(static_cast<int>(0.5 + static_cast<double>(qp.x()) / zoom) - 1);
    qp.setY(static_cast<int>(0.5 + static_cast<double>(qp.y()) / zoom) - 6);
    return qp;
}

QSize graphics::ImgSupport::getZoomCorrected(QSize qs) {
    return QSize(static_cast<int>(static_cast<float>(qs.width()) * zoom), static_cast<int>(static_cast<float>(qs.height()) * zoom));
}

void graphics::ImgSupport::flipVertical(QImage *qi) {
    int w = qi->width(), h = qi->height();
    int halfW = w / 2;
    --w;
    for (int i = 0; i < halfW; ++i)
        for (int j = 0; j < h; ++j) {
            QRgb qc = qi->pixel(i, j);
            qi->setPixel(i, j, qi->pixel(w - i, j));
            qi->setPixel(w - i, j, qc);
        }
}

void graphics::ImgSupport::flipHorizontal(QImage *qi) {
    int w = qi->width(), h = qi->height();
    int halfH = h / 2;
    --h;
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < halfH; ++j) {
            QRgb qc = qi->pixel(i, j);
            qi->setPixel(i, j, qi->pixel(i, h - j));
            qi->setPixel(i, h - j, qc);
        }
}

KernelData graphics::ImgSupport::loadKernel(string fileName) {
    Kernel kernal, identity;
    identity.push_back(vector <float> ());
    identity[0].push_back(1.0);
    KernelData ret = KernelData (false, identity);
    if (fileName != "") {
        int tKernalSize;
        fstream file;
        bool needGreyscale = false;
        file.open(fileName, ios::in);
        if (file.is_open()) {
            string fromFile;
            if (getline(file, fromFile)) {
                try {
                    tKernalSize = stoi(fromFile);
                    tKernalSize -= 1 - (tKernalSize % 2);
                    needGreyscale = fromFile.find("G") != string::npos;
                }
                catch (...) {
                    return ret;
                }
            }
            else
                return ret;
            if (tKernalSize < 1)
                return ret;
            for (int i = 0; i < tKernalSize; ++i) {
                kernal.push_back(vector <float> ());
                for (int j = 0; j < tKernalSize; ++j)
                    kernal[i].push_back(0.0);
            }
            int lines = 0;
            while(lines < tKernalSize && getline(file, fromFile)) {
                int cnt = 0;
                while (cnt < tKernalSize && fromFile != "") {
                    size_t index = fromFile.find(" ");
                    if (index == string::npos)
                        index = fromFile.length();
                    try {
                        kernal[lines][cnt] = stof(fromFile.substr(0, index));
                    }
                    catch (...) {
                        return ret;
                    }
                    ++cnt;
                    fromFile = index + 1 >= fromFile.length() ? "" : fromFile.substr(index + 1);
                }
                if (cnt < tKernalSize)
                    return ret;
                ++lines;
            }
            file.close();
        }
        ret.first = needGreyscale;
        ret.second = kernal;
        return ret;
    }
    return ret;
}

void graphics::ImgSupport::applyAlpha(QImage *qi, int *yStart, int *yEnd, unsigned int *alpha) {
    int ys = *yStart, ye = *yEnd;
    unsigned int a = *alpha;
    while (ys < ye) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(ys));
        for (int x = 0; x < qi->width(); ++x)
            if (line[x] & 0xFF000000)
                line[x] = a | (line[x] & 0x00FFFFFF);
        ++ys;
        //--*yStart&=++*yStart+++!ys;     // gross
    }
}

QImage graphics::Color::Histogram(QImage *in, eType type) {
    int h = 3 * bins;
    QImage qi (QSize(bins * 2, h), QImage::Format_ARGB32_Premultiplied);
    qi.fill(0xFF000000);
    int end = type == RGB ? 4 : 3;
    int histo[4][bins] = {{0}, {0}, {0}, {0}};
    QImage image = in->copy();
    // Fill the array(s) tht the histograms will be constructed from.
    int total = 0;
    for (int x = 0; x < image.width(); ++x)
        for (int y = 0; y < image.height(); ++y) {
            QColor qc = image.pixelColor(x, y);
            if (qc.alpha() != 0) {
                if (type == RGB) {
                    int intensity = static_cast<int>(static_cast<float>(qc.red() + qc.green() + qc.blue()) / 3.0 + 0.5);
                    ++histo[0][intensity];
                    ++histo[1][qc.red()];
                    ++histo[2][qc.green()];
                    ++histo[3][qc.blue()];
                }
                else if (type == HSV) {
                    ++histo[0][static_cast<int>(255.0 * static_cast<float>(qc.hsvHue() + 1.0) / 360.0)];
                    ++histo[1][static_cast<int>(255.0 * qc.hsvSaturationF())];
                    ++histo[2][static_cast<int>(255.0 * qc.valueF())];
                }
                else {
                    ++histo[0][static_cast<int>(255.0 * static_cast<float>(qc.hslHue() + 1.0) / 360.0)];
                    ++histo[1][static_cast<int>(255.0 * qc.hslSaturationF())];
                    ++histo[2][static_cast<int>(255.0 * qc.lightnessF())];
                }
                ++total;
            }
        }
    int maxI = 0, cutoff = (total) / 4;
    for (int j = 0; j < end; ++j)
        for (int i = 1; i < bins - 1; ++i)
            if (histo[j][i] < cutoff)
                maxI = max(maxI, histo[j][i]);
    maxI = max(maxI, 1);
    // Draw histograms.
    double div = static_cast<double>(h / 2 - 1) / static_cast<double>(maxI);
    float fbins = static_cast<float>(bins - 1);
    for (int x = 0; x < bins; ++x) {
        QRgb value = static_cast<QRgb>(bins + x) / 2;
        for  (int j = 0; j < end; ++j) {
            QRgb color = 0xFF000000;
            if (type == RGB && j != 0)
                color += (value << (8 * (2 - (j - 1))));
            else if (type == HSV && j == 0) {
                QColor qc(color);
                qc.setHsvF(static_cast<float>(x) / fbins, 1.0, 1.0);
                color = qc.rgba();
            }
            else if (type == HSL && j == 0) {
                QColor qc(color);
                qc.setHslF(static_cast<float>(x) / fbins, 1.0, 0.5);
                color = qc.rgba();
            }
            else if (type == HSV && j == 1) {
                QColor qc(color);
                qc.setHsvF(0.3, static_cast<float>(x) / fbins, 1.0);
                color = qc.rgba();
            }
            else if (type == HSL && j == 1) {
                QColor qc(color);
                qc.setHslF(0.3, static_cast<float>(x) / fbins, 0.5);
                color = qc.rgba();
            }
            else if ((type == RGB && j == 0) || (type != RGB && j == 2))
                for (unsigned char i= 0; i < 3; ++i)
                    color += value << (8 * i);
            int rowOffset = (h / 2) * (j / 2);
            int colOffset = x + bins * (j % 2);
            int stop = h / 2 + rowOffset;
            int start = stop - static_cast<int>(static_cast<double>(histo[j][x]) * div);
            start = max(start, rowOffset);
            for (int y = start; y < stop; ++y)
                qi.setPixelColor(colOffset, y, color);
        }
    }
    return qi;
}

void graphics::Color::equalizeHistogramTo(QImage *qi, eType type) {
    vector< vector <vec4> > img;
    int w = qi->width(), h = qi->height();
    img.resize(w);
    for (int i = 0; i < w; ++i)
        img[i].resize(h);
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j) {
            QColor qc = qi->pixelColor(i, j);
            if (type == HSV)
                img[i][j] = vec4(qc.hsvHueF(), qc.hsvSaturationF(), qc.valueF());
            else if (type == HSL)
                img[i][j] = vec4(qc.hslHueF(), qc.hslSaturationF(), qc.lightnessF());
            else
                img[i][j] = vec4(qc.redF(), qc.greenF(), qc.blueF());
        }
    //normalize into 0-255 range
    int histo[bins];
    for (int i = 0; i < bins; ++i)
        histo[i] = 0;
    // Fill the array(s) tht the histograms will be constructed from.
    int value;
    int total = 0;
    for (int x = 0; x < w; ++x)
        for (int y = 0; y < h; ++y) {
            if (qi->pixelColor(x, y).alpha() != 0) {
                float vf;
                if (type == RGB)
                    vf = (img[x][y][0] + img[x][y][1] + img[x][y][2]) / 3.0;
                else
                    vf = img[x][y][2];
                value = static_cast<int>(vf * static_cast<float>(bins - 1) + 0.4);
                value = stdFuncs::clamp(value, 0, bins - 1);
                ++total;
                ++histo[value];
            }
        }
    int i = 0;
    while (i < bins && histo[i] == 0)
        ++i;
    if (i == bins || histo[i] == total)
        return;
    float scale = static_cast<double>(bins - 1) / static_cast<double>(total - histo[i]);
    int lut[bins];
    for (int j = 0; j < bins; ++j)
        lut[j] = 0;
    int sum = 0;
    for (++i; i < bins; ++i) {
        sum += histo[i];
        lut[i] = stdFuncs::clamp(static_cast<int>(static_cast<float>(sum) * scale), 0, 255);
    }
    for (int x = 0; x < w; ++x)
        for (int y = 0; y < h; ++y) {
            if (qi->pixelColor(x, y).alpha() != 0) {
                if (type == HSV || type == HSL) {
                    float vf = img[x][y][2];
                    value = static_cast<int>(vf * static_cast<float>(bins - 1) + 0.4);
                    stdFuncs::clamp(value, 0, bins - 1);
                    vf = static_cast<float>(lut[value]) / 255.0;
                    img[x][y].xyzw[2] = vf;
                }
                if (type == RGB) {
                    int c1 = static_cast<int>(img[x][y][0] * 255.0);
                    int c2 = static_cast<int>(img[x][y][1] * 255.0);
                    int c3 = static_cast<int>(img[x][y][2] * 255.0);
                    img[x][y].set(0, static_cast<float>(lut[c1]) / 255.0);
                    img[x][y].set(1, static_cast<float>(lut[c2]) / 255.0);
                    img[x][y].set(2, static_cast<float>(lut[c3]) / 255.0);
                }
                QColor qc = qi->pixelColor(x, y);
                float a = qc.alphaF();
                if (type == HSV)
                    qc.setHsvF(img[x][y][0], img[x][y][1], img[x][y][2], a);
                else if (type == HSL)
                    qc.setHslF(img[x][y][0], img[x][y][1], img[x][y][2], a);
                else
                    qc.setRgbF(img[x][y][0], img[x][y][1], img[x][y][2], a);
                qi->setPixelColor(x, y, qc);
            }
        }
}

//clipLimit 0.0 to 1.0, 0.0 ahe, 1.0 og, inbetween clahe

void graphics::Color::claheTo(QImage *qi, eType type, float clipLimit, int divisionX, int divisionY) {
    if (clipLimit == 1.0)
        return;
    QImage safe = qi->copy();
    int w = qi->width(), h = qi->height();
    int mi = 256, ma = -1;
    vector< vector<int> > edL;
    edL.resize(w, vector<int>(h, 0));
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j) {
            QColor qc = qi->pixelColor(i, j);
            if (type == RGB)
                edL[i][j] = (qc.red() + qc.green() + qc.blue()) / 3;
            else if (type == HSL)
                edL[i][j] = static_cast<int>(255.0 * qc.lightnessF());
            else if (type == HSV)
                edL[i][j] = static_cast<int>(255.0 * qc.valueF());
            ma = max(ma, edL[i][j]);
            mi = min(mi, edL[i][j]);
        }
    // for automatic mode
    if (divisionX == 0) {
        float sum = 0.0;
        vector< vector<int> > sobel;
        sobel.push_back({-1, -2, -1});
        sobel.push_back({ 0,  0,  0});
        sobel.push_back({ 1,  2,  1});
        double scale = 0.176776695;
        for (int i = 0; i < w; ++i)
            for (int j = 0; j < h; ++j) {
                int totalL_1 = 0, totalL_2 = 0;
                for (int m = 0; m < 3; ++m)
                    for (int n = 0; n < 3; ++n) {
                        if ((m - 1) + i < 0 || (n - 1) + j < 0 || (m - 1) + i >= w || (n - 1) + j >= h) {
                            totalL_1 += sobel[m][n] * edL[i][j];
                            totalL_2 += sobel[n][m] * edL[i][j];
                        }
                        else {
                            totalL_1 += sobel[m][n] * edL[(m - 1) + i][(n - 1) + j];
                            totalL_2 += sobel[n][m] * edL[(m - 1) + i][(n - 1) + j];
                        }
                    }
                sum += sqrt(static_cast<double>(totalL_1 * totalL_1 + totalL_2 * totalL_2)) * scale;
            }
        sum = sqrt(stdFuncs::clamp(1.0 - sum / (255.0 * static_cast<float>(w * h)), 0.0, 1.0));
        if (clipLimit > 0.0)
            divisionX = divisionY = 2 + static_cast<int>(30.0 * sum);
        else
            divisionX = divisionY = 16 + static_cast<int>(112.0 * sum);
    }
    /*   pImage - Pointer to the input/output image
     *   uiNrX - Number of contextial regions in the X direction (min 2, max uiMAX_REG_X)
     *   uiNrY - Number of contextial regions in the Y direction (min 2, max uiMAX_REG_Y)
     *   float fCliplimit - Normalized cliplimit (higher values give more contrast)
     * The number of "effective" greylevels in the output image is set by uiNrBins; selecting
     * a small value (eg. 128) speeds up processing and still produce an output image of
     * good quality. The output image will have the same minimum and maximum value as the input
     * image. A clip limit smaller than 1 results in standard (non-contrast limited) AHE.
     */
    unsigned int uiX, uiY;		  /* counters */
    unsigned int uiXSize, uiYSize, uiSubX, uiSubY; /* size of context. reg. and subimages */
    unsigned int uiXL, uiXR, uiYU, uiYB;  /* auxiliary variables interpolation routine */
    unsigned long ulClipLimit, ulNrPixels;/* clip limit and region pixel count */
    unsigned int aLUT[bins];	    /* lookup table used for scaling of input image */
    unsigned long *pulHist, *pulMapArray; /* pointer to histogram and mappings*/
    unsigned long *pulLU, *pulLB, *pulRU, *pulRB; /* auxiliary pointers interpolation */

    pulMapArray = (unsigned long *)malloc(sizeof(unsigned long) * divisionX * divisionY * bins);
    if (pulMapArray == 0)
        return;	  /* Not enough memory! (try reducing uiNrBins) */

    uiXSize = w / divisionX;
    uiYSize = h / divisionY;  /* Actual size of contextual regions */
    ulNrPixels = (unsigned long)uiXSize * (unsigned long)uiYSize;

    if(clipLimit > 0.0) {		  /* Calculate actual cliplimit	 */
       ulClipLimit = (unsigned long) (clipLimit * (uiXSize * uiYSize) / bins);
       ulClipLimit = (ulClipLimit < 1UL) ? 1UL : ulClipLimit;
    }
    else
        ulClipLimit = 1UL<<14;		  /* Large value, do not clip (AHE) */


    /*

    // Calculate greylevel mappings for each contextual region
    for (uiY = 0, pImPointer = pImage; uiY < divisionY; uiY++) {
        for (uiX = 0; uiX < divisionX; uiX++, pImPointer += uiXSize) {
            pulHist = &pulMapArray[bins * (uiY * divisionX + uiX)];
            MakeHistogram(pImPointer, w, uiXSize, uiYSize, pulHist, bins);
            ClipHistogram(pulHist, bins, ulClipLimit);
            MapHistogram(pulHist, Min, Max, bins, ulNrPixels);
        }
        pImPointer += (uiYSize - 1) * w;		  // skip lines, set pointer
    }

    // Interpolate greylevel mappings to get CLAHE image
    for (pImPointer = pImage, uiY = 0; uiY <= uiNrY; uiY++) {
        if (uiY == 0) {					 // special case: top row
            uiSubY = uiYSize >> 1;  uiYU = 0; uiYB = 0;
        }
        else {
            if (uiY == uiNrY) {				  // special case: bottom row
                uiSubY = (uiYSize+1) >> 1;	uiYU = uiNrY-1;	 uiYB = uiYU;
            }
            else {					  // default values
                uiSubY = uiYSize; uiYU = uiY - 1; uiYB = uiYU + 1;
            }
        }
        for (uiX = 0; uiX <= uiNrX; uiX++) {
            if (uiX == 0) {				   //special case: left column
                uiSubX = uiXSize >> 1; uiXL = 0; uiXR = 0;
            }
            else {
                if (uiX == uiNrX) {			  // special case: right column
                    uiSubX = (uiXSize+1) >> 1;  uiXL = uiNrX - 1; uiXR = uiXL;
                }
                else {					  // default values
                    uiSubX = uiXSize; uiXL = uiX - 1; uiXR = uiXL + 1;
                }
            }

            pulLU = &pulMapArray[bins * (uiYU * uiNrX + uiXL)];
            pulRU = &pulMapArray[bins * (uiYU * uiNrX + uiXR)];
            pulLB = &pulMapArray[bins * (uiYB * uiNrX + uiXL)];
            pulRB = &pulMapArray[bins * (uiYB * uiNrX + uiXR)];
            Interpolate(pImPointer,w,pulLU,pulRU,pulLB,pulRB,uiSubX,uiSubY);
            pImPointer += uiSubX;			  // set pointer on next matrix
        }
        pImPointer += (uiSubY - 1) * w;
    }



    */

}

void graphics::Color::brightnessAdjust(QImage *qi, double val, eType type) {
    if (val != 0.0) {
        QImage processed = qi->copy();
        int w = processed.width(), h = processed.height();
        if (type == RGB)
            val += 1.0;
        for (int i = 0; i < w; ++i)
            for (int j = 0; j < h; ++j) {
                QColor qc = processed.pixelColor(i, j);
                if (type == RGB) {
                    qc.setRedF(stdFuncs::clamp(qc.redF() * val, 0.0, 1.0));
                    qc.setGreenF(stdFuncs::clamp(qc.greenF() * val, 0.0, 1.0));
                    qc.setBlueF(stdFuncs::clamp(qc.blueF() * val, 0.0, 1.0));
                }
                else if (type == HSV)
                    qc.setHsvF(qc.hsvHueF(), qc.hsvSaturationF(), stdFuncs::clamp(qc.valueF() + val, 0.0, 1.0), qc.alphaF());
                else if (type == HSL)
                    qc.setHslF(qc.hslHueF(), qc.hslSaturationF(), stdFuncs::clamp(qc.lightnessF() + val, 0.0, 1.0), qc.alphaF());
                processed.setPixelColor(i, j, qc);
            }
        *qi = processed.copy();
    }
}

void graphics::Color::contrastAdjust(QImage *qi, double val) {
    if (val != 0.0) {
        QImage processed = qi->copy();
        int w = processed.width(), h = processed.height();
        float c = static_cast<float>(val);
        for (int i = 0; i < w; ++i)
            for (int j = 0; j < h; ++j) {
                QColor qc = processed.pixelColor(i, j);
                float f = (259.0 * (c + 255.0)) / (255.0 * (259.0 - c));
                qc.setRedF(stdFuncs::clamp(f * (qc.redF() - 128.0) + 128.0, 0.0, 1.0));
                qc.setGreenF(stdFuncs::clamp(f * (qc.greenF() - 128.0) + 128.0, 0.0, 1.0));
                qc.setBlueF(stdFuncs::clamp(f * (qc.blueF() - 128.0) + 128.0, 0.0, 1.0));
                processed.setPixelColor(i, j, qc);
            }
        *qi = processed.copy();
    }
}

void graphics::Color::saturationAdjust(QImage *qi, double val, eType type) {
    if (val != 0.0) {
        QImage processed = qi->copy();
        int w = processed.width(), h = processed.height();
        val += 1.0;
        for (int i = 0; i < w; ++i)
            for (int j = 0; j < h; ++j) {
                QColor qc = processed.pixelColor(i, j);
                if (type == HSV)
                    qc.setHsvF(qc.hsvHueF(), stdFuncs::clamp(qc.hsvSaturationF() * val, 0.0, 1.0), qc.valueF(), qc.alphaF());
                else if (type == HSL)
                    qc.setHslF(qc.hslHueF(), stdFuncs::clamp(qc.hslSaturationF() * val, 0.0, 1.0), qc.lightnessF(), qc.alphaF());
                processed.setPixelColor(i, j, qc);
            }
        *qi = processed.copy();
    }
}

void graphics::Color::gammaAdjust(QImage *qi, double val) {
    if (val != 0.0) {
        QImage processed = qi->copy();
        int w = processed.width(), h = processed.height();
        float c = static_cast<float>(val) + 1.0;
        for (int i = 0; i < w; ++i)
            for (int j = 0; j < h; ++j) {
                QColor qc = processed.pixelColor(i, j);
                qc.setRedF(c == 0.0 ? 0.0 : stdFuncs::clamp(255.0 * (pow(qc.redF(), 1.0 / c) / pow(255.0, 1.0 / c)), 0.0, 1.0));
                qc.setGreenF(c == 0.0 ? 0.0 : stdFuncs::clamp(255.0 * (pow(qc.greenF(), 1.0 / c) / pow(255.0, 1.0 / c)), 0.0, 1.0));
                qc.setBlueF(c == 0.0 ? 0.0 : stdFuncs::clamp(255.0 * (pow(qc.blueF(), 1.0 / c) / pow(255.0, 1.0 / c)), 0.0, 1.0));
                processed.setPixelColor(i, j, qc);
            }
        *qi = processed.copy();
    }
}

void graphics::Color::hueShift(QImage *qi, int val) {
    if (val != 0) {
        QImage processed = qi->copy();
        int w = processed.width(), h = processed.height();
        for (int i = 0; i < w; ++i)
            for (int j = 0; j < h; ++j) {
                QColor qc = processed.pixelColor(i, j);
                int hue = qc.hslHue() + val;
                if (hue >= 360)
                    hue -= 360;
                qc.setHsl(hue, qc.hslSaturation(), qc.lightness(), qc.alpha());
                processed.setPixelColor(i, j, qc);
            }
        *qi = processed.copy();
    }
}

Kernel graphics::Filtering::getBoxBlur(int radius) {
    int width = radius * 2 + 1;
    float w = static_cast<float>(width * width);
    return Kernel (width, vector<float>(width, 1.0 / w));
}

Kernel graphics::Filtering::getConeBlur(int radius) {
    int width = radius * 2 + 1;
    float r = static_cast<float>(width) / 2.0;
    Kernel vec(width, vector<float>(width, 0.0));
    float cnt = 0.0;
    for (int i = 0; i < width; ++i) {
        int I = i - static_cast<int>(r);
        for (int j = 0; j < width; ++j) {
            int J = j - static_cast<int>(r);
            float dist = sqrt(static_cast<float>(I * I + J * J));
            if (dist <= r) {
                vec[i][j] = (r - dist);
                cnt += vec[i][j];
            }
        }
    }
    for (int i = 0; i < width; ++i)
        for (int j = 0; j < width; ++j)
            vec[i][j] /= cnt;
    return vec;
}

void graphics::Color::matchHistogram(QImage *qi, QImage toMatch, mType type) {
    QImage ret(qi->size(), QImage::Format_ARGB32_Premultiplied);
    double intensities[256] = {0};
    double intensities2[256] = {0};
    double intensities3[256] = {-1};
    ret.fill(0xFF000000);
    int w1 = qi->width(), w2 = toMatch.width(), h1 = qi->height(), h2 = toMatch.height();
    int size = w1 * h1;
    int size2 = w2 * h2;
    int max1 = 0, min1 = 255, max2 = 0, min2 = 255;
    int offset1 = 0, offset2 = 0;
    for (int r = 0; r < h1; r++) {
        for (int c = 0; c < w1; c++) {
            int n;
            QColor qc = qi->pixelColor(c, r);
            if (qc.alpha() == 0) {
                offset1++;
                continue;
            }
            switch (type) {
            case RGB_I:
                n = (qc.red() + qc.green() + qc.blue()) / 3;
                break;
            case HUE:
                n = static_cast<int>(255.0 * qc.hslHueF());
                break;
            case SAT:
                n = static_cast<int>(255.0 * qc.hslSaturationF());
                break;
            case HSV_V:
                n = static_cast<int>(255.0 * qc.valueF());
                break;
            case HSL_L:
                n = static_cast<int>(255.0 * qc.lightnessF());
                break;
            }
            intensities[n] += 1.0;
            max2 = max(max2, n);
            min2 = min(min2, n);
        }
    }
    for (int r = 0; r < h2; r++) {
        for (int c = 0; c < w2; c++) {
            int n;
            QColor qc = toMatch.pixelColor(c, r);
            if (qc.alpha() == 0) {
                offset2++;
                continue;
            }
            switch (type) {
            case RGB_I:
                n = (qc.red() + qc.green() + qc.blue()) / 3;
                break;
            case HUE:
                n = static_cast<int>(255.0 * qc.hslHueF());
                break;
            case SAT:
                n = static_cast<int>(255.0 * qc.hslSaturationF());
                break;
            case HSV_V:
                n = static_cast<int>(255.0 * qc.valueF());
                break;
            case HSL_L:
                n = static_cast<int>(255.0 * qc.lightnessF());
                break;
            }
            intensities2[n] += 1.0;
        }
    }
    for (int i = 1; i < 256; i++) {
        intensities[i] += intensities[i - 1];
        intensities2[i] += intensities2[i - 1];
    }
    for (int i = 0; i < 256; i++) {
        intensities[i] = min(255.0, round(intensities[i] * 255.0 / static_cast<double>(size - offset1)));
        intensities2[i] = min(255.0, round(intensities2[i] * 255.0 / static_cast<double>(size2 - offset2)));
    }
    for(int i = 0; i < 256; i++) {
        int j = 0;
        do {
            intensities3[i] = j;
            j++;
        }
        while(intensities[i] > intensities2[j]);
    }
    for (int i = 0; i < 256; i++) {
        max1 = max(max1, static_cast<int>(intensities3[i]));
        if (intensities3[i] != -1)
            min1 = min(min1, static_cast<int>(intensities3[i]));
    }
    for (int r = 0; r < h1; r++){
        for (int c = 0; c < w1; c++){int n;
            QColor qc = qi->pixelColor(c, r);
            if (qc.alpha() == 0)
                continue;
            switch (type) {
            case RGB_I:
                n = (qc.red() + qc.green() + qc.blue()) / 3;
                break;
            case HUE:
                n = static_cast<int>(255.0 * qc.hslHueF());
                break;
            case SAT:
                n = static_cast<int>(255.0 * qc.hslSaturationF());
                break;
            case HSV_V:
                n = static_cast<int>(255.0 * qc.valueF());
                break;
            case HSL_L:
                n = static_cast<int>(255.0 * qc.lightnessF());
                break;
            }
            int tmp = intensities3[n];
            if (tmp == -1)
                tmp = 0;
//			tmp=((tmp-min1)*(max2-min2)/(max1-min1));

            switch (type) {
            case RGB_I:
                qc.setRed(intensities3[qc.red()]);
                qc.setGreen(intensities3[qc.green()]);
                qc.setBlue(intensities3[qc.blue()]);
                break;
            case HUE:
                qc.setHslF(intensities3[n] / 255.0, qc.hslSaturationF(), qc.lightnessF(), qc.alphaF());
                break;
            case SAT:
                qc.setHslF(qc.hslHueF(), intensities3[n] / 255.0, qc.lightnessF(), qc.alphaF());
                break;
            case HSV_V:
                qc.setHsvF(qc.hsvHueF(), qc.hsvSaturationF(), stdFuncs::clamp(intensities3[n] / 255.0, 0.0, 1.0), qc.alphaF());
                break;
            case HSL_L:
                qc.setHslF(qc.hslHueF(), qc.hslSaturationF(), intensities3[n] / 255.0, qc.alphaF());
                break;
            }
            qi->setPixelColor(c, r, qc);
            //sum+=min((int)tmp,(int)intensities2[(int)image.at<uint8_t>(r,c)]);
//			sum+=tmp;
        }
    }

}
