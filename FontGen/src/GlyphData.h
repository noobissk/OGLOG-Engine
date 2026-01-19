#pragma once
#include <vector>
#include <string>
#include <sstream>
#include "mathf.h"
#include <iostream>

using namespace std;

constexpr int ATLAS_SIZE = 1024;
constexpr int CELL_SIZE  = 64;
constexpr int COLS = ATLAS_SIZE / CELL_SIZE;
constexpr int ROWS = ATLAS_SIZE / CELL_SIZE;
constexpr int GLYPHS_PER_ATLAS = COLS * ROWS;

struct glyphRecord
{
    uint32_t unicode;
    uint32_t glyph_id;

    float advance;
    float bearingX;
    float bearingY;
    float width;
    float height;

    uint8_t atlasIndex;   // which texture
    float u0, v0;         // bottom-left UV
    float u1, v1;         // top-right UV
};


struct singleAxisCoord {
    int pos; // position
    int id; // file ID; is -1 if coord isn't from file
    bool isOnCurve;
    
    singleAxisCoord() : pos(0), id(0), isOnCurve(0) {}
    singleAxisCoord(int _pos, int _id, bool _isOnCurve) : pos(_pos), id(_id), isOnCurve(_isOnCurve) {}
};

struct glyphPoint {
    public:
    int x, y;
    int id;
    bool isOnCurve;
    
    glyphPoint(singleAxisCoord _x, singleAxisCoord _y) {
        x = _x.pos;
        y = _y.pos;
        id = _x.id;
        isOnCurve = !_x.isOnCurve;
    }
    glyphPoint() = default;


    bool isEndPoint(vector<short> contourEndIndices) {
        for (int i = 0; i < contourEndIndices.size(); i++) {
            if (id == contourEndIndices[i]) return true;
        }
        return false;
    }
};

struct bezierCurve {
private:
    vector<Vector2> renderPoints;
    int cachedResolution = -1;  // Track which resolution is cached
public:
    glyphPoint bgn;
    glyphPoint ctrl;
    glyphPoint end;

    bezierCurve() = default;
    bezierCurve(glyphPoint& _bgn, glyphPoint& _ctrl, glyphPoint& _end) : bgn(_bgn), ctrl(_ctrl), end(_end),
                                                                         renderPoints() {}
    

    bool isEndPoint(vector<short> contourEndIndices) {
        for (int i = 0; i < contourEndIndices.size(); i++) {
            if (end.id == contourEndIndices[i]) return true;
        }
        return false;
    }
    vector<Vector2>* generateRenderPoints(int resolution) const {

        bezierCurve* self = const_cast<bezierCurve*>(this);

        if (self->cachedResolution == resolution && !self->renderPoints.empty()) {
            return &self->renderPoints;
        }
        
        self->renderPoints.clear();
        self->cachedResolution = resolution;
        int count = resolution * resolution;   // e.g. 3 → 9 points
        self->renderPoints.reserve(count + 1);

        Vector2 P0(bgn.x,  bgn.y);
        Vector2 P1(ctrl.x, ctrl.y);
        Vector2 P2(end.x,  end.y);

        for (int i = 0; i <= count; i++) {
            float t = float(i) / float(count);
            self->renderPoints.push_back(Bezier(P0, P1, P2, t));
        }

        return &self->renderPoints;
    }
};


struct glyphBoundaries {
    public:
    int16_t xMin, yMin;
    int16_t xMax, yMax;
    int16_t overallMinX, overallMinY;
    int16_t overallMaxX, overallMaxY;
    
    
    glyphBoundaries() : xMin(0), xMax(0), yMin(0), yMax(0) {}
    glyphBoundaries(int _xMin, int _yMin, int _xMax, int _yMax) : xMin(_xMin), yMin(_yMin), xMax(_xMax), yMax(_yMax) {}
};
extern glyphBoundaries boundaries;

struct glyphData {
public:
    vector<bezierCurve> curves;
    vector<int16_t> contourEndIndices;
    vector<glyphPoint> points;
    glyphBoundaries bounds;  // Store bounds with each glyph
    
    glyphData() : points(), contourEndIndices(), bounds() {}

    bool isNull() {
        return contourEndIndices.empty() || curves.empty();
    }

    glyphData(vector<singleAxisCoord> _coordsX, vector<singleAxisCoord> _coordsY, vector<int16_t> _contourEndIndeces)
         : contourEndIndices(_contourEndIndeces), bounds() {
        for (size_t i = 0; i < _coordsX.size(); i++) {
            points.emplace_back(_coordsX[i], _coordsY[i]);
        }
        
    }
    glyphData(vector<glyphPoint> _points, vector<int16_t> _contourEndIndeces)
         : contourEndIndices(_contourEndIndeces), points(_points), bounds() { }

    void buildBeziersFromPoints()
    {
        vector<glyphPoint>& pts = points;
        vector<bezierCurve> crvs;
        vector<glyphPoint> renderPoints;
        vector<short> contours;
        contours.reserve(contourEndIndices.size());


        if (pts.empty()) return;

        // Helper to make an implicit on-curve point
        auto makeImplicit = [&](const glyphPoint& A, const glyphPoint& B) {
            singleAxisCoord sx((A.x + B.x) / 2, -1, A.isOnCurve);
            singleAxisCoord sy((A.y + B.y) / 2, -1, A.isOnCurve);
            return glyphPoint(sx, sy);
        };


        int bgnIDX = 0, endIDX = 0; // make bezier curve START CONTROL END points
        for (int c = 0; c < contourEndIndices.size(); c++) {
            endIDX = contourEndIndices[c];
            renderPoints.emplace_back(pts[bgnIDX]);
            for (int i = bgnIDX + 1; i <= endIDX; i++)
            {
                const glyphPoint& ptp = pts[i - 1]; // previous
                const glyphPoint& ptc = pts[i];     // current

                if (ptp.isOnCurve == ptc.isOnCurve) renderPoints.emplace_back(makeImplicit(ptp, ptc));
                renderPoints.emplace_back(ptc);
            }

            if (pts[endIDX].isOnCurve == pts[bgnIDX].isOnCurve)
                renderPoints.emplace_back(makeImplicit(pts[endIDX], pts[bgnIDX]));
            contours.emplace_back(renderPoints.size()-1);
            bgnIDX = endIDX + 1;
        }

        pts.clear();

        contourEndIndices = move(contours);

        // build subdivide bezier curves
        bgnIDX = endIDX = 0;
        for (int c = 0; c < contourEndIndices.size(); c++) {
            endIDX = contourEndIndices[c];
            for (int i = bgnIDX; i < endIDX - 1;) {
                int i1 = i++, i2 = i++, i3 = i;
                crvs.emplace_back(renderPoints[i1], renderPoints[i2], renderPoints[i3]);
            }
            crvs.emplace_back(renderPoints[endIDX-1], renderPoints[endIDX], renderPoints[bgnIDX]);
            bgnIDX = endIDX + 1;
        }
        curves = move(crvs);
    }
};