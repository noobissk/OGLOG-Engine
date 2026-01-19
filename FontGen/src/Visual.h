#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include "FileParser.h"

using namespace std;
using namespace cv;

cv::Mat GenerateGlyphSDF(const glyphData& g, int sdfSize = 128)
{
    float spread = 8.0f;

    float overallMinX = g.bounds.overallMinX;
    float overallMaxX = g.bounds.overallMaxX;
    float overallMinY = g.bounds.overallMinY;
    float overallMaxY = g.bounds.overallMaxY;

    int pad = int(std::round(spread));
    float glyphPixelArea = sdfSize - 2 * pad;

    cv::Mat sdf(sdfSize, sdfSize, CV_32FC1, cv::Scalar(1e6f));

    float oglyphW = overallMaxX - overallMinX;
    float oglyphH = overallMaxY - overallMinY;

    float scale = std::min(glyphPixelArea / oglyphW,
                           glyphPixelArea / oglyphH);

    float offsetX = pad + (glyphPixelArea - oglyphW * scale) * 0.5f;
    float offsetY = pad + (glyphPixelArea - oglyphH * scale) * 0.5f;

    std::vector<std::vector<cv::Point2f>> polyLines;

    for (const bezierCurve& curve : g.curves)
    {
        auto* pts = curve.generateRenderPoints(4);
        if (!pts || pts->empty()) continue;

        std::vector<cv::Point2f> line;
        for (const auto& v : *pts)
        {
            float px = float(v.x - overallMinX) * scale + offsetX;
            float py = float(v.y - overallMinY) * scale + offsetY;
            py = sdfSize - py - 1;

            line.emplace_back(px, py);
        }
        polyLines.push_back(std::move(line));
    }

    // Compute **signed distance**
    for (int y = 0; y < sdfSize; y++)
    {
        for (int x = 0; x < sdfSize; x++)
        {
            cv::Point2f p(x + 0.5f, y + 0.5f);
            float minDist = spread + 1.0f;  // Early termination threshold

            // Distance to nearest polyline
            for (const auto& line : polyLines)
            {
                for (size_t i = 0; i < line.size() - 1; i++)
                {
                    cv::Point2f a = line[i], b = line[i + 1];
                    cv::Point2f ab = b - a;
                    float abLenSq = ab.dot(ab);
                    float t = std::clamp((p - a).dot(ab) / abLenSq, 0.f, 1.f);
                    cv::Point2f proj = a + t * ab;
                    float dist = cv::norm(p - proj);
                    if (dist < minDist) minDist = dist;
                    // Early exit if very close to outline
                    if (minDist < 0.1f) break;
                }
                if (minDist < 0.1f) break;
            }

            // Determine inside/outside using winding number
            int winding = 0;
            for (const auto& line : polyLines)
            {
                for (size_t i = 0; i < line.size() - 1; i++)
                {
                    cv::Point2f a = line[i], b = line[i + 1];
                    if (a.y <= p.y)
                    {
                        if (b.y > p.y && ((b - a).cross(p - a) > 0)) winding++;
                    }
                    else
                    {
                        if (b.y <= p.y && ((b - a).cross(p - a) < 0)) winding--;
                    }
                }
            }
            if (winding != 0) minDist = -minDist; // negative inside

            sdf.at<float>(y, x) = minDist;
        }
    }

    // Normalize SDF: map [-spread, +spread] → [0, 255]
    cv::Mat sdfImg;
    sdf.convertTo(sdfImg, CV_8UC1, 255.0f / (2.0f * spread), 127.5f);
    cv::threshold(sdfImg, sdfImg, 255, 255, cv::THRESH_TRUNC);
    cv::threshold(sdfImg, sdfImg, 0, 0, cv::THRESH_TOZERO);

    return sdfImg;
}


