bool sendingMessage = false;
bool receivingMessage = false;
bool receivedMessage = false;
uint8_t messageReceiving;
uint8_t messageToSend;
unsigned long startMessageSend;
unsigned long startMessageReceive = 0;

void processConnection() {
  if (sendingMessage) {
    static long lastStage = -1;
    long stage = (millis() - startMessageSend) / BITLEN;
    if (stage > lastStage) {
      Serial.println(stage);
      switch (stage) {
        case 0: // Write low then high to signal sending message
          digitalWrite(TRANSMITTER_PIN, LOW);
          break;
        case 1:
          digitalWrite(TRANSMITTER_PIN, HIGH);
          break;
        case 9: // Set sending to false on last bit
          sendingMessage = false;
          lastStage = -1;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8: // Shift over message when not on first bit
          messageToSend >>= 1;
        case 2:
          digitalWrite(TRANSMITTER_PIN, messageToSend & 1);
          break;
        default:
          Serial.println("Overshot janksend");
          sendingMessage = false;
          break;
      }
      lastStage = stage;
    }
  }

  if (receivingMessage) {
    if (millis() > startMessageReceive) {
      static long lastStage = -1;
      long stage = (millis() - startMessageReceive) / BITLEN;
      if (stage > lastStage) {
        Serial.println(stage);
        switch (stage) {
          case 7:
            receivingMessage = false;
            receivedMessage = true;
            lastStage = -1;
          case 1:
          case 2:
          case 3:
          case 4:
          case 5:
          case 6:
            messageReceiving <<= 1;
          case 0:
            messageReceiving |= digitalRead(RECEIVER_PIN);
            break;
          default:
            Serial.println("Overshot jankrecv");
            receivingMessage = false;
            break;
        }
        lastStage = stage;
      }
    }
  } else {
    static uint8_t lastReceive = HIGH;
    uint8_t receive = digitalRead(RECEIVER_PIN);
    if (lastReceive == LOW && receive == HIGH && millis() > startMessageReceive + 10 * BITLEN) {
      receivingMessage = true;
      receivedMessage = false;
      startMessageReceive = millis() + (BITLEN * 3 / 2); // Start receiving in the middle of the first bit
      messageReceiving = 0;
    }
    lastReceive = receive;
  }
}
void sendMessage(uint8_t m) {
  if (!sendingMessage) {
    messageToSend = m;
    startMessageSend = millis();
    sendingMessage = true;
  }
}
