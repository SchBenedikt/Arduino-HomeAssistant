#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Arduino_MKRIoTCarrier.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>  // Bibliothek für Zeitfunktionen
#include <Fonts/FreeMonoBold9pt7b.h>  // Font für Text

// WiFi-Credentials
const char* ssid = "FRITZ!Box 7590 AD";
const char* password = "43836126328846995068";

// MQTT-Credentials
const char* mqtt_server = "192.168.188.188";
const int mqtt_port = 1883;
const char* mqtt_user = "mqtt";
const char* mqtt_password = "mqtt";
const char* mqtt_topic = "home/arduino/touch";  // MQTT Topic für Touch-Ereignisse

WiFiClient espClient;
PubSubClient client(espClient);
MKRIoTCarrier carrier;  // MKR IoT Carrier Objekt

// Entprellungs-Timer
unsigned long lastTouchTime = 0;
const unsigned long debounceDelay = 1000;  // 1 Sekunde Entprellzeit
const unsigned long greenDuration = 3000;   // 3 Sekunden für die grüne Hintergrundfarbe
unsigned long greenStartTime = 0;
bool isGreenBackground = false;

// NTP Client zur Zeitsynchronisation
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variablen für die Zeit
String lastFormattedTime = ""; // Variable zur Speicherung der zuletzt angezeigten Uhrzeit

void setBackgroundColor(uint16_t color) {
    uint16_t width = carrier.display.width();
    uint16_t height = carrier.display.height();

    for (int y = 0; y < height; y++) {
        carrier.display.drawFastHLine(0, y, width, color);
    }
}

void displaySuccessMessage() {
    carrier.display.setTextSize(2);
    carrier.display.setTextColor(ST77XX_BLACK);
    carrier.display.setCursor(10, 10);
    carrier.display.println("Befehl ausgefuehrt!");
}

void printTime() {
    timeClient.update();
    String formattedTime = timeClient.getFormattedTime().substring(0, 5); // Nur Stunden und Minuten extrahieren

    // Wenn die Uhrzeit sich geändert hat, aktualisiere die Anzeige
    if (formattedTime != lastFormattedTime) {
        lastFormattedTime = formattedTime; // Aktualisiere die zuletzt angezeigte Zeit

        // Berechne die Position für die Anzeige
        int16_t x, y;
        uint16_t w, h;
        carrier.display.setTextColor(ST77XX_WHITE);
        carrier.display.setTextSize(4);
        carrier.display.getTextBounds(formattedTime, 0, 0, &x, &y, &w, &h); // Holen Sie sich die Grenzen des Zeit-Textes
        x = (carrier.display.width() - w) / 2; // Berechnen Sie die X-Position für die zentrierte Anzeige
        y = (carrier.display.height() - h) / 2; // Berechnen Sie die Y-Position für die zentrierte Anzeige

        // Anzeige der Zeit
        carrier.display.setCursor(x, y);
        carrier.display.println(formattedTime);
    }
}

void setup() {
    Serial.begin(9600);
    carrier.begin();
    carrier.display.setRotation(0);

    // Hintergrund auf schwarz setzen
    setBackgroundColor(carrier.display.color565(0, 0, 0));

    // NTP Client initialisieren und Zeitzone einstellen
    timeClient.begin();
    timeClient.setTimeOffset(7200);  // Zeitzone auf +2 Stunden (z.B. für MEZ)

    // WiFi verbinden
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Verbindung zum WiFi...");
    }
    Serial.println("Mit WiFi verbunden");

    // MQTT-Server einrichten
    client.setServer(mqtt_server, mqtt_port);

    Serial.println("MKR IoT Carrier bereit");
}

