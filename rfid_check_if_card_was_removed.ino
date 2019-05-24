#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9           // Configurable, see typical pin layout above
#define SS_PIN 10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//*****************************************************************************************//
void setup() {
  Serial.begin(9600);                                           // Initialize serial communications with the PC
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
}
/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
void PrintHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

//*****************************************************************************************//

void loop() {

  // Look for new cards
  if ( !mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( !mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  //mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card
  
  Serial.print("NewCard");
  PrintHex(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println("");

  // Check if Card was removed
  bool cardRemoved = false;
  int counter = 0;
  bool current, previous;
  previous = !mfrc522.PICC_IsNewCardPresent();
    
  while(!cardRemoved){
    current =!mfrc522.PICC_IsNewCardPresent();

    if (current && previous) counter++;

    previous = current;
    cardRemoved = (counter>2);      
    delay(50);
  }
  
  Serial.println("Card was removed");
  delay(500); //change value if you want to read cards faster
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
