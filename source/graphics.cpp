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
        for (int x = 0; x < w; ++x) {
            if (line[x] == 0x00000000)
                continue;
            line[x] = filterApplicator(QColor(line[x]), strength);
        }
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

QRgb graphics::Filtering::enlighten(QColor qc, int strength) {
    int combined = (qc.red() + qc.blue() + qc.green()) / 3;
    return combined < strength ? qc.rgba() : (static_cast<QRgb>(qc.alpha()) << 24) | 0x00FFFFFF;
}

QRgb graphics::Filtering::enclose(QColor qc, int strength) {
    int combined = (qc.red() + qc.blue() + qc.green()) / 3;
    strength /= 2;
    return combined < 255 - strength ? (combined > strength ? qc.rgba() : (static_cast<QRgb>(qc.alpha()) << 24)) : (static_cast<QRgb>(qc.alpha()) << 24) | 0x00FFFFFF;
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

QRgb graphics::Filtering::soltNoise(QColor qc, int strength) {
    if (rand() % (2 * strength + 1) == 0)
        return (qc.alpha() << 24) | 0x00FFFFFF;
    return qc.rgba();
}

QRgb graphics::Filtering::pepporNoise(QColor qc, int strength) {
    if (rand() % (2 * strength + 1) == 0)
        return qc.alpha() << 24;
    return qc.rgba();
}

QRgb graphics::Filtering::soltPepporNoise(QColor qc, int strength) {
    if (rand() % (2 * strength + 1) == 0)
        return rand() % 2 ? qc.alpha() << 24 : (qc.alpha() << 24) | 0x00FFFFFF;
    return qc.rgba();
}

QRgb graphics::Filtering::paletteReduce(QColor qc, int strength) {
    int shift = log2(strength + 1);
    qc.setRed((qc.red() >> shift) << shift);
    qc.setGreen((qc.green() >> shift) << shift);
    qc.setBlue((qc.blue() >> shift) << shift);
    return qc.rgba();
}

QRgb graphics::Filtering::redShift(QColor qc, int strength) {
    int shift = log2(strength + 1);
    qc.setRed((qc.red() >> shift) << shift);
    return qc.rgba();
}

QRgb graphics::Filtering::greenShift(QColor qc, int strength) {
    int shift = log2(strength + 1);
    qc.setGreen((qc.green() >> shift) << shift);
    return qc.rgba();
}

QRgb graphics::Filtering::blueShift(QColor qc, int strength) {
    int shift = log2(strength + 1);
    qc.setBlue((qc.blue() >> shift) << shift);
    return qc.rgba();
}

QRgb graphics::Filtering::cyanShift(QColor qc, int strength) {
    int shift = log2(strength + 1);
    qc.setBlue((qc.blue() >> shift) << shift);
    qc.setGreen((qc.green() >> shift) << shift);
    return qc.rgba();
}

QRgb graphics::Filtering::magentaShift(QColor qc, int strength) {
    int shift = log2(strength + 1);
    qc.setRed((qc.red() >> shift) << shift);
    qc.setBlue((qc.blue() >> shift) << shift);
    return qc.rgba();
}

QRgb graphics::Filtering::yellowShift(QColor qc, int strength) {
    int shift = log2(strength + 1);
    qc.setRed((qc.red() >> shift) << shift);
    qc.setGreen((qc.green() >> shift) << shift);
    return qc.rgba();
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
    for (int j = 0; j < qi->height(); ++j) {
        int J = j % matrixSize;
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
        for (int i = 0; i < qi->width(); ++i) {
            int I = i % matrixSize;
            QColor qp = line[i];
            int r = qp.red() + arr[I][J], g = qp.green() + arr[I][J], b = qp.blue() + arr[I][J];
            r = stdFuncs::clamp(r, 0, 255);
            g = stdFuncs::clamp(g, 0, 255);
            b = stdFuncs::clamp(b, 0, 255);
            r = (r >> bitShift) << bitShift;
            g = (g >> bitShift) << bitShift;
            b = (b >> bitShift) << bitShift;
            line[i] = QColor(r, g, b, qp.alpha()).rgba();
        }
    }
}

void graphics::Color::ditherRandom(QImage *qi, int bpp) {
    int bitShift;
    for (int j = 0; j < qi->height(); ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
        for (int i = 0; i < qi->width(); ++i) {
            QColor qp = line[i];
            bitShift = 8 - (bpp - rand() % bpp);
            int r = qp.red(), g = qp.green(), b = qp.blue();
            r = (r >> bitShift) << bitShift;
            g = (g >> bitShift) << bitShift;
            b = (b >> bitShift) << bitShift;
            line[i] = QColor(r, g, b, qp.alpha()).rgba();
        }
    }
}

void graphics::Color::paletteReduction(QImage *qi, int bpp) {
    int bitShift = 8 - bpp;
    for (int j = 0; j < qi->height(); ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
        for (int i = 0; i < qi->width(); ++i) {
            QColor qc = line[i];
            int r = (qc.red() >> bitShift) << bitShift;
            int g = (qc.green() >> bitShift) << bitShift;
            int b = (qc.blue() >> bitShift) << bitShift;
            line[i] = QColor(r, g, b, qc.alpha()).rgba();
        }
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
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
        for (int i = 0; i < w; ++i) {
            QColor qc = line[i];
            r = ((qc.red() + errorR[acc]) >> bpp) << bpp;
            g = ((qc.green() + errorG[acc]) >> bpp) << bpp;
            b = ((qc.blue() + errorB[acc]) >> bpp) << bpp;
            r = stdFuncs::clamp(r, 0, 255);
            g = stdFuncs::clamp(g, 0, 255);
            b = stdFuncs::clamp(b, 0, 255);
            rErr = static_cast<float>(qc.red() - r);
            gErr = static_cast<float>(qc.green() - g);
            bErr = static_cast<float>(qc.blue() - b);
            line[i] = QColor(r, g, b, qc.alpha()).rgba();
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
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
        for (int i = 0; i < w; ++i) {
            QColor qc = line[i];
            r = ((qc.red() + errorR[acc]) >> bpp) << bpp;
            g = ((qc.green() + errorG[acc]) >> bpp) << bpp;
            b = ((qc.blue() + errorB[acc]) >> bpp) << bpp;
            r = stdFuncs::clamp(r, 0, 255);
            g = stdFuncs::clamp(g, 0, 255);
            b = stdFuncs::clamp(b, 0, 255);
            rErr = static_cast<float>(qc.red() - r);
            gErr = static_cast<float>(qc.green() - g);
            bErr = static_cast<float>(qc.blue() - b);
            line[i] = QColor(r, g, b, qc.alpha()).rgba();
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
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
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
            line[i] = QColor(r, g, b, qc.alpha()).rgba();
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
    for (int j = 0; j < ht; ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(to->scanLine(j));
        for (int i = 0; i < wt; ++i) {
            QColor qc = line[i];
            vec4 color;
            if (type == CIELAB)
                color = rgb2lab(qc);
            else if (type == HunterLAB) {
                color = rgb2lms * vec4(qc.redF(), qc.greenF(), qc.blueF());
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
            else
                color = vec4(qc.redF(), qc.greenF(), qc.blueF());
            lmt += color._L;
            amt += color._A;
            bmt += color._B;
            labImg[i][j] = color;
        }
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
    for (int j = 0; j < hs; ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(from.scanLine(j));
        for (int i = 0; i < ws; ++i) {
            QColor qc = line[i];
            vec4 color;
            if (type == CIELAB)
                color = rgb2lab(qc);
            else if (type == HunterLAB) {
                color = rgb2lms * vec4(qc.redF(), qc.greenF(), qc.blueF());
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
            else
                color = vec4(qc.redF(), qc.greenF(), qc.blueF());
            lms += color._L;
            ams += color._A;
            bms += color._B;
            labImg2[i][j] = color;
        }
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
    for (int j = 0; j < ht; ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(to->scanLine(j));
        for (int i = 0; i < wt; ++i) {
            labImg[i][j]._L = (labImg[i][j]._L - lmt) * lr + lms;
            labImg[i][j]._A = (labImg[i][j]._A - amt) * ar + ams;
            labImg[i][j]._B = (labImg[i][j]._B - bmt) * br + bms;
            vec4 color = labImg[i][j];
            if (type == CIELAB)
                color = lab2rgb(color);
            else if (type == HunterLAB) {
                color = lab2lms * color;
                color._L = pow(10.0, color._L);
                color._A = pow(10.0, color._A);
                color._B = pow(10.0, color._B);
                color = lms2rgb * color;
            }
            QColor qc = line[i];
            qc.setRedF(stdFuncs::clamp(color._R, 0.0, 1.0));
            qc.setGreenF(stdFuncs::clamp(color._G, 0.0, 1.0));
            qc.setBlueF(stdFuncs::clamp(color._B, 0.0, 1.0));
            line[i] = qc.rgba();
        }
    }
}

vec4 graphics::Color::rgb2lab(QColor qc) {
    // D65/2°
    float xyzRef[3] = {95.047f, 100.0f, 108.883f};
    //D50 uses  96.4212   100.0   82.5188
    // rgb to xyz
    float rgb[3] = {tof(qc.redF()), tof(qc.greenF()), tof(qc.blueF())};
    for (unsigned char i = 0; i < 3; ++i) {
        if (rgb[i] > 0.04045f)
            rgb[i] = pow((rgb[i] + 0.055f) / 1.055f, 2.4f);
        else
            rgb[i] /= 12.92f;
        rgb[i] *= 100.0f;
    }
    float xyz[3] = {0.0f};
    xyz[0] = 0.4124f * rgb[0] + 0.3576f * rgb[1] + 0.1805f * rgb[2];
    xyz[1] = 0.2126f * rgb[0] + 0.7152f * rgb[1] + 0.0722f * rgb[2];
    xyz[2] = 0.0193f * rgb[0] + 0.1192f * rgb[1] + 0.9505f * rgb[2];
    // xyz to lab
    for (unsigned char i = 0; i < 3; ++i) {
        xyz[i] /= xyzRef[i];
        if (xyz[i] > 0.008856f)
            xyz[i] = pow(xyz[i], 1.0f / 3.0f);
        else
            xyz[i] = (7.787f * xyz[i]) + 16.0f / 116.0f;
    }
    return vec4({116.0f * xyz[1] - 16.0f, 500.0f * (xyz[0] - xyz[1]), 200.0f * (xyz[1] - xyz[2])});
}

vec4 graphics::Color::getLabScaled(vec4 lab) {
    vec4 ret = lab;
    for (unsigned char i = 0; i < 3; ++i)
        ret.xyzw[i] = scalesLAB[i] * (ret[i] - minsLAB[i]);
    return ret;
}

vec4 graphics::Color::getLabDescaled(vec4 lab) {
    vec4 ret = lab;
    for (unsigned char i = 0; i < 3; ++i)
        ret.xyzw[i] = ret[i] / scalesLAB[i] + minsLAB[i];
    return ret;
}

vec4 graphics::Color::lab2rgb(vec4 lab) {
    // D65/2°
    float xyzRef[3] = {95.047f, 100.0f, 108.883f};
    //D50 uses  96.4212   100.0   82.5188
    // lab to xyz
    float xyz[3] = {0.0f};
    xyz[1] = (lab[0] + 16.0f) / 116.0f;
    xyz[0] = lab[1] / 500.0f + xyz[1];
    xyz[2] = xyz[1] - lab[2] / 200.0f;
    for (unsigned char i = 0; i < 3; ++i) {
        if (pow(xyz[i], 3.0f) > 0.008856f)
            xyz[i] = pow(xyz[i], 3.0f);
        else
            xyz[i] = (xyz[i] - 16.0f / 116.0f) / 7.787f;
        xyz[i] *= xyzRef[i];
    }
    // xyz to rgb
    float rgb[3] = {0.0f};
    rgb[0] = xyz[0] *  3.2406f + xyz[1] * -1.5372f + xyz[2] * -0.4986f;
    rgb[1] = xyz[0] * -0.9689f + xyz[1] *  1.8758f + xyz[2] *  0.0415f;
    rgb[2] = xyz[0] *  0.0557f + xyz[1] * -0.2040f + xyz[2] *  1.0570f;
    for (unsigned char i = 0; i < 3; ++i) {
        rgb[i] /= 100.0f;
        if (rgb[i] > 0.0031308f)
            rgb[i] = 1.055f * pow(rgb[i], 1.0f / 2.4f) - 0.055f;
        else
            rgb[i] *= 12.92f;
    }
    return rgb;
}

QColor graphics::Color::toQColor(vec4 rgb) {
    QColor qc;
    qc.setRgbF(stdFuncs::clamp(rgb[0], 0.0, 1.0), stdFuncs::clamp(rgb[1], 0.0, 1.0), stdFuncs::clamp(rgb[2], 0.0, 1.0));
    return qc;
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
    for (int j = 0; j < h; ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
        for (int i = 0; i < halfW; ++i) {
            QRgb temp = line[i];
            line[i] = line[w - i];
            line[w - i] = temp;
        }
    }
}

void graphics::ImgSupport::flipHorizontal(QImage *qi) {
    int w = qi->width(), h = qi->height();
    int halfH = h / 2;
    --h;
    for (int j = 0; j < halfH; ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
        QRgb *line2 = reinterpret_cast<QRgb *>(qi->scanLine(h - j));
        for (int i = 0; i < w; ++i) {
            QRgb temp = line[i];
            line[i] = line2[i];
            line2[i] = temp;
        }
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

void graphics::ImgSupport::applyAlpha(QImage *qi, int yStart, int yEnd, unsigned int alpha) {
    int ys = yStart;
    while (ys < yEnd) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(ys));
        for (int x = 0; x < qi->width(); ++x)
            if (line[x] & 0xFF000000)
                line[x] = alpha | (line[x] & 0x00FFFFFF);
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
    for (int y = 0; y < image.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            QColor qc = line[x];
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
                else if (type == HSL) {
                    ++histo[0][static_cast<int>(255.0 * static_cast<float>(qc.hslHue() + 1.0) / 360.0)];
                    ++histo[1][static_cast<int>(255.0 * qc.hslSaturationF())];
                    ++histo[2][static_cast<int>(255.0 * qc.lightnessF())];
                }
                else if (type == LAB) {
                    vec4 color = getLabScaled(rgb2lab(qc));
                    for (int i = 0; i < 3; ++i)
                        ++histo[i][static_cast<int>(color[i])];
                }
                ++total;
            }
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
            else if (type == LAB && j != 0) {
                if (j == 1)
                    color = QColor(x, 255 - x, 0).rgba();
                else if (j == 2)
                    color = QColor (255 - x, 255 - x, x).rgba();
            }
            else if (((type == RGB || type == LAB) && j == 0) || (type != RGB && j == 2))
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
    int w = qi->width(), h = qi->height();
    vector< vector <vec4> > img(w, vector <vec4> (h, vec4()));
    //normalize into 0-255 range
    int histo[bins] = {0};
    // Fill the array(s) tht the histograms will be constructed from.
    int value;
    int total = 0;
    for (int j = 0; j < h; ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
        for (int i = 0; i < w; ++i) {
            QColor qc = line[i];
            if (qc.alpha() != 0) {
                float vf;
                if (type == HSV) {
                    img[i][j] = vec4(qc.hsvHueF(), qc.hsvSaturationF(), qc.valueF());
                    vf = img[i][j][2];
                }
                else if (type == HSL) {
                    img[i][j] = vec4(qc.hslHueF(), qc.hslSaturationF(), qc.lightnessF());
                    vf = img[i][j][2];
                }
                else if (type == RGB) {
                    img[i][j] = vec4(qc.redF(), qc.greenF(), qc.blueF());
                    vf = (img[i][j][0] + img[i][j][1] + img[i][j][2]) / 3.0;
                }
                else {
                    img[i][j] = getLabScaled(rgb2lab(qc));
                    img[i][j] /= 255.0;
                    vf = img[i][j][0];
                }
                value = static_cast<int>(vf * static_cast<float>(bins - 1) + 0.4);
                value = stdFuncs::clamp(value, 0, bins - 1);
                ++total;
                ++histo[value];
            }
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
    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(y));
        for (int x = 0; x < w; ++x) {
            QColor qc = line[i];
            float a = qc.alphaF();
            if (qc.alpha() != 0) {
                if (type == HSV || type == HSL) {
                    float vf = img[x][y][2];
                    value = static_cast<int>(vf * static_cast<float>(bins - 1) + 0.4);
                    stdFuncs::clamp(value, 0, bins - 1);
                    vf = static_cast<float>(lut[value]) / 255.0;
                    img[x][y].xyzw[2] = vf;
                    if (type == HSV)
                        qc.setHsvF(img[x][y][0], img[x][y][1], img[x][y][2], a);
                    else
                        qc.setHslF(img[x][y][0], img[x][y][1], img[x][y][2], a);
                }
                if (type == RGB) {
                    int c1 = static_cast<int>(img[x][y][0] * 255.0);
                    int c2 = static_cast<int>(img[x][y][1] * 255.0);
                    int c3 = static_cast<int>(img[x][y][2] * 255.0);
                    img[x][y].set(0, static_cast<float>(lut[c1]) / 255.0);
                    img[x][y].set(1, static_cast<float>(lut[c2]) / 255.0);
                    img[x][y].set(2, static_cast<float>(lut[c3]) / 255.0);
                    qc.setRgbF(img[x][y][0], img[x][y][1], img[x][y][2], a);
                }
                if (type == LAB) {
                    float vf = img[x][y][0];
                    value = static_cast<int>(vf * static_cast<float>(bins - 1) + 0.4);
                    stdFuncs::clamp(value, 0, bins - 1);
                    vf = static_cast<float>(lut[value]) / 255.0;
                    img[x][y].xyzw[0] = vf;
                    img[x][y] *= 255.0;
                    qc = toQColor(lab2rgb(getLabDescaled(img[x][y])));
                    qc.setAlphaF(a);
                }
                line[x] = qc.rgba();
            }
        }
    }
}

//clipLimit 0.0 to 1.0, 0.0 ahe, 1.0 og, inbetween clahe

void graphics::Color::claheTo(QImage *qi, eType type, float clipLimit, int divisionX, int divisionY) {
    QImage safe = qi->copy();
    int w = qi->width(), h = qi->height();
    int mi = 256, ma = -1;
    vector< vector<int> > edL;
    edL.resize(w, vector<int>(h, 0));
    for (int j = 0; j < h; ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
        for (int i = 0; i < w; ++i) {
            QColor qc = line[i];
            if (type == HSL || type == RGB)
                edL[i][j] = static_cast<int>(255.0 * qc.lightnessF());
            else if (type == HSV)
                edL[i][j] = static_cast<int>(255.0 * qc.valueF());
            else if (type == HSV)
                edL[i][j] = static_cast<int>(255.0 * qc.valueF());
            else if (type == LAB)
                edL[i][j] = static_cast<int>(getLabScaled(rgb2lab(qc)).xyzw[0]);
            ma = max(ma, edL[i][j]);
            mi = min(mi, edL[i][j]);
        }
    }
    // for automatic mode
    if (divisionX < 2 || divisionY < 2) {
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
        sum = pow(stdFuncs::clamp(sum / (32.0 * static_cast<float>(w * h)), 0.0, 1.0), 2);
        int divs = clipLimit > 0.0 ? 2 + static_cast<int>(64.0 * sum / clipLimit) : 2;
        if (divisionX < 2)
            divisionX = divs;
        if (divisionY < 2)
            divisionY = divs;
    }
    int offX = 0, offY = 0;
    if (safe.width() % divisionX != 0 || safe.height() % divisionY != 0) {
        int divX = (static_cast<int>(safe.width() / divisionX) + 1) * divisionX;
        int divY = (static_cast<int>(safe.height() / divisionY) + 1) * divisionY;
        safe = safe.scaled(divX, divY, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        offX = (divX - qi->width()) / 2;
        offY = (divY - qi->height()) / 2;
        for (int j = 0; j < qi->height(); ++j) {
            QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
            QRgb *line2 = reinterpret_cast<QRgb *>(safe.scanLine(j + offY));
            for (int i = 0; i < qi->width(); ++i)
                line2[i + offX] = line[i];
        }
    }
    unsigned int uiX, uiY;		  /* counters */
    unsigned int uiXSize, uiYSize, uiSubX, uiSubY; /* size of context. reg. and subimages */
    unsigned int uiXL, uiXR, uiYU, uiYB;  /* auxiliary variables interpolation routine */
    unsigned long ulClipLimit, ulNrPixels;/* clip limit and region pixel count */
    unsigned char *pImPointer, *pImage;		   /* pointer to image */
    unsigned char aLUT[bins];	    /* lookup table used for scaling of input image */
    unsigned long *pulHist, *pulMapArray; /* pointer to histogram and mappings*/
    unsigned long* pulLU, *pulLB, *pulRU, *pulRB; /* auxiliary pointers interpolation */
    uiXSize = safe.width() / divisionX;
    uiYSize = safe.height() / divisionY;  /* Actual size of contextual regions */
    ulNrPixels = static_cast<unsigned long>(uiXSize) * static_cast<unsigned long>(uiYSize);
    pulMapArray = (unsigned long *) malloc(sizeof(unsigned long) * divisionX * divisionY * bins);
    pImage = (unsigned char *) malloc(sizeof(unsigned char) * safe.width() * safe.height());
    long cntr = 0;
    for (int j = 0; j < safe.height(); ++j) {
        QRgb *line2 = reinterpret_cast<QRgb *>(safe.scanLine(j));
        for (int i = 0; i < safe.width(); ++i) {
            QColor qc = line2[i];
            if (type == HSL || type == RGB)
                pImage[cntr] = static_cast<unsigned char>(255.0 * qc.lightnessF());
            else if (type == HSV)
                pImage[cntr] = static_cast<unsigned char>(255.0 * qc.valueF());
            else if (type == LAB)
                pImage[cntr] = static_cast<unsigned char>(getLabScaled(rgb2lab(qc)).xyzw[0]);
            ++cntr;
        }
    }
    --cntr;
    if (clipLimit > 0.0) {		  /* Calculate actual cliplimit	 */
       ulClipLimit = static_cast<unsigned long>(clipLimit * static_cast<float>(uiXSize * uiYSize) / static_cast<float>(bins));
       if (ulClipLimit < 1UL)
           ulClipLimit = 1UL;
    }
    else
        ulClipLimit = 1UL << 14;		  /* Large value, do not clip (AHE) */
    for (int i = 0; i < bins; ++i)
        aLUT[i] = i - mi;
    /* Calculate greylevel mappings for each contextual region */
    for (uiY = 0, pImPointer = pImage; uiY < divisionY; uiY++) {
        for (uiX = 0; uiX < divisionX; uiX++, pImPointer += uiXSize) {
            pulHist = &pulMapArray[bins * (uiY * divisionX + uiX)];
            MakeHistogram(pImPointer, safe.width(), uiXSize, uiYSize, pulHist, aLUT);
            ClipHistogram(pulHist, ulClipLimit);
            MapHistogram(pulHist, mi, ma, ulNrPixels);
        }
        pImPointer += (uiYSize - 1) * safe.width();		  /* skip lines, set pointer */
    }
    /* Interpolate greylevel mappings to get CLAHE image */
    for (pImPointer = pImage, uiY = 0; uiY <= divisionY; uiY++) {
        if (uiY == 0) {					  /* special case: top row */
            uiSubY = uiYSize >> 1;
            uiYU = 0;
            uiYB = 0;
        }
        else {
            if (uiY == divisionY) {				  /* special case: bottom row */
                uiSubY = (uiYSize+1) >> 1;
                uiYU = divisionY - 1;
                uiYB = uiYU;
            }
            else {					  /* default values */
                uiSubY = uiYSize;
                uiYU = uiY - 1;
                uiYB = uiYU + 1;
            }
        }
        for (uiX = 0; uiX <= divisionX; uiX++) {
            if (uiX == 0) {				  /* special case: left column */
                uiSubX = uiXSize >> 1;
                uiXL = 0;
                uiXR = 0;
            }
            else {
                if (uiX == divisionX) {			  /* special case: right column */
                    uiSubX = (uiXSize+1) >> 1;
                    uiXL = divisionX - 1;
                    uiXR = uiXL;
                }
                else {					  /* default values */
                    uiSubX = uiXSize;
                    uiXL = uiX - 1;
                    uiXR = uiXL + 1;
                }
            }
            pulLU = &pulMapArray[bins * (uiYU * divisionX + uiXL)];
            pulRU = &pulMapArray[bins * (uiYU * divisionX + uiXR)];
            pulLB = &pulMapArray[bins * (uiYB * divisionX + uiXL)];
            pulRB = &pulMapArray[bins * (uiYB * divisionX + uiXR)];
            Interpolate(pImPointer, safe.width(), pulLU, pulRU, pulLB, pulRB, uiSubX, uiSubY, aLUT);
            pImPointer += uiSubX;			  /* set pointer on next matrix */
        }
        pImPointer += (uiSubY - 1) * safe.width();
    }
    while(cntr >= 0) {
        QRgb *line2 = reinterpret_cast<QRgb *>(safe.scanLine(cntr / safe.width()));
        for (int i = safe.width() - 1; i >= 0; --i) {
            QColor qc(line2[i]);
            if (type == RGB || type == HSL)
                qc.setHslF(qc.hslHueF(), qc.hslSaturationF(), static_cast<float>(pImage[cntr]) / 255.0);
            else if (type == HSV)
                qc.setHsvF(qc.hsvHueF(), qc.hsvSaturationF(), static_cast<float>(pImage[cntr]) / 255.0);
            else if (type == LAB) {
                vec4 color = getLabScaled(rgb2lab(qc));
                color.xyzw[0] = pImage[cntr];
                qc = toQColor(lab2rgb(getLabDescaled(color)));
            }
            line2[i] = qc.rgba();
            --cntr;
        }
    }
    for (int j = 0; j < qi->height(); ++j) {
        QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(j));
        QRgb *line2 = reinterpret_cast<QRgb *>(safe.scanLine(j + offY));
        for (int i = 0; i < qi->width(); ++i)
            line[i] = line2[i + offX];
    }
    free(pulMapArray);					  /* free space for histograms */
    free(pImage);					  /* free space for histograms */
}

void graphics::Color::ClipHistogram (unsigned long* pulHistogram, unsigned long ulClipLimit)
/* This function performs clipping of the histogram and redistribution of bins.
 * The histogram is clipped and the number of excess pixels is counted. Afterwards
 * the excess pixels are equally redistributed across the whole histogram (providing
 * the bin count is smaller than the cliplimit).
 */
{
    unsigned long* pulBinPointer, *pulEndPointer, *pulHisto;
    unsigned long ulNrExcess, ulUpper, ulBinIncr, ulStepSize, i;
    long lBinExcess;

    ulNrExcess = 0;
    pulBinPointer = pulHistogram;
    for (i = 0; i < bins; i++) { /* calculate total number of excess pixels */
        lBinExcess = (long) pulBinPointer[i] - (long) ulClipLimit;
        if (lBinExcess > 0)
            ulNrExcess += lBinExcess;	  /* excess in current bin */
    }

    /* Second part: clip histogram and redistribute excess pixels in each bin */
    ulBinIncr = ulNrExcess / bins;		  /* average binincrement */
    ulUpper =  ulClipLimit - ulBinIncr;	 /* Bins larger than ulUpper set to cliplimit */

    for (i = 0; i < bins; i++) {
      if (pulHistogram[i] > ulClipLimit)
          pulHistogram[i] = ulClipLimit; /* clip bin */
      else {
          unsigned long temp = ulNrExcess;
          if (pulHistogram[i] > ulUpper) {		/* high bin count */
              ulNrExcess -= pulHistogram[i] - ulUpper;
              pulHistogram[i] = ulClipLimit;
          }
          else {					/* low bin count */
              ulNrExcess -= ulBinIncr;
              pulHistogram[i] += ulBinIncr;
          }
          if (temp < ulNrExcess)
              ulNrExcess = 0;
       }
    }
    while (ulNrExcess) {   /* Redistribute remaining excess  */
        pulEndPointer = &pulHistogram[bins];
        pulHisto = pulHistogram;
        unsigned long loopFault = ulNrExcess;
        while (ulNrExcess && pulHisto < pulEndPointer) {
            ulStepSize = bins / ulNrExcess;
            if (ulStepSize < 1)
                ulStepSize = 1;		  /* stepsize at least 1 */
            for (pulBinPointer = pulHisto; pulBinPointer < pulEndPointer && ulNrExcess; pulBinPointer += ulStepSize) {
                if (*pulBinPointer < ulClipLimit) {
                    (*pulBinPointer)++;
                    ulNrExcess--;	  /* reduce excess */
                }
            }
            pulHisto++;		  /* restart redistributing on other bin location */
        }
        if (loopFault == ulNrExcess)
            break;
    }
}

void graphics::Color::MakeHistogram (unsigned char * pImage, unsigned int uiXRes, unsigned int uiSizeX, unsigned int uiSizeY, unsigned long* pulHistogram, unsigned char* pLookupTable)
/* This function classifies the greylevels present in the array image into
 * a greylevel histogram. The pLookupTable specifies the relationship
 * between the greyvalue of the pixel (typically between 0 and 4095) and
 * the corresponding bin in the histogram (usually containing only 128 bins).
 */
{
    unsigned char* pImagePointer;
    unsigned int i;

    for (i = 0; i < bins; i++) pulHistogram[i] = 0L; /* clear histogram */

    for (i = 0; i < uiSizeY; i++) {
        pImagePointer = &pImage[uiSizeX];
        while (pImage < pImagePointer) pulHistogram[pLookupTable[*pImage++]]++;
        pImagePointer += uiXRes;
        pImage = &pImagePointer[-(int)uiSizeX];	/* go to bdeginning of next row */
    }
}

void graphics::Color::MapHistogram (unsigned long* pulHistogram, unsigned char Min, unsigned char Max, unsigned long ulNrOfPixels)
/* This function calculates the equalized lookup table (mapping) by
 * cumulating the input histogram. Note: lookup table is rescaled in range [Min..Max].
 */
{
    unsigned int i;  unsigned long ulSum = 0;
    const float fScale = ((float)(Max - Min)) / ulNrOfPixels;
    const unsigned long ulMin = (unsigned long) Min;

    for (i = 0; i < bins; i++) {
        ulSum += pulHistogram[i]; pulHistogram[i]=(unsigned long)(ulMin+ulSum*fScale);
        if (pulHistogram[i] > Max) pulHistogram[i] = Max;
    }
}

void graphics::Color::Interpolate (unsigned char * pImage, int uiXRes, unsigned long * pulMapLU, unsigned long * pulMapRU, unsigned long * pulMapLB,  unsigned long * pulMapRB, unsigned int uiXSize, unsigned int uiYSize, unsigned char * pLUT)
/* pImage      - pointer to input/output image
 * uiXRes      - resolution of image in x-direction
 * pulMap*     - mappings of greylevels from histograms
 * uiXSize     - uiXSize of image submatrix
 * uiYSize     - uiYSize of image submatrix
 * pLUT	       - lookup table containing mapping greyvalues to bins
 * This function calculates the new greylevel assignments of pixels within a submatrix
 * of the image with size uiXSize and uiYSize. This is done by a bilinear interpolation
 * between four different mappings in order to eliminate boundary artifacts.
 * It uses a division; since division is often an expensive operation, I added code to
 * perform a logical shift instead when feasible.
 */
{
    const unsigned int uiIncr = uiXRes-uiXSize; /* Pointer increment after processing row */
    unsigned char GreyValue; unsigned int uiNum = uiXSize*uiYSize; /* Normalization factor */

    unsigned int uiXCoef, uiYCoef, uiXInvCoef, uiYInvCoef, uiShift = 0;

    if (uiNum & (uiNum - 1))   /* If uiNum is not a power of two, use division */
        for (uiYCoef = 0, uiYInvCoef = uiYSize; uiYCoef < uiYSize;
         uiYCoef++, uiYInvCoef--,pImage+=uiIncr) {
        for (uiXCoef = 0, uiXInvCoef = uiXSize; uiXCoef < uiXSize;
             uiXCoef++, uiXInvCoef--) {
            GreyValue = pLUT[*pImage];		   /* get histogram bin value */
            *pImage++ = (unsigned char) ((uiYInvCoef * (uiXInvCoef*pulMapLU[GreyValue]
                          + uiXCoef * pulMapRU[GreyValue])
                    + uiYCoef * (uiXInvCoef * pulMapLB[GreyValue]
                          + uiXCoef * pulMapRB[GreyValue])) / uiNum);
        }
    }
    else {			   /* avoid the division and use a right shift instead */
        while (uiNum >>= 1) uiShift++;		   /* Calculate 2log of uiNum */
        for (uiYCoef = 0, uiYInvCoef = uiYSize; uiYCoef < uiYSize;
             uiYCoef++, uiYInvCoef--,pImage+=uiIncr) {
             for (uiXCoef = 0, uiXInvCoef = uiXSize; uiXCoef < uiXSize;
               uiXCoef++, uiXInvCoef--) {
               GreyValue = pLUT[*pImage];	  /* get histogram bin value */
               *pImage++ = (unsigned char)((uiYInvCoef* (uiXInvCoef * pulMapLU[GreyValue]
                          + uiXCoef * pulMapRU[GreyValue])
                    + uiYCoef * (uiXInvCoef * pulMapLB[GreyValue]
                          + uiXCoef * pulMapRB[GreyValue])) >> uiShift);
            }
        }
    }
}

void graphics::Color::brightnessAdjust(QImage *qi, double val, eType type) {
    if (val != 0.0) {
        QImage processed = qi->copy();
        int w = processed.width(), h = processed.height();
        if (type == RGB)
            val += 1.0;
        for (int j = 0; j < h; ++j) {
            QRgb *line = reinterpret_cast<QRgb *>(processed.scanLine(j));
            for (int i = 0; i < w; ++i) {
                if (line[i] == 0x00000000)
                    continue;
                QColor qc = line[i];
                float alpha = qc.alphaF();
                if (type == RGB) {
                    qc.setRedF(stdFuncs::clamp(qc.redF() * val, 0.0, 1.0));
                    qc.setGreenF(stdFuncs::clamp(qc.greenF() * val, 0.0, 1.0));
                    qc.setBlueF(stdFuncs::clamp(qc.blueF() * val, 0.0, 1.0));
                }
                else if (type == HSV)
                    qc.setHsvF(qc.hsvHueF(), qc.hsvSaturationF(), stdFuncs::clamp(qc.valueF() + val, 0.0, 1.0), qc.alphaF());
                else if (type == HSL)
                    qc.setHslF(qc.hslHueF(), qc.hslSaturationF(), stdFuncs::clamp(qc.lightnessF() + val, 0.0, 1.0), qc.alphaF());
                else if (type == LAB) {
                    vec4 color = rgb2lab(qc);
                    color.xyzw[0] += 100.0 * val;
                    qc = toQColor(lab2rgb(color));
                }
                qc.setAlphaF(alpha);
                line[i] = qc.rgba();
            }
        }
        *qi = processed.copy();
    }
}

void graphics::Color::contrastAdjust(QImage *qi, double val) {
    if (val != 0.0) {
        QImage processed = qi->copy();
        int w = processed.width(), h = processed.height();
        float c = static_cast<float>(val);
        for (int j = 0; j < h; ++j) {
            QRgb *line = reinterpret_cast<QRgb *>(processed.scanLine(j));
            for (int i = 0; i < w; ++i) {
                if (line[i] == 0x00000000)
                    continue;
                QColor qc = line[i];
                float f = (259.0 * (c + 255.0)) / (255.0 * (259.0 - c));
                qc.setRedF(stdFuncs::clamp(f * (qc.redF() - 128.0) + 128.0, 0.0, 1.0));
                qc.setGreenF(stdFuncs::clamp(f * (qc.greenF() - 128.0) + 128.0, 0.0, 1.0));
                qc.setBlueF(stdFuncs::clamp(f * (qc.blueF() - 128.0) + 128.0, 0.0, 1.0));
                line[i] = qc.rgba();
            }
        }
        *qi = processed.copy();
    }
}

void graphics::Color::saturationAdjust(QImage *qi, double val, eType type) {
    if (val != 0.0) {
        QImage processed = qi->copy();
        int w = processed.width(), h = processed.height();
        val += 1.0;
        for (int j = 0; j < h; ++j) {
            QRgb *line = reinterpret_cast<QRgb *>(processed.scanLine(j));
            for (int i = 0; i < w; ++i) {
                QColor qc = line[i];
                if (line[i] == 0x00000000)
                    continue;
                if (type == HSV)
                    qc.setHsvF(qc.hsvHueF(), stdFuncs::clamp(qc.hsvSaturationF() * val, 0.0, 1.0), qc.valueF(), qc.alphaF());
                else if (type == HSL)
                    qc.setHslF(qc.hslHueF(), stdFuncs::clamp(qc.hslSaturationF() * val, 0.0, 1.0), qc.lightnessF(), qc.alphaF());
                line[i] = qc.rgba();
            }
        }
        *qi = processed.copy();
    }
}

void graphics::Color::gammaAdjust(QImage *qi, double val) {
    if (val != 0.0) {
        QImage processed = qi->copy();
        int w = processed.width(), h = processed.height();
        float c = static_cast<float>(val) + 1.0;
        for (int j = 0; j < h; ++j) {
            QRgb *line = reinterpret_cast<QRgb *>(processed.scanLine(j));
            for (int i = 0; i < w; ++i) {
                if (line[i] == 0x00000000)
                    continue;
                QColor qc = line[i];
                qc.setRedF(c == 0.0 ? 0.0 : stdFuncs::clamp(255.0 * (pow(qc.redF(), 1.0 / c) / pow(255.0, 1.0 / c)), 0.0, 1.0));
                qc.setGreenF(c == 0.0 ? 0.0 : stdFuncs::clamp(255.0 * (pow(qc.greenF(), 1.0 / c) / pow(255.0, 1.0 / c)), 0.0, 1.0));
                qc.setBlueF(c == 0.0 ? 0.0 : stdFuncs::clamp(255.0 * (pow(qc.blueF(), 1.0 / c) / pow(255.0, 1.0 / c)), 0.0, 1.0));
                line[i] = qc.rgba();
            }
        }
        *qi = processed.copy();
    }
}

void graphics::Color::hueShift(QImage *qi, int val) {
    if (val != 0) {
        QImage processed = qi->copy();
        int w = processed.width(), h = processed.height();
        for (int j = 0; j < h; ++j) {
            QRgb *line = reinterpret_cast<QRgb *>(processed.scanLine(j));
            for (int i = 0; i < w; ++i) {
                if (line[i] == 0x00000000)
                    continue;
                QColor qc = line[i];
                int hue = qc.hslHue() + val;
                if (hue >= 360)
                    hue -= 360;
                qc.setHsl(hue, qc.hslSaturation(), qc.lightness(), qc.alpha());
                line[i] = qc.rgba();
            }
        }
        *qi = processed.copy();
    }
}

Kernel graphics::Filtering::getBoxBlur(int radius) {
    int width = radius * 2 + 1;
    float w = static_cast<float>(width * width);
    return Kernel (width, vector<float>(width, 1.0 / w));
}

void graphics::Filtering::medianFilter(QImage *qi, int radius, eType type) {
    QImage img = qi->copy();
    int h = qi->height(), w = qi->width();
    for (int j = 0; j < h; ++j) {
        QRgb* line2 = reinterpret_cast<QRgb *>(qi->scanLine(j));
        for (int i = 0; i < w; ++i) {
            QColor qc = line2[i];
            vector <int> nums;
            int jStart = j - radius;
            int jEnd = j + radius;
            int iStart = i - radius;
            int iEnd = i + radius;
            if (jStart < 0) {
                int val;
                if (type == HSV)
                    val = qc.value();
                else if (type == HSL || type == RGB)
                    val = qc.lightness();
                else if (type == LAB)
                    val = Color::getLabScaled(Color::rgb2lab(qc))[0];
                nums.insert(nums.end(), (-jStart) * w, val);
                jStart = 0;
            }
            if (jEnd >= h) {
                int val;
                if (type == HSV)
                    val = qc.value();
                else if (type == HSL || type == RGB)
                    val = qc.lightness();
                else if (type == LAB)
                    val = Color::getLabScaled(Color::rgb2lab(qc))[0];
                nums.insert(nums.end(), (jEnd - h) * w, val);
                jEnd = h;
            }
            if (iStart < 0) {
                int val;
                if (type == HSV)
                    val = qc.value();
                else if (type == HSL || type == RGB)
                    val = qc.lightness();
                else if (type == LAB)
                    val = Color::getLabScaled(Color::rgb2lab(qc))[0];
                nums.insert(nums.end(), -iStart, val);
                iStart = 0;
            }
            if (iEnd >= w) {
                int val;
                if (type == HSV)
                    val = qc.value();
                else if (type == HSL || type == RGB)
                    val = qc.lightness();
                else if (type == LAB)
                    val = Color::getLabScaled(Color::rgb2lab(qc))[0];
                nums.insert(nums.end(), iEnd - w, val);
                iEnd = w;
            }
            for (int y = jStart; y <= jEnd; ++y) {
                QRgb* line = reinterpret_cast<QRgb *>(img.scanLine(y));
                for (int x = iStart; x <= iEnd; ++x) {
                    QColor color = line[x];
                    int val;
                    if (type == HSV)
                        val = color.value();
                    else if (type == HSL)
                        val = color.lightness();
                    else if (type == LAB)
                        val = Color::getLabScaled(Color::rgb2lab(color))[0];
                    nums.push_back(val);
                }
            }
            std::sort(nums.begin(), nums.end());
            int val = nums[nums.size() / 2];
            if (type == HSV)
                qc.setHsv(qc.hsvHue(), qc.hsvSaturation(), val);
            else if (type == HSL)
                qc.setHsl(qc.hslHue(), qc.hslSaturation(), val);
            else if (type == LAB) {
                vec4 vec = Color::getLabScaled(Color::rgb2lab(qc));
                vec.xyzw[0] = val;
                qc = Color::toQColor(Color::lab2rgb(Color::getLabDescaled(vec)));
            }
            line2[i] = qc.rgba();
        }
    }
}

void graphics::Filtering::kuwahara(QImage *qi, int radius, eType type) {
    QImage toProcess = qi->copy();
    int w = toProcess.width(), h = toProcess.height();
    vector <vector <int>> og(w, vector <int> (h, 0));
    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(toProcess.scanLine(y));
        for (int x = 0; x < w; ++x) {
            QColor color = line[x];
            if (type == RGB || type == HSL)
                og[x][y] = static_cast<int>(255.0 * color.lightnessF());
            else if (type == HSV)
                og[x][y] = static_cast<int>(255.0 * color.valueF());
            else if (type == LAB)
                og[x][y] = static_cast<int>(Color::getLabScaled(Color::rgb2lab(color))[0]);
        }
    }
    int rad2 = radius * radius;
    int pad = radius - 1;
    vector <vector <int>> Area = vector <vector <int>> (4, vector <int> (rad2, 0));
    int Size[4] = {0};
    unsigned long long Sum[4] = {0};
    for (int y = 0; y < h; ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(toProcess.scanLine(y));
        for (int x = 0; x < w; ++x) {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < rad2; ++j)
                    Area[i][j] = 0;
                Size[i] = 0;
                Sum[i] = 0;
            }
            int jStart = max(y - pad, 0), jEnd = min(y, h - 1);
            int iStart1 = max(x - pad, 0), iEnd1 = min(x, w - 1);
            int iStart2 = x, iEnd2 = min(x + pad, w - 1);
            for (int j = jStart; j <= jEnd; ++j) {
                for (int i = iStart1; i <= iEnd1; ++i) {
                    Area[1][Size[1]] = og[i][j];
                    Sum[1] += og[i][j];
                    ++Size[1];
                }
                for (int i = iStart2; i <= iEnd2; ++i) {
                    Area[2][Size[2]] = og[i][j];
                    Sum[2] += og[i][j];
                    ++Size[2];
                }
            }
            jStart = y;
            jEnd = min(y + pad, h - 1);
            for (int j = jStart; j <= jEnd; ++j) {
                for (int i = iStart1; i <= iEnd1; ++i) {
                    Area[3][Size[3]] = og[i][j];
                    Sum[3] += og[i][j];
                    ++Size[3];
                }
                for (int i = iStart2; i <= iEnd2; ++i) {
                    Area[0][Size[0]] = og[i][j];
                    Sum[0] += og[i][j];
                    ++Size[0];
                }
            }
            int index = 0;
            double var = DBL_MAX;
            for (int i = 0; i < 4; ++i) {
                if (Size[i] <= 1)
                    continue;
                double mean = static_cast<double>(Sum[i]) / static_cast<double>(Size[i]);
                double temp = 0.0;
                for (int j = 0; j < Size[i]; ++j)
                    temp += (static_cast<double>(Area[i][j]) - mean) * (static_cast<double>(Area[i][j]) - mean);
                temp = sqrt(temp / (static_cast<double>(Size[i]) - 1));
                if (temp < var) {
                    index = i;
                    var = temp;
                }
            }
            var = static_cast<double>(Sum[index]) / static_cast<double>(Size[index]);
            QColor color = line[x];
            if (type == RGB || type == HSL)
                color.setHslF(color.hslHueF(), color.hslSaturationF(), var / 255.0);
            else if (type == HSV)
                color.setHslF(color.hsvHueF(), color.hsvSaturationF(), var / 255.0);
            else if (type == LAB) {
                vec4 c = Color::getLabScaled(Color::rgb2lab(color));
                c.xyzw[0] = var;
                color = Color::toQColor(Color::lab2rgb(Color::getLabDescaled(c)));
            }
            line[x] = color.rgba();
        }
    }
    *qi = toProcess;
}

void graphics::Filtering::pixelate(QImage *qi, int radius, eType type) {
    QImage img = QImage(qi->size(), QImage::Format_ARGB32);
    int w = img.width(), h = img.height(), d = 2 * radius + 1;
    int xOff = (w % d) / 2, yOff = (h % d) / 2;
    int xn = w / d, yn = h / d;
    if (xOff != 0)
        xn += 2;
    if (yOff != 0)
        yn += 2;
    for (int j = 0; j < yn; ++j) {
        int J = j * d + yOff - radius;
        int yStart = max(0, J - radius);
        int yEnd = min(h - 1, J + radius);
        for (int i = 0; i < xn; ++i) {
            int r = 0, g = 0, b = 0, c = 0;
            int I = i * d + xOff - radius;
            int xStart = max(0, I - radius);
            int xEnd = min(w - 1, I + radius);
            for (int y = yStart; y <= yEnd; ++y) {
                QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(y));
                for (int x = xStart; x <= xEnd; ++x) {
                    QColor color = line[x];
                    r += color.red();
                    g += color.green();
                    b += color.blue();
                    ++c;
                }
            }
            if (c == 0)
                continue;
            r /= c;
            g /= c;
            b /= c;
            QRgb color = QColor(r, g, b, 255).rgba();
            for (int y = yStart; y <= yEnd; ++y) {
                QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
                for (int x = xStart; x <= xEnd; ++x)
                    line[x] = color;
            }
        }
    }
    *qi = img;
}

void graphics::Filtering::bubble(QImage *qi, int radius, eType type) {
    QImage img = QImage(qi->size(), QImage::Format_ARGB32);
    int w = img.width(), h = img.height(), d = 2 * radius + 1;
    int xOff = (w % d) / 2, yOff = (h % d) / 2;
    int xn = w / d, yn = h / d;
    int R = radius * radius;
    if (xOff != 0)
        xn += 2;
    if (yOff != 0)
        yn += 2;
    for (int j = 0; j < yn; ++j) {
        int J = j * d + yOff - radius;
        int yStart = max(0, J - radius);
        int yEnd = min(h - 1, J + radius);
        for (int i = 0; i < xn; ++i) {
            int r = 0, g = 0, b = 0, c = 0;
            int I = i * d + xOff - radius;
            int xStart = max(0, I - radius);
            int xEnd = min(w - 1, I + radius);
            for (int y = yStart; y <= yEnd; ++y) {
                QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(y));
                for (int x = xStart; x <= xEnd; ++x) {
                    QColor color = line[x];
                    r += color.red();
                    g += color.green();
                    b += color.blue();
                    ++c;
                }
            }
            if (c == 0)
                continue;
            r /= c;
            g /= c;
            b /= c;
            QRgb color = QColor(r, g, b, 255).rgba();
            for (int y = yStart; y <= yEnd; ++y) {
                int yd = (y - J) * (y - J);
                QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
                for (int x = xStart; x <= xEnd; ++x) {
                    int xd = (x - I) * (x - I);
                    if (xd + yd <= R)
                        line[x] = color;
                    else
                        line[x] = 0xFF000000;
                }
            }
        }
    }
    *qi = img;
}

void graphics::Filtering::diamonds(QImage *qi, int radius, eType type) {
    QImage img = QImage(qi->size(), QImage::Format_ARGB32);
    int w = img.width(), h = img.height(), d = 2 * radius + 1;
    int xOff = (w % d) / 2, yOff = (h % d) / 2;
    int xn = w / d, yn = h / d;
    if (xOff != 0)
        xn += 2;
    if (yOff != 0)
        yn += 2;
    for (int j = 0; j < yn; ++j) {
        int J = j * d + yOff - radius;
        int yStart = max(0, J - radius);
        int yEnd = min(h - 1, J + radius);
        for (int i = 0; i < xn; ++i) {
            int r = 0, g = 0, b = 0, c = 0;
            int I = i * d + xOff - radius;
            int xStart = max(0, I - radius);
            int xEnd = min(w - 1, I + radius);
            for (int y = yStart; y <= yEnd; ++y) {
                QRgb *line = reinterpret_cast<QRgb *>(qi->scanLine(y));
                for (int x = xStart; x <= xEnd; ++x) {
                    QColor color = line[x];
                    r += color.red();
                    g += color.green();
                    b += color.blue();
                    ++c;
                }
            }
            if (c == 0)
                continue;
            r /= c;
            g /= c;
            b /= c;
            QRgb color = QColor(r, g, b, 255).rgba();
            for (int y = yStart; y <= yEnd; ++y) {
                int yd = abs(y - J);
                QRgb *line = reinterpret_cast<QRgb *>(img.scanLine(y));
                for (int x = xStart; x <= xEnd; ++x) {
                    int xd = abs(x - I);
                    int r = radius;
                    while (r > 1) {
                        if (xd + yd <= r) {
                            if (max(xd, yd) <= r / 2) {
                                r /= 2;
                                if (r == 1)
                                    line[x] = 0xFF000000;
                            }
                            else {
                                line[x] = 0xFF000000;
                                break;
                            }
                        }
                        else {
                            line[x] = color;
                            break;
                        }
                    }
                }
            }
        }
    }
    *qi = img;
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

//todo
void graphics::Color::matchHistogram(QImage *qi, QImage toMatch, mType type) {
    QImage ret(qi->size(), QImage::Format_ARGB32_Premultiplied);
    double intensities[bins] = {0};
    double intensities2[bins] = {0};
    double intensities3[bins] = {-1};
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
    for (int i = 1; i < bins; i++) {
        intensities[i] += intensities[i - 1];
        intensities2[i] += intensities2[i - 1];
    }
    for (int i = 0; i < bins; i++) {
        intensities[i] = min(255.0, round(intensities[i] * 255.0 / static_cast<double>(size - offset1)));
        intensities2[i] = min(255.0, round(intensities2[i] * 255.0 / static_cast<double>(size2 - offset2)));
    }
    for(int i = 0; i < bins; i++) {
        int j = 0;
        do {
            intensities3[i] = j;
            j++;
        }
        while(intensities[i] > intensities2[j]);
    }
    for (int i = 0; i < bins; i++) {
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

QImage graphics::ImgSupport::addLayers(QImage a, QImage b, eType type) {
    int h = min(a.height(), b.height());
    int w = min(a.width(), b.width());
    int H = max(a.height(), b.height());
    int W = max(a.width(), b.width());
    QImage out(W, H, QImage::Format_ARGB32_Premultiplied);
    for (int j = 0; j < H; ++j) {
        QRgb *o = reinterpret_cast<QRgb *>(out.scanLine(j));
        if (j < h) {
            QRgb *A = reinterpret_cast<QRgb *>(a.scanLine(j));
            QRgb *B = reinterpret_cast<QRgb *>(b.scanLine(j));
            QImage c = a.width() > w ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0;  i < W; ++i) {
                if (i < w) {
                    QColor ca(A[i]), cb(B[i]), oc;
                    if (type == RGB)
                        oc = QColor(min(ca.red() + cb.red(), 255), min(ca.green() + cb.green(), 255), min(ca.blue() + cb.blue(), 255), 255);
                    else if (type == HSV) {
                        int hue = ca.hsvHue() + cb.hsvHue();
                        if (hue < -1)
                            hue += 360;
                        if (hue > 359)
                            hue -= 360;
                        oc.setHsvF(static_cast<double>(hue) / 360.0, min(ca.saturationF() + cb.saturationF(), 1.0), min(ca.valueF() + cb.valueF(), 1.0), 1.0);
                    }
                    else if (type == HSL) {
                        int hue = ca.hslHue() + cb.hslHue();
                        if (hue < -1)
                            hue += 360;
                        if (hue > 359)
                            hue -= 360;
                        oc.setHslF(static_cast<double>(hue) / 360.0, min(ca.hslSaturationF() + cb.hslSaturationF(), 1.0), min(ca.lightnessF() + cb.lightnessF(), 1.0), 1.0);
                    }
                    //cout << ca.rgba() << endl;
                    o[i] = oc.rgba();
                }
                else
                    o[i] = C[i];
            }
        }
        else {
            QImage c = a.height() > h ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0; i < W; ++i)
                o[i] = i < c.width() ? C[i] : 0x00000000;
        }
    }
    // todo lab
    return out;
}

QImage graphics::ImgSupport::subLayers(QImage a, QImage b, eType type) {
    int h = min(a.height(), b.height());
    int w = min(a.width(), b.width());
    int H = max(a.height(), b.height());
    int W = max(a.width(), b.width());
    QImage out(W, H, QImage::Format_ARGB32_Premultiplied);
    for (int j = 0; j < H; ++j) {
        QRgb *o = reinterpret_cast<QRgb *>(out.scanLine(j));
        if (j < h) {
            QRgb *A = reinterpret_cast<QRgb *>(a.scanLine(j));
            QRgb *B = reinterpret_cast<QRgb *>(b.scanLine(j));
            QImage c = a.width() > w ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0;  i < W; ++i) {
                if (i < w) {
                    QColor ca(A[i]), cb(B[i]), oc;
                    if (type == RGB)
                        oc = QColor(max(ca.red() - cb.red(), 0), max(ca.green() - cb.green(), 0), max(ca.blue() - cb.blue(), 0), 255);
                    else if (type == HSV) {
                        int hue = ca.hsvHue() - cb.hsvHue();
                        if (hue < -1)
                            hue += 360;
                        if (hue > 359)
                            hue -= 360;
                        oc.setHsvF(static_cast<double>(hue) / 360.0, max(ca.saturationF() - cb.saturationF(), 0.0), max(ca.valueF() - cb.valueF(), 0.0), 1.0);
                    }
                    else if (type == HSL) {
                        QColor ca(A[i]), cb(B[i]), oc;
                        int hue = ca.hslHue() - cb.hslHue();
                        if (hue < -1)
                            hue += 360;
                        if (hue > 359)
                            hue -= 360;
                        oc.setHslF(static_cast<double>(hue) / 360.0, max(ca.hslSaturationF() - cb.hslSaturationF(), 0.0), max(ca.lightnessF() - cb.lightnessF(), 0.0), 1.0);
                        o[i] = oc.rgba();
                    }
                    o[i] = oc.rgba();
                }
                else
                    o[i] = C[i];
            }
        }
        else {
            QImage c = a.height() > h ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0; i < W; ++i)
                o[i] = i < c.width() ? C[i] : 0x00000000;
        }
    }
    // todo lab
    return out;
}

QImage graphics::ImgSupport::diffLayers(QImage a, QImage b, eType type) {
    int h = min(a.height(), b.height());
    int w = min(a.width(), b.width());
    int H = max(a.height(), b.height());
    int W = max(a.width(), b.width());
    QImage out(W, H, QImage::Format_ARGB32_Premultiplied);
    for (int j = 0; j < H; ++j) {
        QRgb *o = reinterpret_cast<QRgb *>(out.scanLine(j));
        if (j < h) {
            QRgb *A = reinterpret_cast<QRgb *>(a.scanLine(j));
            QRgb *B = reinterpret_cast<QRgb *>(b.scanLine(j));
            QImage c = a.width() > w ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0;  i < W; ++i) {
                if (i < w) {
                    QColor ca(A[i]), cb(B[i]), oc;
                    if (type == RGB)
                        oc = QColor(abs(ca.red() - cb.red()), abs(ca.green() - cb.green()), abs(ca.blue() - cb.blue()), 255);
                    else if (type == HSV)
                        oc.setHsv(abs(ca.hsvHue() - cb.hsvHue()), abs(ca.saturation() - cb.saturation()), abs(ca.value() - cb.value()), 255);
                    else if (type == HSL)
                        oc.setHsl(abs(ca.hslHue() - cb.hslHue()), abs(ca.hslSaturation() - cb.hslSaturation()), abs(ca.lightness() - cb.lightness()), 255);
                    o[i] = oc.rgba();
                }
                else
                    o[i] = C[i];
            }
        }
        else {
            QImage c = a.height() > h ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0; i < W; ++i)
                o[i] = i < c.width() ? C[i] : 0x00000000;
        }
    }
    // todo lab
    return out;
}

QImage graphics::ImgSupport::maxLayers(QImage a, QImage b, eType type) {
    int h = min(a.height(), b.height());
    int w = min(a.width(), b.width());
    int H = max(a.height(), b.height());
    int W = max(a.width(), b.width());
    QImage out(W, H, QImage::Format_ARGB32_Premultiplied);
    for (int j = 0; j < H; ++j) {
        QRgb *o = reinterpret_cast<QRgb *>(out.scanLine(j));
        if (j < h) {
            QRgb *A = reinterpret_cast<QRgb *>(a.scanLine(j));
            QRgb *B = reinterpret_cast<QRgb *>(b.scanLine(j));
            QImage c = a.width() > w ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0;  i < W; ++i) {
                if (i < w) {
                    QColor ca(A[i]), cb(B[i]), oc;
                    if (type == RGB)
                        oc = QColor(max(ca.red(), cb.red()), max(ca.green(), cb.green()), max(ca.blue(), cb.blue()), 255);
                    else if (type == HSV)
                        oc.setHsv((ca.hsvHue() + cb.hsvHue()) / 2, max(ca.hsvSaturation(), cb.hsvSaturation()), max(ca.value(), cb.value()), 255);
                    else if (type == HSL)
                        oc.setHsl((ca.hslHue() + cb.hslHue()) / 2, max(ca.hslSaturation(), cb.hslSaturation()), max(ca.lightness(), cb.lightness()), 255);
                    o[i] = oc.rgba();
                }
                else
                    o[i] = C[i];
            }
        }
        else {
            QImage c = a.height() > h ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0; i < W; ++i)
                o[i] = i < c.width() ? C[i] : 0x00000000;
        }
    }
    return out;
    //todo lab
}

