#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "FileParser.h"
#include "Visual.h"
#include <math.h>
#include <cstdint>
#include <limits>

using namespace std;

double scale = 0.2;
int displayWidth = 800;
int displayHeight = 800;
int subdivision = 3;

struct HMetric {
    uint16_t advance;
    int16_t lsb;
};
std::vector<HMetric> readHMTX(
    std::ifstream& file,
    const HHEA& hhea,
    uint16_t numGlyphs
) {
    std::vector<HMetric> metrics(numGlyphs);

    file.seekg(subTableMap["hmtx"].offset, std::ios::beg);

    for (int i = 0; i < hhea.numberOfHMetrics; i++) {
        metrics[i].advance = readUInt16(file);
        metrics[i].lsb     = readInt16(file);
    }

    uint16_t lastAdvance = metrics[hhea.numberOfHMetrics - 1].advance;
    for (int i = hhea.numberOfHMetrics; i < numGlyphs; i++) {
        metrics[i].advance = lastAdvance;
        metrics[i].lsb     = readInt16(file);
    }

    return metrics;
}

HHEA readHHEA(ifstream& file) {
    HHEA h;
    file.seekg(subTableMap["hhea"].offset, ios::beg);

    h.majorVersion = readUInt16(file);
    h.minorVersion = readUInt16(file);
    h.ascender = readInt16(file);
    h.descender = readInt16(file);
    h.lineGap = readInt16(file);
    h.advanceWidthMax = readUInt16(file);
    h.minLeftSideBearing = readInt16(file);
    h.minRightSideBearing = readInt16(file);
    h.xMaxExtent = readInt16(file);
    h.caretSlopeRise = readInt16(file);
    h.caretSlopeRun = readInt16(file);
    h.caretOffset = readInt16(file);
    file.seekg(8, ios::cur); // reserved fields
    h.metricDataFormat = readInt16(file);
    h.numberOfHMetrics = readUInt16(file);

    return h;
}


struct sdfm_glyph {
    uint32_t unicode;
    uint32_t glyph_id;

    float advance;
    float bearingX;
    float bearingY;
    float width;
    float height;
};
struct sdfm_header {
    uint32_t magic;
    uint32_t version;
    uint32_t font_res;
    uint32_t glyph_count;
    float unitsPerEm;
    float ascender;
    float descender;
};
void SaveGlyphMappings(
    const std::string& path,
    const std::vector<glyphRecord>& record,
    uint32_t font_res,
    float unitsPerEm,
    float ascender,
    float descender
)
{
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open file for writing: " + path);
    }

    sdfm_header header;

    header.magic       = 0x4D464453;
    header.version     = 1;
    header.font_res    = font_res;
    header.glyph_count = static_cast<uint32_t>(record.size());
    header.unitsPerEm  = unitsPerEm;
    header.ascender    = ascender;
    header.descender   = descender;

    out.write(reinterpret_cast<const char*>(&header), sizeof(header));

    for (const auto& g : record) {
        sdfm_glyph bin{};
        bin.unicode  = g.unicode;
        bin.glyph_id = g.glyph_id;
        bin.advance  = g.advance;
        bin.bearingX = g.bearingX;
        bin.bearingY = g.bearingY;
        bin.width    = g.width;
        bin.height   = g.height;

        out.write(reinterpret_cast<const char*>(&bin), sizeof(bin));
    }
}


static vector<unsigned int> getAllGlyphLocations(ifstream& file) {
    file.seekg(subTableMap["maxp"].offset + 4);
    int numGlyphs = readUInt16(file);

    file.seekg(subTableMap["head"].offset + 50);

    bool is2BytesEntry = readInt16(file) == 0;

    unsigned int locationTableStart = subTableMap["loca"].offset;
    unsigned int glyphTableStart = subTableMap["glyf"].offset;
    vector<unsigned int> allGlyphLocations(numGlyphs);

    for (int i = 0; i < numGlyphs; i++) {
        file.seekg(locationTableStart + i * (is2BytesEntry ? 2 : 4));

        unsigned int glyphDataOffset = is2BytesEntry ? readUInt16(file) * 2u : readUInt32(file);
        allGlyphLocations[i] = glyphTableStart + glyphDataOffset;
    }

    return allGlyphLocations;
}

