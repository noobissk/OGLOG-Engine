#pragma once
#include <fstream>
#include <string>
using namespace std;

static uint8_t readUInt8(std::ifstream& f) {
    uint8_t byte;
    f.read(reinterpret_cast<char*>(&byte), 1);
    return byte;
}
static uint16_t readUInt16(std::ifstream& f) {
    uint8_t bytes[2];
    f.read(reinterpret_cast<char*>(bytes), 2);
    return (bytes[0] << 8) | bytes[1];
}
static uint32_t readUInt32(std::ifstream& f) {
    uint8_t bytes[4];
    f.read(reinterpret_cast<char*>(bytes), 4);
    return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}
static string read4Char(std::ifstream& f) {
    char bytes[4];
    f.read(bytes, 4);
    return string(bytes, 4);
}

static float readF2DOT14(std::ifstream& f) {
    // Read as signed 16-bit big-endian
    uint8_t bytes[2];
    f.read(reinterpret_cast<char*>(bytes), 2);

    int16_t raw = (bytes[0] << 8) | bytes[1];  // sign preserved because raw is int16_t

    // Convert fixed-point 2.14 to float
    return raw / 16384.0f;  // 2^14
}




static int8_t readInt8(std::ifstream& f) {
    uint8_t raw = readUInt16(f);
    return static_cast<int8_t>(raw);
}
static int16_t readInt16(std::ifstream& f) {
    uint16_t raw = readUInt16(f);
    return static_cast<int16_t>(raw);
}
static int32_t readInt32(std::ifstream& f) {
    uint32_t raw = readUInt16(f);
    return static_cast<int32_t>(raw);
}