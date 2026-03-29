//flash read
#include <SPI.h>

// -------- CONFIG (Must match your Write Code) --------
#define CS_PIN 15
#define LOG_SIZE 256
#define MAX_LOGS 10
#define META_ADDR  0x0000   
#define DATA_START 0x1000   

// SPI Settings
SPISettings flashSPI(1000000, MSBFIRST, SPI_MODE0);

// -------- PERSISTENCE STRUCT --------
struct Meta {
  int flashIndex;
  int totalLogs;
  int totalPersons;
};

Meta meta;

// ================= READ CORE =================

void flashReadData(uint32_t addr, char *buffer, int len) {
  SPI.beginTransaction(flashSPI);
  digitalWrite(CS_PIN, LOW);
  
  SPI.transfer(0x03); // Read Command
  SPI.transfer((addr >> 16) & 0xFF);
  SPI.transfer((addr >> 8) & 0xFF);
  SPI.transfer(addr & 0xFF);
  
  for (int i = 0; i < len; i++) {
    buffer[i] = (char)SPI.transfer(0x00);
  }
  
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
}

void checkChipID() {
  SPI.beginTransaction(flashSPI);
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x9F); 
  uint8_t m = SPI.transfer(0x00);
  uint8_t t = SPI.transfer(0x00);
  uint8_t c = SPI.transfer(0x00);
  digitalWrite(CS_PIN, HIGH);
  SPI.endTransaction();
  
  Serial.printf("\n--- CHIP ID: %02X %02X %02X ---\n", m, t, c);
  if (m == 0xEF) Serial.println("Status: Winbond Flash Detected.");
  else if (m == 0x00 || m == 0xFF) Serial.println("Status: ERROR - Chip not found (Check wiring/power).");
}

// ================= SETUP =================

void setup() {
  Serial.begin(115200);
  delay(2000); 

  SPI.begin(18, 19, 23, CS_PIN);
  pinMode(CS_PIN, OUTPUT); 
  digitalWrite(CS_PIN, HIGH);

  checkChipID();

  // 1. Load the Metadata
  flashReadData(META_ADDR, (char*)&meta, sizeof(Meta));

  Serial.println("\n--- METADATA INFO ---");
  Serial.print("Total Logs in Meta: "); Serial.println(meta.totalLogs);
  Serial.print("Total Persons Counted: "); Serial.println(meta.totalPersons);
  Serial.print("Last Write Index: "); Serial.println(meta.flashIndex);
  
  // 2. Read Logs based on Meta
  Serial.println("\n--- READING LOGS FROM MEMORY ---");
  if (meta.totalLogs <= 0 || meta.totalLogs > MAX_LOGS) {
    Serial.println("No valid logs found in Meta. Starting Deep Scan...");
  } else {
    for (int i = 0; i < meta.totalLogs; i++) {
      // Calculate circular buffer index
      int idx = (meta.totalLogs == MAX_LOGS) ? (meta.flashIndex + i) % MAX_LOGS : i;
      uint32_t addr = DATA_START + (idx * LOG_SIZE);
      
      char buf[LOG_SIZE];
      flashReadData(addr, buf, LOG_SIZE);
      
      Serial.print("Slot "); Serial.print(idx); Serial.print(": ");
      if ((uint8_t)buf[0] == 0xFF) Serial.println("[Erased/Empty]");
      else if (buf[0] == 0x00) Serial.println("[Null/Empty]");
      else Serial.println(buf);
    }
  }

  // 3. Deep Scan (Force read first 10 slots regardless of meta)
  Serial.println("\n--- DEEP SCAN (RAW DATA) ---");
  for (int i = 0; i < 10; i++) {
    uint32_t addr = DATA_START + (i * LOG_SIZE);
    char buf[LOG_SIZE];
    flashReadData(addr, buf, LOG_SIZE);
    
    Serial.printf("Addr 0x%04X: ", addr);
    if ((uint8_t)buf[0] == 0xFF) Serial.println("[0xFF - Empty]");
    else if (buf[0] == 0x00) Serial.println("[0x00 - Null]");
    else Serial.println(buf);
  }

  Serial.println("\n--- READ FINISHED ---");
}

void loop() {
  // Standalone reader - do nothing
}
