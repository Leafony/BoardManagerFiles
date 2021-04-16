#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUG_ENABLED
#define DEBUG_INIT() Serial.begin(115200)
#define DEBUG(...) Serial.println(__VA_ARGS__)
#else
#define DEBUG_INIT()
#define DEBUG(...)
#endif

#endif // DEBUG_H_
