#include <Arduino.h>
#include <PicoSlugify.h>

void setup() {
    Serial.begin(115200);
    delay(5000);

    const char * input = "Zażółć gęślą jaźń + Grüße aus Łódź";
    const String output = PicoSlugify::unidecode(input);

    Serial.println("=== PicoSlugify: Unidecode ===");
    Serial.print("Input:     ");
    Serial.println(input);
    Serial.print("Unidecode: ");
    Serial.println(output);
}

void loop() { delay(1000); }
