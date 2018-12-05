#include "RemotePrint.h"

RemotePrint::RemotePrint(String  hostName,
                         String  telnetPassword,
                         uint8_t startingDebugLevel,
                         bool    serialEnabled,
                         bool    telnetEnabled) {
  if (telnetEnabled) {
    Debug.begin(hostName, startingDebugLevel);
    Debug.setSerialEnabled(serialEnabled);
    Debug.setResetCmdEnabled(true); // Enable the reset command

    if (telnetPassword.length() != 0) Debug.setPassword(telnetPassword);
  }
}

// Define the static members
RemotePrint *RemotePrint::s_instance = NULL;
uint8_t RemotePrint::logLevel        = 0;
bool    RemotePrint::serialEnabled   = false;
bool    RemotePrint::telnetEnabled   = false;

RemotePrint * RemotePrint::instance() {
  if (!s_instance) {
    String hostName = "telnet_hostname";
    #ifdef HOSTNAME
    hostName = HOSTNAME;
    #endif // ifdef HOSTNAME

    String telnetPassword = "";
    #ifdef TELNET_PASSWORD
    telnetPassword = TELNET_PASSWORD;
    #endif // ifdef TELNET_PASSWORD

    #ifndef LOG_OUTPUT
    # define LOG_OUTPUT "SERIAL_TELNET"
    #endif // ifndef LOG_OUTPUT

    logLevel      = RemoteDebug::INFO;
    serialEnabled = (strcasecmp(LOG_OUTPUT, "SERIAL") == 0 || strcasecmp(LOG_OUTPUT, "SERIAL_TELNET") == 0);
    telnetEnabled = (strcasecmp(LOG_OUTPUT, "TELNET") == 0 || strcasecmp(LOG_OUTPUT, "SERIAL_TELNET") == 0);

    #ifdef LOG_LEVEL

    if (strcasecmp(LOG_LEVEL, "ANY") == 0) logLevel = RemoteDebug::ANY;
    else if (strcasecmp(LOG_LEVEL, "VERBOSE") == 0) logLevel = RemoteDebug::VERBOSE;
    else if (strcasecmp(LOG_LEVEL, "DEBUG") == 0) logLevel = RemoteDebug::DEBUG;
    else if (strcasecmp(LOG_LEVEL, "INFO") == 0) logLevel = RemoteDebug::INFO;
    else if (strcasecmp(LOG_LEVEL, "WARNING") == 0) logLevel = RemoteDebug::WARNING;
    else if (strcasecmp(LOG_LEVEL, "ERROR") == 0) logLevel = RemoteDebug::ERROR;
    else if (strcasecmp(LOG_LEVEL, "PROFILER") == 0) logLevel = RemoteDebug::PROFILER;
    #endif // ifdef LOG_LEVEL

    if (serialEnabled) {
      #ifndef SERIAL_BAND
      # define SERIAL_BAND 115200
      #endif // ifndef SERIAL_BAND
      Serial.begin(SERIAL_BAND);
      Serial.setTimeout(2000);
    }

    s_instance = new RemotePrint(hostName,
                                 telnetPassword,
                                 logLevel,
                                 serialEnabled,
                                 telnetEnabled);
  }
  return s_instance;
}

void RemotePrint::handle() {
  if (RemotePrint::telnetEnabled) {
    Debug.handle();
  }
}

void RemotePrint::print(String args, uint8_t _logLevel) {
  if (!isActive(_logLevel)) return;

  if (telnetEnabled) {
    Debug.print(args);
  } else if (serialEnabled) {
    Serial.print(args);
  }
}

void RemotePrint::println(String args, uint8_t _logLevel) {
  if (!isActive(_logLevel)) return;

  if (telnetEnabled) {
    Debug.println(args);
  } else if (serialEnabled) {
    Serial.println(args);
  }
}

bool RemotePrint::isActive(uint8_t _logLevel) {
  if (telnetEnabled) {
    return Debug.isActive(_logLevel);
  } else if (serialEnabled) {
    return _logLevel >= logLevel;
  }
  return false;
}
