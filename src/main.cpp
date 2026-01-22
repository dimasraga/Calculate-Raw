#include <Arduino.h>

// ==========================================
// GLOBAL VARIABLES (The State)
// ==========================================
// Point 1 (Now)
float x1_raw  = 43718.0; 
float y1_temp = 26.8;    

// Point 2 (Max)
float x2_raw  = 50265.0; 
float y2_temp = 60.0;    

// Input to Test
float input_x = 50265.0; 

// ==========================================
// FUNCTION PROTOTYPES
// ==========================================
void calculateAndReport();
void parseCommand(String command);

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; } 
  delay(1000);

  Serial.println("=== SYSTEM READY ===");
  Serial.println("Commands:");
  Serial.println("  x1=VALUE  (Update Raw Now)");
  Serial.println("  y1=VALUE  (Update Temp Now)");
  Serial.println("  x2=VALUE  (Update Raw Max)");
  Serial.println("  y2=VALUE  (Update Temp Max)");
  Serial.println("  in=VALUE  (Update Input X)");
  Serial.println("Example: x1=44000");
  Serial.println("====================");

  // Perform initial calculation once
  calculateAndReport();
}

void loop() {
  // LISTENER: Check if user typed something
  if (Serial.available() > 0) {
    String receivedString = Serial.readStringUntil('\n'); // Read until Enter
    receivedString.trim(); // Remove whitespace/newlines
    
    if (receivedString.length() > 0) {
      Serial.print(">> Command Received: ");
      Serial.println(receivedString);
      
      parseCommand(receivedString); // Process the command
    }
  }
}

// ==========================================
// CORE LOGIC
// ==========================================

void parseCommand(String cmd) {
  // Check which variable the user wants to update
  // We use .substring(3) to grab the number after "xx="
  
  if (cmd.startsWith("x1=") || cmd.startsWith("X1=")) {
    x1_raw = cmd.substring(3).toFloat();
  }
  else if (cmd.startsWith("y1=") || cmd.startsWith("Y1=")) {
    y1_temp = cmd.substring(3).toFloat();
  }
  else if (cmd.startsWith("x2=") || cmd.startsWith("X2=")) {
    x2_raw = cmd.substring(3).toFloat();
  }
  else if (cmd.startsWith("y2=") || cmd.startsWith("Y2=")) {
    y2_temp = cmd.substring(3).toFloat();
  }
  else if (cmd.startsWith("in=") || cmd.startsWith("IN=")) {
    input_x = cmd.substring(3).toFloat();
  }
  else {
    Serial.println(">> ERROR: Unknown Command. Use format 'x1=1234'");
    return; // Stop here, don't calculate
  }

  // If we successfully updated a variable, re-run calculation
  calculateAndReport();
}

void calculateAndReport() {
  // 1. Calculate Gradient (m)
  // m = (y2 - y1) / (x2 - x1)
  float m = (y2_temp - y1_temp) / (x2_raw - x1_raw);

  // 2. Calculate Intercept (c)
  // c = y1 - (m * x1)
  float c = y1_temp - (m * x1_raw);

  // 3. Calculate Result (y)
  // y = mx + c
  float output_y = (m * input_x) + c;

  // 4. Print Report (Academic Style)
  Serial.println("\n--- CALIBRATION REPORT ---");
  Serial.printf("X1 RAW NOW: %.0f -> %.1f C\n", x1_raw, y1_temp);
  Serial.printf("X2 RAW MAX: %.0f -> %.1f C\n", x2_raw, y2_temp);
  Serial.println("--------------------------");
  Serial.printf("Gradient (m) : %.6f\n", m);
  Serial.printf("Intercept (c): %.6f\n", c);
  Serial.println("--------------------------");
  Serial.printf("TEST INPUT x : %.0f\n", input_x);
  Serial.printf("RESULT y     : %.6f\n", output_y);
  Serial.println("==========================\n");
}