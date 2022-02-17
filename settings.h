// Generic settings
#define CHECK_INTERVAL 5000
#define DHT_PIN D5
#define DHT_VERSION DHT22
#define MH_Z19_RX D7
#define MH_Z19_TX D6
#define MAX_DATA_ERRORS 15       //max of errors, reset after them

// MQTT settings
#define MQTT_ENABLED true
#define MQTT_SERVER "smarthome.plugin.lu"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""

#define CO2_TOPIC "sensor/living/co2"
#define RAM_TOPIC "sensor/living/ram"

// WIFI settings
#define WIFI_SSID  "BANANA2"             //  your network SSID (name)
#define WIFI_PASS "001839C5E205-CL7B1F768081" // your network password
#define WIFI_MAX_ATTEMPTS_INIT 3 //set to 0 for unlimited, do not use more then 65535
#define WIFI_MAX_ATTEMPTS_SEND 1 //set to 0 for unlimited, do not use more then 65535
#define WIFI_SEND_INTERVAL 20000
