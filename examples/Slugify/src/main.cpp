#include <Arduino.h>
#include <PicoSlugify.h>

void setup() {
  Serial.begin(115200);
  delay(5000);

  const char* input =
      "Zażółć gęślą jaźń";  // Polish pangram with various diacritics
  String slug = PicoSlugify::slugify(input, '-');

  Serial.println("=== PicoSlugify: Basic (slugify) ===");
  Serial.print("Input: ");
  Serial.println(input);
  Serial.print("Slug:  ");
  Serial.println(slug);
}

void loop() { delay(1000); }
