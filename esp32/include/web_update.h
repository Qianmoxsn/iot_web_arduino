#ifndef web_update_h
#define web_update_h

#include <Arduino.h>
#include <HTTPClient.h>
#include <Update.h>

#include <display.h>
#include <basic.h>
// #include <Ethernet.h>

/*

Its very simple to use, just create a web_update object with the host, debug messages,
buffer size and timeout for the update.
Then call the update method.

When calling the update method, if any error occurs, the method will return the error codes:

1 - WiFi not connected
2 - Host not found
3 - Update Timeout (1 minute, customizable in the web_update object with seconds)

*/
class web_update
{
public:
    web_update(String host = "", String directory = "", int debugger = 0, int https = 1, int read_buffer = 64, int timeout_seconds = 60);
    int update_wifi();
    int update_ethernet();
    void host(String host);
    void directory(String Dir);
    void debugger(int debugger);
    void https(int https);
    void buffer_size(int Buffer);
    void timeout(int timeout);

private:
    // EthernetClient ethernet_client;
    HTTPClient wifi_client;
    String Host, dir;
    int buffer, Https;
};
#endif
