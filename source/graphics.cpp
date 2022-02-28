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

QRgb graphics::Filtering::redPass(QColor qc, int strength) {
    if (qc.red() > qc.blue() && qc.red() > qc.green() && qc.red() - max(qc.green(), qc.blue()) > abs(qc.green() - qc.blue()))
        return qc.rgba();
    int grey = (qc.red() + qc.green() + qc.blue()) / 3;
    return toRGB(qc.alpha(), grey, grey, grey);
}

QRgb graphics::Filtering::greenPass(QColor qc, int strength) {
    if (qc.green() > qc.blue() && qc.green() > qc.red() && qc.green() - max(qc.red(), qc.blue()) > abs(qc.red() - qc.blue()))
        return qc.rgba();
    int grey = (qc.red() + qc.green() + qc.blue()) / 3;
    return toRGB(qc.alpha(), grey, grey, grey);
}

QRgb graphics::Filtering::bluePass(QColor qc, int strength) {
    if (qc.blue() > qc.red() && qc.blue() > qc.green() && qc.blue() - max(qc.red(), qc.green()) > abs(qc.green() - qc.red()))
        return qc.rgba();
    int grey = (qc.red() + qc.green() + qc.blue()) / 3;
    return toRGB(qc.alpha(), grey, grey, grey);
}

QRgb graphics::Filtering::magentaPass(QColor qc, int strength) {
    if (max(qc.red(), qc.blue()) - min(qc.red(), qc.blue()) < min(qc.red(), qc.blue()) - qc.green())
        return qc.rgba();
    int grey = (qc.red() + qc.green() + qc.blue()) / 3;
    return toRGB(qc.alpha(), grey, grey, grey);
}

QRgb graphics::Filtering::yellowPass(QColor qc, int strength) {
    if (max(qc.red(), qc.green()) - min(qc.red(), qc.green()) < min(qc.red(), qc.green()) - qc.blue())
        return qc.rgba();
    int grey = (qc.red() + qc.green() + qc.blue()) / 3;
    return toRGB(qc.alpha(), grey, grey, grey);
}

QRgb graphics::Filtering::cyanPass(QColor qc, int strength) {
    if (max(qc.green(), qc.blue()) - min(qc.green(), qc.blue()) < min(qc.green(), qc.blue()) - qc.red())
        return qc.rgba();
    int grey = (qc.red() + qc.green() + qc.blue()) / 3;
    return toRGB(qc.alpha(), grey, grey, grey);
}

QRgb graphics::Filtering::redFilter(QColor qc, int strength) {
    int combined = (qc.red() + qc.green() + qc.blue()) / 3;
    return (qc.red() > qc.green() && qc.red() > qc.blue()) ? toRGB(qc.alpha(), combined, combined, combined) : toRGB(qc.alpha(), qc.red(), qc.green(), qc.blue());
}

QRgb graphics::Filtering::greenFilter(QColor qc, int strength) {
    int combined = (qc.red() + qc.green() + qc.blue()) / 3;
    return (qc.green() > qc.blue() && qc.green() > qc.blue()) ? toRGB(qc.alpha(), combined, combined, combined) : toRGB(qc.alpha(), qc.red(), qc.green(), qc.blue());
}

QRgb graphics::Filtering::blueFilter(QColor qc, int strength) {
    int combined = (qc.red() + qc.green() + qc.blue()) / 3;
    return (qc.blue() > qc.red() && qc.blue() > qc.green())  ? toRGB(qc.alpha(), combined, combined, combined) : toRGB(qc.alpha(), qc.red(), qc.green(), qc.blue());
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

void graphics::Filtering::applyKernal(QProgressDialog *qpd, QImage *qi, KernalData kernalInfo) {
    bool needGreyscale = kernalInfo.first;
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
    Kernal kernal = kernalInfo.second;
    int kernalSize = kernal.size();
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

void graphics::Filtering::ditherBayer(QImage *qi, int bpp, int matrixSize) {
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

void graphics::Filtering::ditherRandom(QImage *qi, int bpp) {
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

void graphics::Filtering::paletteReduction(QImage *qi, int bpp) {
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

void graphics::Filtering::ditherFloydSteinberg(QImage *qi, int bpp) {
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

void graphics::Filtering::ditherSierra(QImage *qi, int bpp) {
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

void graphics::Filtering::ditherSierraLite(QImage *qi, int bpp) {
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

void graphics::Filtering::colorTransfer(QImage *to, QImage from) {
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
            color = lab2lms * color;
            color._L = pow(10.0, color._L);
            color._A = pow(10.0, color._A);
            color._B = pow(10.0, color._B);
            color = lms2rgb * color;
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
    qp.setX(static_cast<int>(0.5 + static_cast<double>(qp.x()) / zoom));
    qp.setY(static_cast<int>(0.5 + static_cast<double>(qp.y()) / zoom));
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

KernalData graphics::ImgSupport::loadKernal(string fileName) {
    Kernal kernal, identity;
    identity.push_back(vector <float> ());
    identity[0].push_back(1.0);
    KernalData ret = KernalData (false, identity);
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

void graphics::ImgSupport::Histogram(QLabel *histograms, QImage *in, int layerNum, eType type) {
    int hueDiv = bins / 3;
    int h = 3 * bins;
    QImage qi (QSize(bins * 2, h), QImage::Format_ARGB32_Premultiplied);
    histograms->resize(qi.width(), qi.height());
    histograms->setWindowFilePath(("Histogram of Layer " + to_string(layerNum)).c_str());
    qi.fill(0xFF000000);
    int histo[4][bins];
    for (int x = 0; x < bins; ++x)
        for (int y = 0; y < 4; ++y)
            histo[y][x] = 0;
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
                    ++histo[0][qc.hsvHue()];
                    ++histo[1][qc.hsvSaturation()];
                    ++histo[2][qc.value()];
                }
                else {
                    ++histo[0][qc.hslHue()];
                    ++histo[1][qc.hslSaturation()];
                    ++histo[2][qc.lightness()];
                }
                ++total;
            }
        }
    int maxI = 0, cutoff = (total) / 4;
    for (int j = 0; j < 4; ++j)
        for (int i = 1; i < bins - 1; ++i)
            if (histo[j][i] < cutoff)
                maxI = max(maxI, histo[j][i]);
    // Draw histograms.
    double div = static_cast<double>(h / 2 - 1) / static_cast<double>(maxI);
    float fbins = static_cast<float>(bins - 1);
    for (int x = 0; x < bins; ++x) {
        QRgb value = static_cast<QRgb>(bins + x) / 2;
        for  (int j = 0; j < 4; ++j) {
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
    histograms->setPixmap(QPixmap::fromImage(qi));
    histograms->setFixedSize(histograms->size());
}

void graphics::ImgSupport::equalizeHistogramTo(QImage *qi, eType type) {
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
                value = static_cast<int>(vf * static_cast<int>(bins - 1) + 0.4);
                stdFuncs::clamp(value, 0, bins - 1);
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
                float vf;
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