QImage graphics::ImgSupport::minLayers(QImage a, QImage b, eType type) {
    int h = min(a.height(), b.height());
    int w = min(a.width(), b.width());
    int H = max(a.height(), b.height());
    int W = max(a.width(), b.width());
    QImage out(W, H, QImage::Format_ARGB32_Premultiplied);
    for (int j = 0; j < H; ++j) {
        QRgb *o = reinterpret_cast<QRgb *>(out.scanLine(j));
        if (j < h) {
            QRgb *A = reinterpret_cast<QRgb *>(a.scanLine(j));
            QRgb *B = reinterpret_cast<QRgb *>(b.scanLine(j));
            QImage c = a.width() > w ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0;  i < W; ++i) {
                if (i < w) {
                    QColor ca(A[i]), cb(B[i]), oc;
                    if (type == RGB)
                        oc = QColor(min(ca.red(), cb.red()), min(ca.green(), cb.green()), min(ca.blue(), cb.blue()), 255);
                    else if (type == HSV)
                        oc.setHsv((ca.hsvHue() + cb.hsvHue()) / 2, min(ca.hsvSaturation(), cb.hsvSaturation()), min(ca.value(), cb.value()), 255);
                    else if (type == HSL)
                        oc.setHsl((ca.hslHue() + cb.hslHue()) / 2, min(ca.hslSaturation(), cb.hslSaturation()), min(ca.lightness(), cb.lightness()), 255);
                    o[i] = oc.rgba();
                }
                else
                    o[i] = C[i];
            }
        }
        else {
            QImage c = a.height() > h ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0; i < W; ++i)
                o[i] = i < c.width() ? C[i] : 0x00000000;
        }
    }
    return out;
    //todo lab
}