vector<glyphRecord> getUnicodeToGlyphIndexMappings(ifstream& file) {
    vector<glyphRecord> record;

    const uint32_t cmapTableStart = subTableMap["cmap"].offset;
    file.seekg(cmapTableStart, ios::beg);

    uint16_t version = readUInt16(file);
    uint16_t numSubtables = readUInt16(file);

    uint32_t chosenSubtableOffset = std::numeric_limits<uint32_t>::max();
    int selected_unicode_version_id = -1;
    for (int i = 0; i < numSubtables; ++i) {
        uint16_t platform_id = readUInt16(file);
        uint16_t platform_specific_id = readUInt16(file);
        uint32_t offset = readUInt32(file);

        if (platform_id == 0) {
            if ((int)platform_specific_id > selected_unicode_version_id) {
                selected_unicode_version_id = platform_specific_id;
                chosenSubtableOffset = offset;
            }
        } else if (platform_id == 3 && chosenSubtableOffset == std::numeric_limits<uint32_t>::max()) {
            if (platform_specific_id == 1 || platform_specific_id == 10) {
                chosenSubtableOffset = offset;
            }
        }
    }

    if (chosenSubtableOffset == std::numeric_limits<uint32_t>::max()) {
        throw runtime_error("Font does not contain supported cmap subtable (no usable Unicode/MS encoding found).");
    }

    file.seekg(cmapTableStart + chosenSubtableOffset, ios::beg);
    uint16_t format = readUInt16(file);

    if (format == 0) {
        throw runtime_error("cmap format 0 not supported by this parser.");
    }

    if (format == 4) {
        uint16_t length = readUInt16(file);
        uint16_t language = readUInt16(file);
        uint16_t segCountX2 = readUInt16(file);
        uint16_t seg_count = segCountX2 / 2;
        file.seekg(6, ios::cur);

        long long subtableStart = static_cast<long long>(cmapTableStart + chosenSubtableOffset);

        vector<uint16_t> end_codes(seg_count);
        for (size_t i = 0; i < seg_count; ++i) end_codes[i] = readUInt16(file);

        uint16_t reservedPad = readUInt16(file);

        vector<uint16_t> start_codes(seg_count);
        for (size_t i = 0; i < seg_count; ++i) start_codes[i] = readUInt16(file);

        vector<int16_t> id_deltas(seg_count);
        for (size_t i = 0; i < seg_count; ++i) id_deltas[i] = static_cast<int16_t>(readUInt16(file)); // signed

        vector<uint16_t> idRangeOffsets(seg_count);
        long long idRangeOffsetsArrayPos = static_cast<long long>(file.tellg());
        for (size_t i = 0; i < seg_count; ++i) idRangeOffsets[i] = readUInt16(file);

        long long glyphIdArrayPos = idRangeOffsetsArrayPos + seg_count * 2;

        bool has_read_missing_char_glyph = false;

        for (size_t si = 0; si < seg_count; ++si) {
            uint16_t startCode = start_codes[si];
            uint16_t endCode = end_codes[si];
            if (startCode == 0xFFFF && endCode == 0xFFFF) {
                continue;
            }

            for (uint32_t code = startCode; code <= endCode; ++code) {
                uint32_t glyphIndex = 0;
                if (idRangeOffsets[si] == 0) {
                    glyphIndex = (static_cast<uint32_t>(code) + static_cast<int32_t>(id_deltas[si])) & 0xFFFF;
                } else {
                    long long idRangeOffsetWordAddr = idRangeOffsetsArrayPos + static_cast<long long>(2 * si);
                    long long glyphAddress = idRangeOffsetWordAddr + idRangeOffsets[si] + 2 * (code - startCode);

                    file.seekg(glyphAddress, ios::beg);
                    uint16_t glyphIdFromArray = readUInt16(file);
                    if (glyphIdFromArray == 0) {
                        glyphIndex = 0;
                    } else {
                        glyphIndex = (static_cast<uint32_t>(glyphIdFromArray) + static_cast<int32_t>(id_deltas[si])) & 0xFFFF;
                    }

                    file.seekg(glyphIdArrayPos + 0, ios::beg);
                }

                if (glyphIndex != 0) {
                    record.emplace_back((unsigned int)code, (unsigned int)glyphIndex);
                }
                has_read_missing_char_glyph |= (glyphIndex == 0);
            }
        }

        if (!has_read_missing_char_glyph) {
            record.emplace_back(65535u, 0u);
        }

        return record;
    }

    if (format == 12) {
        uint16_t reserved = readUInt16(file);
        uint32_t length = readUInt32(file);
        uint32_t language = readUInt32(file);
        uint32_t numGroups = readUInt32(file);

        bool has_read_missing_char_glyph = false;
        for (uint32_t g = 0; g < numGroups; ++g) {
            uint32_t start_char_code = readUInt32(file);
            uint32_t end_char_code = readUInt32(file);
            uint32_t start_glyph = readUInt32(file);

            uint32_t n = end_char_code - start_char_code + 1;
            for (uint32_t off = 0; off < n; ++off) {
                uint32_t char_code = start_char_code + off;
                uint32_t glyph_index = start_glyph + off;
                if (glyph_index != 0) {
                    record.emplace_back((unsigned int)char_code, (unsigned int)glyph_index);
                }
                has_read_missing_char_glyph |= (glyph_index == 0);
            }
        }

        if (!has_read_missing_char_glyph) {
            record.emplace_back(65535u, 0u);
        }

        return record;
    }

    // unsupported format
    throw runtime_error(string("Unsupported cmap format: ") + to_string(format));
}
glyphBoundaries recomputeBounds(const glyphData& g)
{
    glyphBoundaries b;
    b.xMin = b.yMin =  std::numeric_limits<float>::max();
    b.xMax = b.yMax = -std::numeric_limits<float>::max();

    for (const auto& curve : g.curves)
    {
        for (const auto& p : *curve.generateRenderPoints(4))
        {
            b.xMin = std::min(int(b.xMin), p.x);
            b.yMin = std::min(int(b.yMin), p.y);
            b.xMax = std::max(int(b.xMax), p.x);
            b.yMax = std::max(int(b.yMax), p.y);
        }
    }
    return b;
}
void SaveSDFData(
    const std::string& path,
    const std::vector<glyphRecord>& records,
    float ascender,
    float descender,
    float unitsPerEm
)
{
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Failed to open " + path);

    uint32_t glyphCount = (uint32_t)records.size();
    out.write((char*)&glyphCount, sizeof(uint32_t));

    out.write((char*)&ascender, sizeof(float));
    out.write((char*)&descender, sizeof(float));
    out.write((char*)&unitsPerEm, sizeof(float));

    for (const auto& g : records)
    {
        out.write((char*)&g.unicode, sizeof(uint32_t));
        out.write((char*)&g.glyph_id, sizeof(uint32_t));

        out.write((char*)&g.advance, sizeof(float));
        out.write((char*)&g.bearingX, sizeof(float));
        out.write((char*)&g.bearingY, sizeof(float));
        out.write((char*)&g.width, sizeof(float));
        out.write((char*)&g.height, sizeof(float));

        out.write((char*)&g.atlasIndex, sizeof(uint8_t));

        out.write((char*)&g.u0, sizeof(float));
        out.write((char*)&g.v0, sizeof(float));
        out.write((char*)&g.u1, sizeof(float));
        out.write((char*)&g.v1, sizeof(float));
    }
}

