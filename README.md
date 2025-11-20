# Doom-Arduino (Doom-like top-down shooter)

**Descrição**
Projeto *inspirado* em Doom — uma versão simplificada top-down shooter para Arduino com display OLED 128×64 (SSD1306).  
**Importante:** isto **não** é o jogo Doom original (por razões técnicas e de direitos autorais). É um jogo inspirado: mapa em tiles, jogador, inimigos simples e tiros.

**Hardware alvo**
- Arduino UNO / Nano / Pro Micro / ESP32 (pinos podem variar)
- OLED SSD1306 128×64 (I2C)
- Joystick analógico (ou 2 potenciômetros)
- Botão de disparo
- Fios e breadboard

**Bibliotecas necessárias**
- Adafruit SSD1306
- Adafruit GFX

Instale via Library Manager do Arduino IDE:
`Adafruit SSD1306` e `Adafruit GFX Library`

**Como usar**
1. Abra `doom_arduino.ino` no Arduino IDE.
2. Ajuste os pinos do joystick e botão caso necessário na seção `PIN CONFIG`.
3. Selecione a placa correta e envie.
4. Controle:
   - Movimento: joystick analógico (X/Y) ou potenciômetros.
   - Atirar: botão (pulldown/up conforme sua montagem).

**Arquivo principal**
- `doom_arduino.ino` — sketch Arduino com implementação do jogo.

**Licença**
MIT — veja LICENSE.

**Como subir no GitHub**
1. Crie um novo repositório no GitHub (ex: `doom-arduino`).
2. No terminal:
```bash
git init
git add .
git commit -m "Initial commit - Doom-like top-down shooter for Arduino"
git branch -M main
git remote add origin https://github.com/<seu-usuario>/<doom-arduino>.git
git push -u origin main
```
(Substitua `<seu-usuario>` e `<doom-arduino>` pelo nome real.)

Divirta-se e personalize o mapa, inimigos e mecânicas!
