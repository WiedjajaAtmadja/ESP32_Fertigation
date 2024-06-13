#include <Arduino.h>
#include "Task.h"

Task::Task() {
  m_nLastMillis = 0;
  m_nOutputCount = 0;
  m_nCurrentIndex = 0;
  m_nMotorPumpPin=-1;
}

void Task::addSolenoidOutput(uint8_t nPin, uint16_t nDuration) {
  if (m_nOutputCount < MAX_OUTPUTS) {
    m_arOutputPin[m_nOutputCount] = nPin;
    m_arOutputDuration[m_nOutputCount] = nDuration;
    m_nOutputCount++;
  }
}

void Task::start() {
  if (m_fStart)
    return;
  m_fStart = true;
  m_nLastMillis = millis();
  m_nCurrentIndex = 0;
  digitalWrite(m_arOutputPin[m_nCurrentIndex], ON);
  delay(100);
  if (m_nMotorPumpPin != -1)
  {
    m_ticker.once_ms(m_nMotorPumpOnDelay, &Task::setMotorPumpOn, this);
  }
}

void Task::update() {
    if (!m_fStart)
        return;

    if (millis() - m_nLastMillis >= m_arOutputDuration[m_nCurrentIndex]) {
        m_nLastMillis = millis();
        digitalWrite(m_arOutputPin[m_nCurrentIndex], OFF);
        m_nCurrentIndex++;
        if (m_nCurrentIndex >= m_nOutputCount) {
            m_nCurrentIndex = 0;
            m_fStart = false;
            if (m_nMotorPumpPin != -1)
            {
              m_ticker.once_ms(m_nMotorPumpOffDelay,&Task::setMotorPumpOff, this);
            }
        }
        else
          digitalWrite(m_arOutputPin[m_nCurrentIndex], ON);
    }
}

void Task::initOutputs() {
  for (uint8_t i = 0; i < m_nOutputCount; i++) {
    pinMode(m_arOutputPin[i], OUTPUT);
    digitalWrite(m_arOutputPin[i], OFF);
  }
}