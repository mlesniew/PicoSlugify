# PicoSlugify

A lightweight Arduino framework library for converting strings into URL-friendly slugs.

## Features

- Simple and fast slug generation
- Removes special characters and spaces
- Converts to lowercase
- Converts Unicode characters to their closest ASCII equivalent

## Usage

```cpp
#include <Arduino.h>
#include <PicoSlugify.h>

void setup() {
    Serial.begin();
    Serial.println(PicoSlugify::slugify("Żółć_#3"));  // prints zolc_3
}
```