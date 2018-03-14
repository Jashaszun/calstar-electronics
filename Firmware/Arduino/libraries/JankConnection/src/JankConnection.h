
/* CONNECTION SPECS

 - Would like to have every code at max hamming distance with every other code
 - Also would like codes to not be shifted versions of other codes (to prevent action on late detection)
 - All 0s or all 1s should not be allowed (to prevent action on a mis-fire)
 - Would like to have baud rate at max that doesn't cause errors, minus a margin of error for safety


 Ejection -> Deployment
  0f: Activate black powder
  17: Activate black powder for 1s
  2B: Deactivate black powder
  33: Query continuity
  4d: Query state
  55: Beep
  69, 71, 8E, 96, AA, B2, CC: Set state to [INIT, LAUNCH:PAD, LAUNCH:FLIGHT, LAUNCH:LANDED, LAUNCH:SIGNAL_RECEIVED, LAUNCH:TRIGGER, LAUNCH:DEPLOYED]
  D4: Turn off altitude checks
  E8: Turn on altitude checks
  F0: Trigger deployment

 Deployment -> Ejection
  0f: Ack BP activate
  17: Ack BP activate 1s
  2B: Ack BP deactivate
  33: Send discontinuous
  4d: Send continuous
  55: Ack beep
  69, 71, 8E, 96, AA, B2, CC: Send state x7 (Same as ack stage change)
  D4 Ack altitude checks off
  E8 Ack altitude checks on
  F0: Ack trigger deployment
*/






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
      lastStage = stage;
      Serial.println(stage);
      switch (stage) {
        case 0: // Write low then high to signal sending message
          digitalWrite(TRANSMITTER_PIN, LOW);
          break;
        case 1:
          digitalWrite(TRANSMITTER_PIN, HIGH);
          break;
        case 9: // Set sending to false on last bit
          lastStage = -1;
          sendingMessage = false;
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
          lastStage = -1;
          sendingMessage = false;
          break;
      }
    }
  }

  if (receivingMessage) {
    if (millis() > startMessageReceive) {
      static long lastStage = -1;
      long stage = (millis() - startMessageReceive) / BITLEN;
      if (stage > lastStage) {
        lastStage = stage;
        Serial.println(stage);
        if (stage < 8) {
          if (stage == 7) {
            receivingMessage = false;
            receivedMessage = true;
            lastStage = -1;
          }
          messageReceiving |= digitalRead(RECEIVER_PIN) << stage;
        } else {
          Serial.println("Overshot jankrecv");
          receivingMessage = false;
          lastStage = -1;
        }
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