QImage graphics::ImgSupport::avgLayers(QImage a, QImage b, eType type) {
    int h = min(a.height(), b.height());
    int w = min(a.width(), b.width());
    int H = max(a.height(), b.height());
    int W = max(a.width(), b.width());
    QImage out(W, H, QImage::Format_ARGB32_Premultiplied);
    for (int j = 0; j < H; ++j) {
        QRgb *o = reinterpret_cast<QRgb *>(out.scanLine(j));
        if (j < h) {
            QRgb *A = reinterpret_cast<QRgb *>(a.scanLine(j));
            QRgb *B = reinterpret_cast<QRgb *>(b.scanLine(j));
            QImage c = a.width() > w ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0;  i < W; ++i) {
                if (i < w) {
                    QColor ca(A[i]), cb(B[i]), oc;
                    if (type == RGB)
                        oc = QColor((ca.red() + cb.red()) / 2, (ca.green() + cb.green()) / 2, (ca.blue() + cb.blue()) / 2, 255);
                    else if (type == HSV) {
                        bool flag = abs(ca.hsvHue() - cb.hsvHue()) > 180;
                        int hue;
                        if (flag) {
                            if (ca.hsvHue() < cb.hsvHue())
                                hue = ca.hsvHue() > 360 - cb.hsvHue() ? (ca.hsvHue() + (cb.hsvHue() - 360)) : (cb.hsvHue() + (ca.hsvHue() + 360));
                            else
                                hue = cb.hsvHue() > 360 - cb.hsvHue() ? (cb.hsvHue() + (ca.hsvHue() - 360)) : (ca.hsvHue() + (cb.hsvHue() + 360));
                        }
                        else
                            hue = ca.hsvHue() + cb.hsvHue();
                        hue /= 2;
                        oc.setHsv(hue, (ca.hsvSaturation() + cb.hsvSaturation()) / 2, (ca.value() + cb.value()) / 2, 255);
                    }
                    else if (type == HSL) {
                        bool flag = abs(ca.hslHue() - cb.hslHue()) > 180;
                        int hue;
                        if (flag) {
                            if (ca.hslHue() < cb.hslHue())
                                hue = ca.hslHue() > 360 - cb.hslHue() ? (ca.hslHue() + (cb.hslHue() - 360)) : (cb.hslHue() + (ca.hslHue() + 360));
                            else
                                hue = cb.hslHue() > 360 - cb.hslHue() ? (cb.hslHue() + (ca.hslHue() - 360)) : (ca.hslHue() + (cb.hslHue() + 360));
                        }
                        else
                            hue = ca.hslHue() + cb.hslHue();
                        hue /= 2;
                        oc.setHsl(hue, (ca.hslSaturation() + cb.hslSaturation()) / 2, (ca.lightness() + cb.lightness()) / 2, 255);
                    }
                    o[i] = oc.rgba();
                }
                else
                    o[i] = C[i];
            }
        }
        else {
            QImage c = a.height() > h ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0; i < W; ++i)
                o[i] = i < c.width() ? C[i] : 0x00000000;
        }
    }
    // todo lab
    return out;
}

