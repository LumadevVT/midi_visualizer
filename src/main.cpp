#include <cstdint>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <bitset>

#include "endian_utils.h"

namespace fs = std::filesystem;

struct MidiFile {
    enum class Format : uint16_t {
        SingleTrack,
        MultiTrack,
    };

    struct HeaderChunk {
        char str[5] = {};
        uint32_t length = 0;
        Format format = Format::SingleTrack;
        uint16_t numTracks = 1;
        std::bitset<sizeof(int16_t) * 8> division = 0;
    };

    struct TrackChunk {};

    fs::path filePath;
    HeaderChunk header;
    std::vector<TrackChunk> tracks;
};

std::ostream& operator<<(std::ostream& os, const MidiFile& midiFile) {
    os << "Head String: " << midiFile.header.str << '\n';
    os << "Length: " << midiFile.header.length << '\n';
    os << "Format: " << static_cast<uint16_t>(midiFile.header.format) << '\n';
    os << "Number of tracks: " << midiFile.header.numTracks << '\n';
    os << "division: " << midiFile.header.division << '\n';

    return os;
}


std::istream& operator>>(std::istream& inputStream, MidiFile& midiFile) {
    uint16_t format = 0;
    int16_t division = 0;
    inputStream.read(midiFile.header.str, 4);

    inputStream.read(reinterpret_cast<char*>(&midiFile.header.length), sizeof(midiFile.header.length));
    midiFile.header.length = swap_uint32(midiFile.header.length);

    inputStream.read(reinterpret_cast<char*>(&format), sizeof(format));
    midiFile.header.format = static_cast<MidiFile::Format>(swap_uint16(format));

    inputStream.read(reinterpret_cast<char*>(&midiFile.header.numTracks), sizeof(midiFile.header.numTracks));
    midiFile.header.numTracks = swap_uint16(midiFile.header.numTracks);

    inputStream.read(reinterpret_cast<char*>(&division), sizeof(division));
    midiFile.header.division = swap_int16(division);
    return inputStream;
}

MidiFile ReadMidiFile(const char* filePath) {
    std::ifstream file(filePath, std::ios::binary | std::ios::in);
    if (!file.is_open()) {
        return {};
    }

    MidiFile midi = {};
    midi.filePath = filePath;

    file >> midi;

    return midi;
}

int main() {
    std::cout << "Hello, World!" << std::endl;

    const MidiFile file = ReadMidiFile("assets/music-box-7.mid");
    std::cout << file << std::endl;

    return 0;
}