// cv::Mat GenerateGlyphAtlas(
//     const std::vector<glyphData>& glyphs,
//     int glyphCount,
//     int resolutionPerGlyph     // e.g. 128
// )
// {
//     if (glyphCount <= 0 || glyphs.empty())
//         return cv::Mat();
//     int cols = ceil(sqrt((double)glyphCount));
//     int rows = ceil(glyphCount / (double)cols);
//     // Final atlas size
//     int atlasW = cols * resolutionPerGlyph;
//     int atlasH = rows * resolutionPerGlyph;
//     cv::Mat atlas(atlasH, atlasW, CV_8UC1, cv::Scalar(0));
//     #pragma omp parallel for collapse(2) schedule(dynamic)
//     for (int row = 0; row < rows; row++)
//     {
//         for (int col = 0; col < cols; col++)
//         {
//             int i = row * cols + col;
//             if (i >= glyphCount) continue;
//             const glyphData& g = glyphs[i];
//             cv::Mat sdf = GenerateGlyphSDF(g, resolutionPerGlyph);
//             if (sdf.empty()) continue;
//             // Compute target cell location in atlas
//             int cellX = col * resolutionPerGlyph;
//             int cellY = row * resolutionPerGlyph;
//             // Copy glyph SDF into atlas cell
//             sdf.copyTo(atlas(cv::Rect(cellX, cellY, resolutionPerGlyph, resolutionPerGlyph)));
//             // Log progress only occasionally
//             if ((i + 1) % 50 == 0) {
//                 cout << "Rendered " << (i + 1) << " / " << glyphCount << " glyphs\n";
//             }
//         }
//     }
//     cout << "Atlas complete: " << glyphCount << " glyphs rendered\n";
//     return atlas;
// }

struct AtlasResult
{
    std::vector<cv::Mat> atlases;
    std::vector<glyphRecord> records;
};

AtlasResult GenerateSDFAtlases(
    const std::vector<glyphData>& glyphs,
    const std::vector<glyphRecord>& baseRecords
)
{
    AtlasResult result;
    result.records = baseRecords;

    int totalGlyphs = (int)std::min(glyphs.size(), baseRecords.size());
    int atlasCount = (totalGlyphs + GLYPHS_PER_ATLAS - 1) / GLYPHS_PER_ATLAS;

    result.atlases.resize(atlasCount);

    // Pre-generate all glyph SDFs in parallel using actual glyph IDs from records
    std::vector<cv::Mat> glyphSDFs(totalGlyphs);
    
    #pragma omp parallel for schedule(dynamic) num_threads(4)
    for (int i = 0; i < totalGlyphs; i++)
    {
        // Use the actual glyph_id from the record, not the sequential index
        uint32_t glyphId = result.records[i].glyph_id;
        if (glyphId < glyphs.size())
        {
            glyphSDFs[i] = GenerateGlyphSDF(glyphs[glyphId], CELL_SIZE);
            if ((i+1) % 25 == 0)
                std::cout << "Generated SDFs: " << (i + 1) << " / " << totalGlyphs << "\n";
        }
    }
    std::cout << "Generated SDFs: " << totalGlyphs << " / " << totalGlyphs << "\n";
    std::cout << "SDF generation complete. Packing into atlases...\n";

    for (int a = 0; a < atlasCount; a++)
    {
        result.atlases[a] = cv::Mat(ATLAS_SIZE, ATLAS_SIZE, CV_8UC1, cv::Scalar(0));

        for (int i = 0; i < GLYPHS_PER_ATLAS; i++)
        {
            int recordIndex = a * GLYPHS_PER_ATLAS + i;
            if (recordIndex >= totalGlyphs) break;

            int col = i % COLS;
            int row = i / COLS;

            int x = col * CELL_SIZE;
            int y = row * CELL_SIZE;

            if (glyphSDFs[recordIndex].empty()) continue;

            glyphSDFs[recordIndex].copyTo(result.atlases[a](cv::Rect(x, y, CELL_SIZE, CELL_SIZE)));

            float inv = 1.0f / ATLAS_SIZE;

            glyphRecord& r = result.records[recordIndex];
            r.atlasIndex = (uint8_t)a;
            r.u0 = x * inv;
            r.v0 = y * inv;
            r.u1 = (x + CELL_SIZE) * inv;
            r.v1 = (y + CELL_SIZE) * inv;
        }
        std::cout << "Generated atlas " << (a + 1) << " / " << atlasCount << "\n";
    }

    return result;
}

void SaveAtlases(const std::vector<cv::Mat>& atlases, const std::string& folder)
{
    for (int i = 0; i < atlases.size(); i++)
    {
        std::string path = folder + "/atlas_" + std::to_string(i) + ".png";
        cv::imwrite(path, atlases[i]);
    }
}