QImage graphics::ImgSupport::remLayers(QImage a, QImage b, eType type) {
    QImage diff = diffLayers(a, b, type);
    float histogram[bins] = {0.0};
    float sum = 0.0;
    vector <vector <int> > remMap(diff.width(), vector <int> (diff.height(), 0));
    for (int i = 0; i < diff.width(); ++i)
        for (int j = 0; j < diff.height(); ++j) {
            QColor qc = diff.pixelColor(i, j);
            float f = static_cast<float>(qc.red() + qc.green() + qc.blue()) / 3.0;
            int val = static_cast<int>(f + 0.4);
            remMap[i][j] = val;
            sum += val;
            ++histogram[val];
        }
    float probability[bins];
    float invSize = 1.0 / (diff.width() * diff.height());
    for (int i = 0; i < bins; ++i)
        probability[i] = histogram[i] * invSize;
    float p1 = probability[0];
    float q1 = p1, mu1 = 0.0, mu2 = 0.0;
    float mu = sum * invSize;
    float q1prev = q1, maxbetweenvariance = 0.0, optimizedthresh = 0.0;
    for (int t = 1; t < bins - 1; ++t) {
        float q1next = q1prev + probability[t + 1];
        float mu1next = (q1prev * mu1 + (t + 1) * (probability[t + 1])) / q1next;
        float mu2next = (mu - q1next * mu1next) / (1 - q1next);
        float betweenvariance = q1prev * (1 - q1prev) * ((mu1 - mu2) * (mu1 - mu2));
        if (betweenvariance > maxbetweenvariance){
            maxbetweenvariance = betweenvariance;
            optimizedthresh = t;
        }
        q1prev = q1next;
        mu1 = mu1next;
        mu2 = mu2next;
        if(q1next == 0)
            mu1 = 0;
    }
    QImage out(a.size(), QImage::Format_ARGB32_Premultiplied);
    for (int j = 0; j < out.height(); ++j) {
        QRgb *o = reinterpret_cast<QRgb *>(out.scanLine(j));
        QRgb *A = reinterpret_cast<QRgb *>(a.scanLine(j));
        for (int i = 0; i < diff.width(); ++i)
            o[i] = remMap[i][j] >= optimizedthresh ? A[i] : 0x00000000;
    }
    return out;
}

