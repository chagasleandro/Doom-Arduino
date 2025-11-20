Doom-Arduino — Top-Down Shooter inspirado em Doom

Este projeto é um jogo estilo Doom (top-down shooter) desenvolvido para Arduino + OLED SSD1306, com player, inimigos simples, mapa em tiles e sistema de tiro.
Não é o Doom original, mas sim uma versão inspirada, otimizada para rodar em microcontroladores de baixa memória.

🎮 Funcionalidades

Movimentação do jogador via joystick

Disparo usando botão físico

Inimigos com movimento simples

Renderização em 2D no OLED 128×64

Mapa baseado em tiles

Game loop leve para rodar em Arduino UNO/Nano

🛠️ Hardware necessário

Arduino UNO / Nano / Pro Micro (ou ESP32 para melhor desempenho)

Display OLED SSD1306 (I2C)

Joystick analógico (ou 2 potenciômetros)

Botão de disparo

Jumpers + breadboard

📚 Bibliotecas utilizadas

Instale via Arduino IDE:

Adafruit GFX Library  
Adafruit SSD1306

📂 Estrutura do projeto
doom_arduino/
<br/>│── doom_arduino.ino
<br/>│── README.md
<br/>│── hardware.md
<br/>└── LICENSE (MIT)

▶️ Como rodar

Abra o arquivo doom_arduino.ino no Arduino IDE.

Instale as bibliotecas necessárias.

Ajuste os pinos no início do código conforme seu hardware.

Conecte o Arduino e faça upload.

Aproveite o jogo! 🎮

🔧 Ligações (resumo)

OLED SSD1306: SDA → A4, SCL → A5 (Arduino UNO)

Joystick: VRX → A0, VRY → A1

Botão: pino digital (definido no código)

Detalhes completos estão em hardware.md.

📜 Licença

Este projeto está sob a licença MIT — sinta-se livre para usar e modificar.

🤝 Contribuições

Pull requests são bem-vindos! Sugestões de melhorias, otimizações ou novas fases são super aceitas.
