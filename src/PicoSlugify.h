#pragma once

#include <Arduino.h>

#ifndef PICOSLUGIFY_UNIDECODE_DISABLE_DEFAULT
#define PICOSLUGIFY_UNIDECODE_GERMAN
#define PICOSLUGIFY_UNIDECODE_POLISH
#endif

namespace PicoSlugify {

// Decode a UTF-8 encoded string to a Unicode codepoint
uint32_t decodeUtf8(const char* str, size_t& index);

// Convert a Unicode codepoint to its closest ASCII representation
uint32_t unidecode(const uint32_t codepoint);

// Generate a slug from the input string
String slugify(const char* input, const char replacement = '_',
               bool mergeConsecutive = true);
String slugify(const String& input, const char replacement = '_',
               bool mergeConsecutive = true);

// Check if given char or string is a slug already
bool isSlug(const char c, const char replacement = '_');
bool isSlug(const char* s, const char replacement = '_');
bool isSlug(const String& s, const char replacement = '_');

}  // namespace PicoSlugify
