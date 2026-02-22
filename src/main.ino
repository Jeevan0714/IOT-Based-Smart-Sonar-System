#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

// ==========================================
// 1. CONFIGURE YOUR SETTINGS
// ==========================================
const char* ssid = "Pavank";
const char* password = "Pavu8095";

// GPIO pins for the sensor
const int trigPin = 5;
const int echoPin = 18;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Timer variables
unsigned long lastMeasureTime = 0;
const long measureInterval = 1000; // Send data every 1 second

// ==========================================
// 2. EMBEDDED WEBSITE (HTML, CSS, JAVASCRIPT)
// ==========================================
const char htmlContent[] PROGMEM = R"rawstring(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ESP32 Sensor Hub</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    :root {
      --bg-color: #f0f2f5;
      --card-bg: #ffffff;
      --text-color: #1a1a1a;
      --sub-text-color: #555;
      --accent-color: #007aff;
      --chart-line: rgba(0, 122, 255, 1);
      --chart-fill: rgba(0, 122, 255, 0.2);
      --shadow: 0 4px 12px rgba(0,0,0,0.05);
    }
    /* Auto Dark Mode */
    @media (prefers-color-scheme: dark) {
      :root {
        --bg-color: #1c1c1e;
        --card-bg: #ffffff; /* Force chart card to white */
        --chart-line: rgba(0, 122, 255, 1); /* Force line to blue */
        --chart-fill: rgba(0, 122, 255, 0.2); /* Force fill to blue */
        --text-color: #f2f2f7;
        --sub-text-color: #555;
        --accent-color: #00aaff;
        --shadow: 0 4px 12px rgba(0,0,0,0.2);
      }
    }
    body {
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
      background-color: var(--bg-color);
      color: var(--text-color);
      margin: 0;
      padding: 20px;
      display: flex;
      justify-content: center;
      align-items: center;
      min-height: 100vh;
      box-sizing: border-box;
    }
    main { width: 100%; max-width: 800px; text-align: center; }
    h1 { color: var(--text-color); font-weight: 600; margin-bottom: 30px; }
    .card {
      background-color: var(--card-bg);
      border-radius: 12px;
      box-shadow: var(--shadow);
      padding: 24px;
      margin-bottom: 20px;
      transition: all 0.3s ease;
    }
    h2 { color: var(--sub-text-color); font-weight: 500; margin-bottom: 10px; margin-top: 0; }
    #distance { font-size: 4.5rem; font-weight: 300; color: var(--accent-color); line-height: 1.1; }
    #distance-unit { font-size: 1.5rem; color: var(--sub-text-color); margin-left: 5px; }
    .chart-container { position: relative; height: 40vh; min-height: 300px; padding: 15px; }
  </style>
</head>
<body>
  <main>
    <h1>IoT Based Smart Automated Sonar Sensor</h1>
    <div class="card">
      <h2>Current Distance</h2>
      <p>
        <span id="distance">...</span>
        <span id="distance-unit">cm</span>
      </p>
    </div>
    <div class="card chart-container">
      <canvas id="distanceChart"></canvas>
    </div>
  </main>
  
  <script>
    // --- WebSocket Connection
    const gateway = `ws://${window.location.hostname}/ws`;
    let websocket;
    
    window.addEventListener('load', () => { initWebSocket(); });
    
    function initWebSocket() {
      console.log('Trying to open a WebSocket connection...');
      websocket = new WebSocket(gateway);
      websocket.onopen = onOpen;
      websocket.onclose = onClose;
      websocket.onmessage = onMessage;
    }
    
    function onOpen(event) { console.log('Connection opened'); }
    function onClose(event) { 
      console.log('Connection closed'); 
      setTimeout(initWebSocket, 2000); // Try to reconnect every 2 seconds
    }

    // --- Chart.js Initialization
    const ctx = document.getElementById('distanceChart').getContext('2d');
    const chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: 'Distance (cm)',
          data: [],
          borderColor: 'var(--chart-line)',
          backgroundColor: 'var(--chart-fill)',
          borderWidth: 2,
          fill: true,
          tension: 0.4
        }]
      },
      options: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
          y: { 
            beginAtZero: true, 
            ticks: { color: 'var(--sub-text-color)' },
            grid: { color: 'rgba(128, 128, 128, 0.1)' }
          },
          x: { ticks: { display: false }, grid: { display: false } }
        },
        plugins: { legend: { display: false } },
        animation: { duration: 250 }
      }
    });

    const MAX_DATA_POINTS = 30;

    // --- Handle Incoming Messages
    function onMessage(event) {
      const distanceNum = parseFloat(event.data);
      const distanceStr = distanceNum.toFixed(2);
      
      document.getElementById('distance').innerText = distanceStr;
      
      // Update Chart
      const now = new Date(); 
      const timeLabel = `${now.getHours()}:${now.getMinutes()}:${now.getSeconds()}`;
      
      chart.data.labels.push(timeLabel);
      chart.data.datasets[0].data.push(distanceNum);
      
      if (chart.data.labels.length > MAX_DATA_POINTS) {
        chart.data.labels.shift();
        chart.data.datasets[0].data.shift();
      }
      chart.update();
    }
  </script>
</body>
</html>
)rawstring";

// ==========================================
// 3. HARDWARE & WEBSOCKET LOGIC
// ==========================================

/**
 * @brief Reads the distance from the JSN-SR04T sensor IN WATER.
 * @return The distance in centimeters (cm).
 */
float getDistance() { 
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH, 35000);
  
  // Speed of sound in water is ~1484 m/s = 0.1484 cm/us
  float distance = (duration * 0.1484) / 2.0;
  
  // Handle out-of-range readings (cap at timeout distance for water: ~2600 cm)
  if (distance <= 0 || distance > 2600) {
    return 0.0; 
  }
  return distance;
}

/**
 * @brief Handles WebSocket events
 */
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_CONNECT) {
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
  } else if (type == WS_EVT_DISCONNECT) {
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
  } else if (type == WS_EVT_DATA) {
    Serial.printf("Received data from client #%u\n", client->id());
  }
}

// ==========================================
// 4. SETUP & LOOP
// ==========================================

void setup() {
  Serial.begin(115200);
  
  // Initialize sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
  // Initialize WebSocket
  ws.onEvent(onWsEvent);
  server.addHandler(&ws);
  
  // Define web server route
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", htmlContent);
  });
  
  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Check if it's time to take a new measurement 
  if (millis() - lastMeasureTime > measureInterval) {
    lastMeasureTime = millis();
    
    float distance = getDistance();
    Serial.print("Distance: ");
    Serial.println(distance);
    
    // Send the distance to all connected WebSocket clients
    ws.textAll(String(distance));
  }
  
  // Clean up disconnected WebSocket clients
  ws.cleanupClients();
}
