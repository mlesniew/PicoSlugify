#include <Arduino.h>
#include <PicoSlugify.h>

void printSlugCheck(const char * value) {
    Serial.print("\"");
    Serial.print(value);
    Serial.print("\" -> ");
    Serial.println(PicoSlugify::isSlug(value, '-') ? "slug" : "not slug");
}

void setup() {
    Serial.begin(115200);
    delay(5000);

    Serial.println("=== PicoSlugify: isSlug ===");

    printSlugCheck("already-slugified-123");
    printSlugCheck("Not slugified yet");
    printSlugCheck("zazolc-gesla-jazn");
    printSlugCheck("contains_underscore");
}

void loop() { delay(1000); }
