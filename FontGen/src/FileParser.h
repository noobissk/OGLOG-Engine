#pragma once
#include "ByteReading.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "GlyphData.h"
#include <map>

using namespace std;

static string path;
static uint32_t scaler_type;
static uint16_t numTables;
static uint16_t searchRange;
static uint16_t entrySelector;
static uint16_t rangeShift;

struct subTable {
public:
    uint32_t checksum;
    uint32_t offset;
    uint32_t length;

    subTable() : checksum(0), offset(0), length(0) {}

    subTable(uint32_t _checksum, uint32_t _offset, uint32_t _length) : checksum(_checksum), offset(_offset), length(_length) {}
};
struct HHEA {
    uint16_t majorVersion;
    uint16_t minorVersion;
    int16_t ascender;
    int16_t descender;
    int16_t lineGap;
    uint16_t advanceWidthMax;
    int16_t minLeftSideBearing;
    int16_t minRightSideBearing;
    int16_t xMaxExtent;
    int16_t caretSlopeRise;
    int16_t caretSlopeRun;
    int16_t caretOffset;
    int16_t reserved1;
    int16_t reserved2;
    int16_t reserved3;
    int16_t reserved4;
    int16_t metricDataFormat;
    uint16_t numberOfHMetrics;
    HHEA() {
        uint16_t majorVersion = 0;
        uint16_t minorVersion = 0;
        int16_t ascender = 0;
        int16_t descender = 0;
        int16_t lineGap = 0;
        uint16_t advanceWidthMax = 0;
        int16_t minLeftSideBearing = 0;
        int16_t minRightSideBearing = 0;
        int16_t xMaxExtent = 0;
        int16_t caretSlopeRise = 0;
        int16_t caretSlopeRun = 0;
        int16_t caretOffset = 0;
        int16_t reserved1 = 0;
        int16_t reserved2 = 0;
        int16_t reserved3 = 0;
        int16_t reserved4 = 0;
        int16_t metricDataFormat = 0;
        uint16_t numberOfHMetrics = 0;
    }
};


static map<string, subTable> subTableMap;
extern vector<unsigned int> glyphOffsets;

static bool flagBitIsSet(uint8_t flag, int bitIndex) {
    return (flag & (1 << bitIndex)) != 0;
}
static int getContourEndIDFromCoord(const singleAxisCoord& coords, const vector<int16_t>& contourEndIndices) {
    for (size_t i = 0; i < contourEndIndices.size(); i++) {
        if (contourEndIndices[i] <= coords.id) return contourEndIndices[i];
    }
    return contourEndIndices.back(); // or some safe fallback
}
static void recalculateLegitPointIDs(vector<glyphPoint>* points) {
    int idxCounter = 0;
    for (int i = 0; i < points->size(); i++) {
        if (points->at(i).id >= 0) {
            points->at(i).id = idxCounter++;
        }
    }
}
static int getContourEndIndexFromID(int originalID, const vector<glyphPoint>& point) {
    for (size_t i = 0; i < point.size(); i++) {
        if (point[i].id == originalID) return i;
    }
    return point.size() - 1; // or some safe fallback
}
static int getContourEndIndexFromID(int originalID, const vector<singleAxisCoord>& point) {
    for (size_t i = 0; i < point.size(); i++) {
        if (point[i].id == originalID) return i;
    }
    return point.size() - 1; // or some safe fallback
}

static void ParseFont(ifstream& file) {

    cout << "[LOG] tables: "<< endl;
    for (int i = 0; i < numTables; i++) {
        string tag = read4Char(file);
        uint32_t checksum = readUInt32(file);
        uint32_t offset = readUInt32(file);
        uint32_t length = readUInt32(file);
        subTableMap.emplace(tag, subTable(checksum, offset, length));
        cout << "[DEBUG] tag -> '" << tag << "' | Location -> '" << offset << "'" << endl;
    }
}


static vector<singleAxisCoord> ReadCoordinates(ifstream& file, const vector<uint8_t>* flags, const vector<int16_t>& contourEndIndices, bool isX) {
    const int SHORT_BIT = isX ? 1 : 2;
    const int SAME_BIT  = isX ? 4 : 5;

    vector<singleAxisCoord> coords;
    int prev = 0;

    uint8_t flagOld = 0;
    int negI = -1;
    for (int i = 0; i < flags->size(); i++) {

        if (i != flags->size() - 1) {
            flagOld = flags->at(i + 1);
        }
        uint8_t flag = flags->at(i);
        int delta = 0;

        if (flag & (1 << SHORT_BIT)) {
            // short 1-byte delta
            uint8_t v = readUInt8(file);
            if (flag & (1 << SAME_BIT))
                delta = +v;
            else
                delta = -((int)v);
        }
        else {
            // not short
            if (flag & (1 << SAME_BIT)) {
                delta = 0;  // explicitly no change
            } else {
                delta = readInt16(file);  // SIGNED!
            }
        }
        
        coords.push_back(singleAxisCoord(prev + delta, i, flagBitIsSet(flag, 0)));
        prev = coords.back().pos;
    }

    return coords;
}


