#include <RemotePrint.h>

#define LOG_OUTPUT "SERIAL_TELNET" // "SERIAL", "TELNET", "SERIAL_TELNET", "NONE"
#define LOG_LEVEL "DEBUG"          // "ANY", "VERBOSE", "DEBUG", "INFO", "WARNING", "ERROR", "PROFILER"
#define SERIAL_BAND 115200

unsigned long lastPrintMillis = 0;

void setup() {}

void loop() {
  if (millis() - lastPrintMillis > 5000) {
    PRINTLN("TEST INFO LOG message");
    PRINTLN_D("TEST DEBUG LOG message");
    PRINTLN_W("TEST WARNING LOG message");
    PRINTLN_E("TEST ERROR LOG message");
    lastPrintMillis = millis();
  }

  REMOTE_PRINT_HANDLER;
}
