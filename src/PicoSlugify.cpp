#include "PicoSlugify.h"

#include <Arduino.h>

#include <cstring>

namespace PicoSlugify {

static inline bool isContinuationByte(const uint8_t byte) {
  return (byte & 0xC0) == 0x80;
}

uint32_t decodeUtf8(const char* str, size_t& index) {
  uint32_t codepoint = 0;
  const uint8_t byte = static_cast<uint8_t>(str[index]);

  if (!byte) {
    return 0;
  }

  if (byte <= 0x7F) {
    // 1-byte sequence
    codepoint = byte;
    index += 1;
  } else if ((byte & 0xE0) == 0xC0) {
    // 2-byte sequence
    const uint8_t b1 = static_cast<uint8_t>(str[index + 1]);
    if (!b1 || !isContinuationByte(b1)) {
      index += 1;
      return 0xFFFD;
    }
    codepoint = byte & 0x1F;
    codepoint = (codepoint << 6) | (b1 & 0x3F);
    index += 2;
  } else if ((byte & 0xF0) == 0xE0) {
    // 3-byte sequence
    const uint8_t b1 = static_cast<uint8_t>(str[index + 1]);
    const uint8_t b2 = static_cast<uint8_t>(str[index + 2]);
    if (!b1 || !b2 || !isContinuationByte(b1) || !isContinuationByte(b2)) {
      index += 1;
      return 0xFFFD;
    }
    codepoint = byte & 0x0F;
    codepoint = (codepoint << 6) | (b1 & 0x3F);
    codepoint = (codepoint << 6) | (b2 & 0x3F);
    index += 3;
  } else if ((byte & 0xF8) == 0xF0) {
    // 4-byte sequence
    const uint8_t b1 = static_cast<uint8_t>(str[index + 1]);
    const uint8_t b2 = static_cast<uint8_t>(str[index + 2]);
    const uint8_t b3 = static_cast<uint8_t>(str[index + 3]);
    if (!b1 || !b2 || !b3 || !isContinuationByte(b1) ||
        !isContinuationByte(b2) || !isContinuationByte(b3)) {
      index += 1;
      return 0xFFFD;
    }
    codepoint = byte & 0x07;
    codepoint = (codepoint << 6) | (b1 & 0x3F);
    codepoint = (codepoint << 6) | (b2 & 0x3F);
    codepoint = (codepoint << 6) | (b3 & 0x3F);
    index += 4;
  } else {
    // Invalid UTF-8 byte
    index += 1;  // Skip invalid byte
    codepoint = 0xFFFD;
  }

  return codepoint;
}

uint32_t unidecode(const uint32_t codepoint) {
  switch (codepoint) {
#ifdef PICOSLUGIFY_UNIDECODE_GERMAN
    case 0x00E4:
      return 'a';  // ä
    case 0x00C4:
      return 'A';  // Ä
    case 0x00F6:
      return 'o';  // ö
    case 0x00D6:
      return 'O';  // Ö
    case 0x00FC:
      return 'u';  // ü
    case 0x00DC:
      return 'U';  // Ü
    case 0x00DF:
      return 's';  // ß
#endif
#ifdef PICOSLUGIFY_UNIDECODE_POLISH
    case 0x0105:
      return 'a';  // ą
    case 0x0104:
      return 'A';  // Ą
    case 0x0107:
      return 'c';  // ć
    case 0x0106:
      return 'C';  // Ć
    case 0x0119:
      return 'e';  // ę
    case 0x0118:
      return 'E';  // Ę
    case 0x0142:
      return 'l';  // ł
    case 0x0141:
      return 'L';  // Ł
    case 0x0144:
      return 'n';  // ń
    case 0x0143:
      return 'N';  // Ń
    case 0x00F3:
      return 'o';  // ó
    case 0x00D3:
      return 'O';  // Ó
    case 0x015B:
      return 's';  // ś
    case 0x015A:
      return 'S';  // Ś
    case 0x017C:
      return 'z';  // ź
    case 0x017B:
      return 'Z';  // Ź
    case 0x017A:
      return 'z';  // ż
    case 0x0179:
      return 'Z';  // Ż
#endif
    default:
      return codepoint;
  }
}

String unidecode(const char* input) {
  if (!input) {
    return String();
  }

  String output;
  output.reserve(strlen(input) + 1);

  size_t inputIndex = 0;

  while (true) {
    uint32_t codepoint = decodeUtf8(input, inputIndex);

    if (!codepoint) {
      break;
    }

    codepoint = unidecode(codepoint);

    if (codepoint <= 0x7F) {
      output += static_cast<char>(codepoint);
    }
  }

  return output;
}

String unidecode(const String& input) { return unidecode(input.c_str()); }

String slugify(const char* input, const char replacement,
               bool mergeConsecutive) {
  if (!input) {
    return String();
  }

  String output;
  output.reserve(strlen(input) + 1);

  // TODO: Don't emit leading/trailing replacements (optionally)
  bool last_was_replacement = false;
  size_t input_index = 0;

  while (true) {
    uint32_t codepoint = decodeUtf8(input, input_index);

    if (!codepoint) {
      break;  // End of string or error
    }

    // Apply unidecode
    codepoint = unidecode(codepoint);

    // Convert to lowercase
    if (codepoint >= 'A' && codepoint <= 'Z') {
      codepoint ^= ' ';  // ASCII lowercase conversion
    }

    // Check if alphanumeric
    if ((codepoint >= 'a' && codepoint <= 'z') ||
        (codepoint >= '0' && codepoint <= '9') ||
        (codepoint == uint32_t(replacement))) {
      output += static_cast<char>(codepoint);
      last_was_replacement = false;
    } else {
      // Replace non-alphanumeric with hyphen
      if (!mergeConsecutive || !last_was_replacement) {
        output += replacement;
      }
      last_was_replacement = true;
    }
  }

  return output;
}

String slugify(const String& input, const char replacement,
               bool mergeConsecutive) {
  return slugify(input.c_str(), replacement, mergeConsecutive);
}

bool isSlug(const char c, const char replacement) {
  return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || (c == replacement);
}

bool isSlug(const char* s, const char replacement) {
  if (!s) {
    return false;
  }

  for (; *s; ++s) {
    if (!isSlug(*s, replacement)) {
      return false;
    }
  }
  return true;
}

bool isSlug(const String& s, const char replacement) {
  return isSlug(s.c_str(), replacement);
}

}  // namespace PicoSlugify