static glyphData ReadSimpleGlyph(ifstream& file, int16_t countInd, unsigned int glyphID) {
    vector<int16_t> contourEndIndices(countInd);
    file.seekg(glyphOffsets[glyphID] + 10, ios::beg); // skip bounds

    for (int i = 0; i < countInd; i++)
        contourEndIndices[i] = readUInt16(file);

    uint16_t instructionLength = readUInt16(file);
    file.seekg(instructionLength, ios::cur);

    uint16_t numPoints = contourEndIndices.back() + 1;
    vector<uint8_t> allFlags;

    for (int i = 0; i < numPoints;) {
        uint8_t flag = readUInt8(file);
        allFlags.push_back(flag);
        i++;
        if (flag & 0x08) { // repeat flag
            uint8_t repeatCount = readUInt8(file);
            for (int r = 0; r < repeatCount; r++) {
                allFlags.push_back(flag);
                i++;
            }
        }
    }

    vector<singleAxisCoord> coordsX = ReadCoordinates(file, &allFlags, contourEndIndices, true);
    vector<singleAxisCoord> coordsY = ReadCoordinates(file, &allFlags, contourEndIndices, false);

    return glyphData(coordsX, coordsY, contourEndIndices);
}

static glyphData readComponentGlyph(ifstream& file, int16_t countInd, glyphData& parent, bool& isLast) {
    uint16_t flags = readUInt16(file);
    uint16_t glyphIndex = readUInt16(file);

    uint32_t oldLocation = static_cast<uint32_t>(file.tellg());
    file.seekg(glyphOffsets[glyphIndex], ios::beg);
    countInd = readInt16(file);
    glyphData glyph = ReadSimpleGlyph(file, countInd, glyphIndex);
    file.seekg(oldLocation, ios::beg);

    int arg1 = flagBitIsSet(flags, 0) ? readInt16(file) : readUInt8(file);
    int arg2 = flagBitIsSet(flags, 0) ? readInt16(file) : readUInt8(file);
    double offsetX = 0;
    double offsetY = 0;

    if (flagBitIsSet(flags, 1)) {
        offsetX = arg1;
        offsetY = arg2;
    } else {
        int parentPointIndex    = arg1;
        int componentPointIndex = arg2;
        offsetX = parent.points[parentPointIndex].x
                - glyph.points[componentPointIndex].x;

        offsetY = parent.points[parentPointIndex].y
                - glyph.points[componentPointIndex].y;
    }
    double scaleX = 1;
    double scaleY = 1;

    if (flagBitIsSet(flags, 3)) {
        scaleX = scaleY = readF2DOT14(file);
    } else if (flagBitIsSet(flags, 6)) {
        scaleX = readF2DOT14(file);
        scaleY = readF2DOT14(file);
    }
    else if (flagBitIsSet(flags, 7)) throw runtime_error("TODO: 2x2 Matrix");

    for (int i = 0; i < glyph.points.size(); i++) {
        glyph.points[i].x = (int)glyph.points[i].x * scaleX + offsetX;
        glyph.points[i].y = (int)glyph.points[i].y * scaleY + offsetY;
    }

    isLast = (!flagBitIsSet(flags, 5));
    return glyph;
}

static glyphData ReadCompoundGlyph(ifstream& file, int16_t countInd, unsigned int glyphID) {
    file.seekg(glyphOffsets[glyphID] + 10, ios::beg);
    
    vector<glyphPoint> points;
    vector<int16_t> allContourEndIndices;
    
    bool isLast;
    while (true) {
        glyphData parentSoFar(points, allContourEndIndices);
        glyphData cGlyph = readComponentGlyph(file, countInd, parentSoFar, isLast);
        int indexOffset = 0;
        for (glyphPoint point : points) {
            if (point.id >= 0) indexOffset++;
        }
        points.append_range(cGlyph.points);

        
        for (auto endIndex : cGlyph.contourEndIndices) {
            allContourEndIndices.emplace_back(endIndex + indexOffset);
        }

        if (isLast) break;
    }

    recalculateLegitPointIDs(&points); // Have to override simpleGlyph id's based on position in compoundGlyph

    return glyphData(points, allContourEndIndices);
}