int main(int argc, char* argv[])
{
    if (argc == 2) {
        path = string(argv[1]);
    }
    else {
        cout << "Please input font filepath." << endl;
        cin >> path;
    }
    if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) {
        cout << "Invalid font filepath." << endl;
        return 1;
    }


    ifstream file(path, ios::binary);

    file.seekg(4);
    numTables      = readUInt16(file);
    searchRange    = readUInt16(file);
    entrySelector  = readUInt16(file);
    rangeShift     = readUInt16(file);
    
    cout << "[LOG] found '" << numTables << "' tables" << endl;

    ParseFont(file);
    
    file.seekg(subTableMap["maxp"].offset + 4); // skip unused: version
    int numGlyphs = readUInt16(file);
    auto hhea = readHHEA(file);
    auto hmtx = readHMTX(file, hhea, numGlyphs);
    file.seekg(subTableMap["head"].offset, ios::beg);
    file.seekg(18, ios::cur);
    uint16_t unitsPerEm = readUInt16(file);

    glyphOffsets = getAllGlyphLocations(file);


    int glyphCount = glyphOffsets.size();
    int glyphID;


    vector<glyphData> allGlyphs;

    allGlyphs.reserve(glyphCount);

    for (int i = 0; i < glyphCount; i++) {   
        glyphID = i;
    
        file.seekg(glyphOffsets[glyphID], ios::beg);
        int16_t countInd = readUInt16(file);
        int16_t xMin = readInt16(file);
        int16_t yMin = readInt16(file);
        int16_t xMax = readInt16(file);
        int16_t yMax = readInt16(file);


        glyphData glyph0;
        if (countInd >= 0)
            glyph0 = ReadSimpleGlyph(file, countInd, glyphID);
        else
            glyph0 = ReadCompoundGlyph(file, countInd, glyphID);

        glyph0.bounds = glyphBoundaries(xMin, yMin, xMax, yMax);
        glyph0.buildBeziersFromPoints();
        glyph0.bounds = recomputeBounds(glyph0);
        allGlyphs.emplace_back(glyph0);
    }
    int overallMinX = 0;
    int overallMinY = 0;
    int overallMaxX = 0;
    int overallMaxY = 0;
    for (const auto& g : allGlyphs) {
        if (g.bounds.xMin < overallMinX) overallMinX = g.bounds.xMin;
        if (g.bounds.yMin < overallMinY) overallMinY = g.bounds.yMin;
        if (g.bounds.xMax > overallMaxX) overallMaxX = g.bounds.xMax;
        if (g.bounds.yMax > overallMaxY) overallMaxY = g.bounds.yMax;

    }
    for (int i = 0; i < glyphCount; i++) {
        allGlyphs[i].bounds.overallMaxX = overallMaxX;
        allGlyphs[i].bounds.overallMaxY = overallMaxY;
        allGlyphs[i].bounds.overallMinX = overallMinX;
        allGlyphs[i].bounds.overallMinY = overallMinY;
    }
    // string outputPath = "../../output_atlas_" + to_string(glyphResolution) + ".png";
    // auto atlas = GenerateGlyphAtlas(allGlyphs, glyphCount, glyphResolution); // error here since I'm not using it anymore
    // cv::imwrite(outputPath, atlas);
    vector<glyphRecord> record = getUnicodeToGlyphIndexMappings(file);
    
    // Populate advance, bearing, and width/height from hmtx and glyph bounds
    for (auto& rec : record) {
        uint32_t glyphId = rec.glyph_id;
        if (glyphId < hmtx.size()) {
            rec.advance = hmtx[glyphId].advance / float(unitsPerEm);
            rec.bearingX = hmtx[glyphId].lsb / float(unitsPerEm);
        }
        
        if (glyphId < allGlyphs.size()) {
            const auto& bounds = allGlyphs[glyphId].bounds;
            float width = (bounds.xMax - bounds.xMin) / float(unitsPerEm);
            float height = (bounds.yMax - bounds.yMin) / float(unitsPerEm);
            rec.width = width;
            rec.height = height;
            rec.bearingY = (bounds.yMax) / float(unitsPerEm);
        }
    }
    
    AtlasResult result = GenerateSDFAtlases(allGlyphs, record);
    SaveAtlases(result.atlases, "../../");
    SaveSDFData(
        "../../output_font_" + to_string(CELL_SIZE) + ".sdfd",
        result.records,
        hhea.ascender,
        hhea.descender,
        float(unitsPerEm)
    );

    return 0;
}