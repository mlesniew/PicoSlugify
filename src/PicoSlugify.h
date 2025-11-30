#pragma once

#include <Arduino.h>

#ifndef PICOSLUGIFY_UNIDECODE_DISABLE_DEFAULT
#define PICOSLUGIFY_UNIDECODE_GERMAN
#define PICOSLUGIFY_UNIDECODE_POLISH
#endif

namespace PicoSlugify {

// Decode a UTF-8 encoded string to a Unicode codepoint
uint32_t decode_utf8(const char * str, size_t & index);

// Convert a Unicode codepoint to its closest ASCII representation
uint32_t unidecode(const uint32_t codepoint);

// Generate a slug from the input string
String slugify(const String & input, const char replacement = '_', bool merge_consecutive = true);

}