void reconnect() {
    while (!client.connected()) {
        Serial.println("Verbindung zum MQTT-Server...");
        if (client.connect("ArduinoClient", mqtt_user, mqtt_password)) {
            Serial.println("Mit MQTT verbunden");
        } else {
            Serial.print("Fehler, rc=");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    // Touch-Tasten Status abfragen
    carrier.Buttons.update();
    unsigned long currentMillis = millis();

    // Entprellung für Touch-Taste 0
    if (carrier.Buttons.getTouch(TOUCH0) && (currentMillis - lastTouchTime > debounceDelay)) {
        client.publish(mqtt_topic, "Touch 0 gedrückt");
        Serial.println("Touch 0 gedrückt, Nachricht gesendet");
        lastTouchTime = currentMillis;
        setBackgroundColor(carrier.display.color565(0, 255, 0)); // Hintergrund auf grün setzen
        displaySuccessMessage(); // Erfolgsmeldung anzeigen
        greenStartTime = currentMillis; // Startzeit für die grüne Hintergrundfarbe speichern
        isGreenBackground = true;
    }

    // Entprellung für Touch-Taste 1
    if (carrier.Buttons.getTouch(TOUCH1) && (currentMillis - lastTouchTime > debounceDelay)) {
        client.publish(mqtt_topic, "Touch 1 gedrückt");
        Serial.println("Touch 1 gedrückt, Nachricht gesendet");
        lastTouchTime = currentMillis;
        setBackgroundColor(carrier.display.color565(0, 255, 0)); // Hintergrund auf grün setzen
        displaySuccessMessage(); // Erfolgsmeldung anzeigen
        greenStartTime = currentMillis; // Startzeit für die grüne Hintergrundfarbe speichern
        isGreenBackground = true;
    }

    // Entprellung für Touch-Taste 2
    if (carrier.Buttons.getTouch(TOUCH2) && (currentMillis - lastTouchTime > debounceDelay)) {
        client.publish(mqtt_topic, "Touch 2 gedrückt");
        Serial.println("Touch 2 gedrückt, Nachricht gesendet");
        lastTouchTime = currentMillis;
        setBackgroundColor(carrier.display.color565(0, 255, 0)); // Hintergrund auf grün setzen
        displaySuccessMessage(); // Erfolgsmeldung anzeigen
        greenStartTime = currentMillis; // Startzeit für die grüne Hintergrundfarbe speichern
        isGreenBackground = true;
    }

    // Entprellung für Touch-Taste 3
    if (carrier.Buttons.getTouch(TOUCH3) && (currentMillis - lastTouchTime > debounceDelay)) {
        client.publish(mqtt_topic, "Touch 3 gedrückt");
        Serial.println("Touch 3 gedrückt, Nachricht gesendet");
        lastTouchTime = currentMillis;
        setBackgroundColor(carrier.display.color565(0, 255, 0)); // Hintergrund auf grün setzen
        displaySuccessMessage(); // Erfolgsmeldung anzeigen
        greenStartTime = currentMillis; // Startzeit für die grüne Hintergrundfarbe speichern
        isGreenBackground = true;
    }

    // Entprellung für Touch-Taste 4
    if (carrier.Buttons.getTouch(TOUCH4) && (currentMillis - lastTouchTime > debounceDelay)) {
        client.publish(mqtt_topic, "Touch 4 gedrückt");
        Serial.println("Touch 4 gedrückt, Nachricht gesendet");
        lastTouchTime = currentMillis;
        setBackgroundColor(carrier.display.color565(0, 255, 0)); // Hintergrund auf grün setzen
        displaySuccessMessage(); // Erfolgsmeldung anzeigen
        greenStartTime = currentMillis; // Startzeit für die grüne Hintergrundfarbe speichern
        isGreenBackground = true;
    }

    // Überprüfen, ob die grüne Hintergrundfarbe für 3 Sekunden angezeigt werden soll
    if (isGreenBackground && (currentMillis - greenStartTime >= greenDuration)) {
        setBackgroundColor(carrier.display.color565(0, 0, 0)); // Hintergrund wieder auf schwarz setzen
        isGreenBackground = false; // Reset

        // Zeit sofort aktualisieren, nachdem der grüne Hintergrund verschwunden ist
        printTime();
    }

    // Zeit aktualisieren, wenn die letzte Minute abgelaufen ist
    if (currentMillis % 60000 < 100) { // Überprüfung auf jede volle Minute
        carrier.display.fillScreen(carrier.display.color565(0, 0, 0)); // Bildschirm löschen
        printTime(); // Uhrzeit erneut anzeigen
    }
}
