# Esquema e pinos

- SSD1306 I2C
  - VCC -> 3.3V ou 5V (verifique seu módulo)
  - GND -> GND
  - SDA -> A4 (UNO) or SDA
  - SCL -> A5 (UNO) or SCL

- Joystick (ou 2 potenciômetros)
  - VRx -> A0
  - VRy -> A1
  - SW  -> D2 (botão do joystick, opcional)

- Botão (tiro)
  - Botão -> D3 (pulldown ou pullup conforme montagem)

Ajuste os pinos em `doom_arduino.ino` se necessário.
