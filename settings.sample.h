// Generic settings
#define CHECK_INTERVAL 5000
#define DHT_PIN D5
#define DHT_VERSION DHT22
#define MH_Z19_RX D7
#define MH_Z19_TX D6
#define MAX_DATA_ERRORS 15       //max of errors, reset after them

// Data server settings
#define DATA_SERVER "co2.jehy.ru"
#define DATA_SENSOR_ID 1
#define DATA_URL "/send.php"
#define SERVER_ENABLED true


// MQTT settings
#define MQTT_ENABLED true
#define MQTT_SERVER "192.168.0.5"
#define MQTT_PORT 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""

#define HUMIDITY_TOPIC "sensor/humidity"
#define TEMPERATURE_TOPIC "sensor/temperature"
#define CO2_TOPIC "sensor/co2"
#define RAM_TOPIC "sensor/ram"

// WIFI settings
#define WIFI_SSID  "Me"             //  your network SSID (name)
#define WIFI_PASS "secure_password" // your network password
#define WIFI_MAX_ATTEMPTS_INIT 3 //set to 0 for unlimited, do not use more then 65535
#define WIFI_MAX_ATTEMPTS_SEND 1 //set to 0 for unlimited, do not use more then 65535
#define USE_GOOGLE_DNS false
#define WIFI_SEND_INTERVAL 20000
