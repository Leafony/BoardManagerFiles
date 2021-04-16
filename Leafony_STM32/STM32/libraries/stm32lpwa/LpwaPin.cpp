#include "LpwaPin.h"

int LpwaPin::isPin() {
  char rcvbuff[100];
  char *find_p;
  theLpwaModemCore.send("AT+CPIN?");
  if (theLpwaModemCore.waitForResponse3("OK\r",rcvbuff,100) < 0) {
    return -3;
  }

  // +CPIN: <code>
  find_p = strstr(rcvbuff, "READY\r");
  if (find_p != NULL)
    return 0;

  find_p = strstr(rcvbuff, " SIM PIN\r");
  if (find_p != NULL)
    return 1;  

  return -1;
}
