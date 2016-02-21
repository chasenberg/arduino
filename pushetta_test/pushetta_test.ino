#include <Pushetta.h>

#include <Pushetta.h>
#include <Bridge.h>

// create a variable of type SpacebrewYun and initialize it with the constructor
Pushetta p = Pushetta("21a6e54e956aa7526f638e6d0274c09307326434");

// Notification interval
int interval = 3000;

void setup() {
 Bridge.begin();
}

void loop() { 
  // Send a (very original) message to a channel named "Pushetta"
  p.pushMessage("Arduino_safety", "Hello World");
  
  // wait interval millisedonds before send a messege again
  delay(interval);
}
