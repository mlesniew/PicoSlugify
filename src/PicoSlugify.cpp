#include <Arduino.h>
#include "PicoSlugify.h"

namespace PicoSlugify {

uint32_t decode_utf8(const char * str, size_t & index) {
    // TODO: Check for string terminator in multibyte sequences

    uint32_t codepoint = 0;
    uint8_t byte = static_cast<uint8_t>(str[index]);

    if (byte <= 0x7F) {
        // 1-byte sequence
        codepoint = byte;
        index += 1;
    } else if ((byte & 0xE0) == 0xC0) {
        // 2-byte sequence
        codepoint = byte & 0x1F;
        index += 1;
        codepoint = (codepoint << 6) | (static_cast<uint8_t>(str[index]) & 0x3F);
        index += 1;
    } else if ((byte & 0xF0) == 0xE0) {
        // 3-byte sequence
        codepoint = byte & 0x0F;
        index += 1;
        codepoint = (codepoint << 6) | (static_cast<uint8_t>(str[index]) & 0x3F);
        index += 1;
        codepoint = (codepoint << 6) | (static_cast<uint8_t>(str[index]) & 0x3F);
        index += 1;
    } else if ((byte & 0xF8) == 0xF0) {
        // 4-byte sequence
        codepoint = byte & 0x07;
        index += 1;
        codepoint = (codepoint << 6) | (static_cast<uint8_t>(str[index]) & 0x3F);
        index += 1;
        codepoint = (codepoint << 6) | (static_cast<uint8_t>(str[index]) & 0x3F);
        index += 1;
        codepoint = (codepoint << 6) | (static_cast<uint8_t>(str[index]) & 0x3F);
        index += 1;
    } else {
        // Invalid UTF-8 byte
        index += 1; // Skip invalid byte
    }

    return codepoint;
}

uint32_t unidecode(const uint32_t codepoint) {
    switch (codepoint) {
#ifdef PICOSLUGIFY_UNIDECODE_GERMAN
        case 0x00E4: return 'a'; // ä
        case 0x00C4: return 'A'; // Ä
        case 0x00F6: return 'o'; // ö
        case 0x00D6: return 'O'; // Ö
        case 0x00FC: return 'u'; // ü
        case 0x00DC: return 'U'; // Ü
        case 0x00DF: return 's'; // ß
#endif
#ifdef PICOSLUGIFY_UNIDECODE_POLISH
        case 0x0105: return 'a'; // ą
        case 0x0104: return 'A'; // Ą
        case 0x0119: return 'e'; // ę
        case 0x0118: return 'E'; // Ę
        case 0x0142: return 'l'; // ł
        case 0x0141: return 'L'; // Ł
        case 0x00F3: return 'o'; // ó
        case 0x00D3: return 'O'; // Ó
        case 0x015B: return 's'; // ś
        case 0x015A: return 'S'; // Ś
        case 0x017C: return 'z'; // ź
        case 0x017B: return 'Z'; // Ź
        case 0x017A: return 'z'; // ż
        case 0x0179: return 'Z'; // Ż
#endif
        default:
            return codepoint;
    }
}

String slugify(const String & input, const char replacement, bool merge_consecutive) {
    // TODO: Consider in-place replacement to save memory
    char output_buffer[input.length() + 1];

    // TODO: Don't emit leading/trailing replacements (optionally)
    bool last_was_replacement = false;
    size_t input_index = 0;
    size_t output_index = 0;

    while (true) {
        uint32_t codepoint = decode_utf8(input.c_str(), input_index);

        if (!codepoint) {
            break; // End of string or error
        }

        // Apply unidecode
        codepoint = unidecode(codepoint);

        // Convert to lowercase
        if (codepoint >= 'A' && codepoint <= 'Z') {
            codepoint ^= ' ';  // ASCII lowercase conversion
        }

        // Check if alphanumeric
        if ((codepoint >= 'a' && codepoint <= 'z') || (codepoint >= '0' && codepoint <= '9')
                || (codepoint == uint32_t(replacement))) {
            output_buffer[output_index++] = static_cast<char>(codepoint);
            last_was_replacement = false;
        } else {
            // Replace non-alphanumeric with hyphen
            if (!merge_consecutive || !last_was_replacement) {
                output_buffer[output_index++] = replacement;
            }
            last_was_replacement = true;
        }
    }

    output_buffer[output_index] = '\0';

    return String(output_buffer);
}

}