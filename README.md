# PicoSlugify

A lightweight Arduino framework library for converting strings into URL-friendly slugs.

## Features

- Simple and fast slug generation
- Removes special characters and spaces
- Converts to lowercase
- Converts Unicode characters to their closest ASCII equivalent

## Usage

PicoSlugify is super easy to use. Include the header, call a function, done!

### `slugify(...)`

```cpp
#include <Arduino.h>
#include <PicoSlugify.h>

void setup() {
    Serial.begin(115200);
    Serial.println(PicoSlugify::slugify("Zażółć gęślą jaźń", '-'));
    // zazolc-gesla-jazn
}
```

### `isSlug(...)`

```cpp
#include <Arduino.h>
#include <PicoSlugify.h>

void setup() {
    Serial.begin(115200);
    Serial.println(PicoSlugify::isSlug("already-slug-123", '-'));  // 1
    Serial.println(PicoSlugify::isSlug("Not a slug", '-'));        // 0
}
```

### `unidecode(...)`

```cpp
#include <Arduino.h>
#include <PicoSlugify.h>

void setup() {
    Serial.begin(115200);
    Serial.println(PicoSlugify::unidecode("Grüße aus Łódź"));
    // Gruse aus Lodz
}
```

## Language support

Language mapping is controlled with preprocessor flags set in your `platformio.ini`.

- By default, support for **German** and **Polish** is enabled.
- To take full control, disable defaults in project settings:

```ini
[env:your_env]
build_flags =
    -D PICOSLUGIFY_UNIDECODE_DISABLE_DEFAULT
```

- Then enable only the languages you want:

```ini
[env:your_env]
build_flags =
    -D PICOSLUGIFY_UNIDECODE_DISABLE_DEFAULT
    -D PICOSLUGIFY_UNIDECODE_GERMAN
    ; -D PICOSLUGIFY_UNIDECODE_POLISH
```

Currently supported languages:

- **German** (`PICOSLUGIFY_UNIDECODE_GERMAN`)
- **Polish** (`PICOSLUGIFY_UNIDECODE_POLISH`)

## Examples

- `examples/Basic` — demonstrates `slugify(...)`
- `examples/Unidecode` — demonstrates string-level `unidecode(...)`
- `examples/IsSlug` — demonstrates `isSlug(...)`