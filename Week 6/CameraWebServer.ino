#include "esp_camera.h"
#include <WiFi.h>
#include <ESP32Servo.h>
#include "camera_pins.h"

// ===========================
// WiFi credentials
// ===========================
const char* ssid = "Samsung's A36";
const char* password = "aswas1410";

// ===========================
// Servo configuration
// ===========================
const int servoPin = 12;
Servo myServo;
int servoPos = 90;  // Start at center

// ===========================
// Camera config
// ===========================
#define CAMERA_MODEL_AI_THINKER

#include "esp_camera.h"
#include "fd_forward.h"  // face detection library

// Function prototypes
void startCameraServer();
void setupLedFlash(int pin);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

  // ----------------------
  // Initialize camera
  // ----------------------
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_GRAYSCALE; // required for face detection
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.grab_mode = CAMERA_GRAB_LATEST;
  config.jpeg_quality = 12;
  config.fb_count = 2;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  myServo.attach(servoPin);
  myServo.write(servoPos);  // Start centered

  // ----------------------
  // Connect WiFi
  // ----------------------
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
}

void loop() {
  // ----------------------
  // Face detection
  // ----------------------
  camera_fb_t * fb = esp_camera_fb_get();
  if(!fb){
    Serial.println("Camera capture failed");
    return;
  }

  // Perform face detection
  dl_matrix3du_t *image_matrix = dl_matrix3du_alloc(1, fb->width, fb->height, 1);
  if(image_matrix){
    fmt2rgb565(fb->buf, fb->len, fb->format, image_matrix->item);
    box_array_t *boxes = face_detect(image_matrix, 0.7); // threshold 0.7
    if(boxes && boxes->len > 0){
      Serial.println("Face detected!");
      servoPos = 0;        // Move servo to 0° when face detected
      myServo.write(servoPos);
      delay(500);          // Keep it there for half a second
      servoPos = 90;       // Return to center
      myServo.write(servoPos);
    }
    if(boxes) free(boxes->box);
    free(boxes);
    dl_matrix3du_free(image_matrix);
  }

  esp_camera_fb_return(fb);
  delay(200);  // Small delay to avoid overload
}