QImage graphics::ImgSupport::bitLayers(QImage a, QImage b, bType type) {
    int h = min(a.height(), b.height());
    int w = min(a.width(), b.width());
    int H = max(a.height(), b.height());
    int W = max(a.width(), b.width());
    QImage out(W, H, QImage::Format_ARGB32_Premultiplied);
    for (int j = 0; j < H; ++j) {
        QRgb *o = reinterpret_cast<QRgb *>(out.scanLine(j));
        if (j < h) {
            QRgb *A = reinterpret_cast<QRgb *>(a.scanLine(j));
            QRgb *B = reinterpret_cast<QRgb *>(b.scanLine(j));
            QImage c = a.width() > w ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0;  i < W; ++i) {
                if (i < w) {
                    if (type == NOT)
                        o[i] = ~A[i];
                    else if (type == AND)
                        o[i] = A[i] & B[i];
                    else if (type == NAND)
                        o[i] = ~(A[i] & B[i]);
                    else if (type == OR)
                        o[i] = A[i] | B[i];
                    else if (type == NOR)
                        o[i] = ~(A[i] | B[i]);
                    else if (type == XOR)
                        o[i] = A[i] ^ B[i];
                    else if (type == XNOR)
                        o[i] = ~(A[i] ^ B[i]);
                }
                else {
                    o[i] = 0x00000000;
                    if (type == NOT)
                        o[i] = ~C[i];
                    else if (type == AND)
                        o[i] = C[i] & o[i];
                    else if (type == NAND)
                        o[i] = ~(C[i] & o[i]);
                    else if (type == OR)
                        o[i] = C[i] | o[i];
                    else if (type == NOR)
                        o[i] = ~(C[i] | o[i]);
                    else if (type == XOR)
                        o[i] = C[i] ^ o[i];
                    else if (type == XNOR)
                        o[i] = ~(C[i] ^ o[i]);
                }
                o[i] = o[i] | 0xFF000000;
            }
        }
        else {
            QImage c = a.height() > h ? a : b;
            QRgb *C = reinterpret_cast<QRgb *>(c.scanLine(j));
            for (int i = 0; i < W; ++i) {
                o[i] = 0x00000000;
                if (i < c.width()) {
                    if (type == NOT)
                        o[i] = ~C[i];
                    else if (type == AND)
                        o[i] = C[i] & o[i];
                    else if (type == NAND)
                        o[i] = ~(C[i] & o[i]);
                    else if (type == OR)
                        o[i] = C[i] | o[i];
                    else if (type == NOR)
                        o[i] = ~(C[i] | o[i]);
                    else if (type == XOR)
                        o[i] = C[i] ^ o[i];
                    else if (type == XNOR)
                        o[i] = ~(C[i] ^ o[i]);
                }
                o[i] = o[i] | 0xFF000000;
            }
        }
    }
    return out;
}

