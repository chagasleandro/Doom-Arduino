#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// 128x64 OLED SSD1306 via I2C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// PIN CONFIG - ajuste conforme seu hardware
const int PIN_JOY_X = A0;    // movimento X
const int PIN_JOY_Y = A1;    // movimento Y
const int PIN_BTN_SHOOT = 3; // botão atirar (pulldown recomendado)

// Game config
const int TILE = 8; // cada tile = 8x8 pixels
const int MAP_W = 16;
const int MAP_H = 8;

// Simple map: '#' = wall, '.' = floor
const char MAP[MAP_H][MAP_W+1] = {
  "################",
  "#..............#",
  "#..##.....##...#",
  "#..............#",
  "#...####.......#",
  "#...........#..#",
  "#......E.......#",
  "################"
};

struct Entity {
  int x, y; // pixel coordinates
  int vx, vy;
  bool active;
};

Entity player;
Entity bullets[6];
Entity enemies[6];

unsigned long lastBulletTime = 0;
const unsigned long BULLET_COOLDOWN = 200; // ms
const int BULLET_SPEED = 6;
const int PLAYER_SPEED = 2;
const unsigned long ENEMY_MOVE_T = 180;

unsigned long lastEnemyMove = 0;

void spawnEnemy(int idx, int tx, int ty){
  enemies[idx].x = tx;
  enemies[idx].y = ty;
  enemies[idx].active = true;
}

void setupEntities(){
  player.x = 16;
  player.y = 16;
  player.vx = player.vy = 0;
  player.active = true;
  for(int i=0;i<6;i++){
    bullets[i].active = false;
    enemies[i].active = false;
  }
  // spawn enemies from map 'E' positions
  int ei = 0;
  for(int r=0;r<MAP_H;r++){
    for(int c=0;c<MAP_W;c++){
      if(MAP[r][c] == 'E' && ei < 6){
        spawnEnemy(ei, c*TILE + TILE/2, r*TILE + TILE/2);
        ei++;
      }
    }
  }
}

void setup() {
  pinMode(PIN_BTN_SHOOT, INPUT_PULLUP); // assume button to ground
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  setupEntities();
}

bool isWallAtPixel(int px, int py){
  if(px < 0 || py < 0 || px >= SCREEN_WIDTH || py >= SCREEN_HEIGHT) return true;
  int cx = px / TILE;
  int cy = py / TILE;
  if(cx < 0 || cy < 0 || cx >= MAP_W || cy >= MAP_H) return true;
  return MAP[cy][cx] == '#';
}

void movePlayer(int dx, int dy){
  int nx = player.x + dx;
  int ny = player.y + dy;
  // collision simple: check center point and small offsets
  if(!isWallAtPixel(nx, ny) && !isWallAtPixel(nx+3, ny) && !isWallAtPixel(nx-3, ny) && !isWallAtPixel(nx, ny+3) && !isWallAtPixel(nx, ny-3)){
    player.x = nx;
    player.y = ny;
  }
}

void fireBullet(int dirx, int diry){
  unsigned long now = millis();
  if(now - lastBulletTime < BULLET_COOLDOWN) return;
  lastBulletTime = now;
  for(int i=0;i<6;i++){
    if(!bullets[i].active){
      bullets[i].active = true;
      bullets[i].x = player.x;
      bullets[i].y = player.y;
      bullets[i].vx = dirx * BULLET_SPEED;
      bullets[i].vy = diry * BULLET_SPEED;
      break;
    }
  }
}

void updateBullets(){
  for(int i=0;i<6;i++){
    if(!bullets[i].active) continue;
    bullets[i].x += bullets[i].vx;
    bullets[i].y += bullets[i].vy;
    // deactivate on wall or out of bounds
    if(isWallAtPixel(bullets[i].x, bullets[i].y) || bullets[i].x < 0 || bullets[i].y < 0 || bullets[i].x >= SCREEN_WIDTH || bullets[i].y >= SCREEN_HEIGHT){
      bullets[i].active = false;
      continue;
    }
    // check hit enemies
    for(int e=0;e<6;e++){
      if(!enemies[e].active) continue;
      int dx = bullets[i].x - enemies[e].x;
      int dy = bullets[i].y - enemies[e].y;
      if(abs(dx) < 6 && abs(dy) < 6){
        // hit
        enemies[e].active = false;
        bullets[i].active = false;
        break;
      }
    }
  }
}

void updateEnemies(){
  unsigned long now = millis();
  if(now - lastEnemyMove < ENEMY_MOVE_T) return;
  lastEnemyMove = now;
  for(int i=0;i<6;i++){
    if(!enemies[i].active) continue;
    int dx = player.x - enemies[i].x;
    int dy = player.y - enemies[i].y;
    int stepX = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
    int stepY = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
    // try move X then Y with collision check
    int nx = enemies[i].x + stepX;
    int ny = enemies[i].y + stepY;
    if(!isWallAtPixel(nx, enemies[i].y)) enemies[i].x = nx;
    if(!isWallAtPixel(enemies[i].x, ny)) enemies[i].y = ny;
    // check collision with player
    if(abs(enemies[i].x - player.x) < 6 && abs(enemies[i].y - player.y) < 6){
      // reset player to start
      player.x = 16;
      player.y = 16;
    }
  }
}

void drawMap(){
  for(int r=0;r<MAP_H;r++){
    for(int c=0;c<MAP_W;c++){
      int px = c*TILE;
      int py = r*TILE;
      if(MAP[r][c] == '#'){
        display.fillRect(px, py, TILE, TILE, SSD1306_WHITE);
      } else {
        // small floor pattern (optional)
        // display.drawRect(px, py, TILE, TILE, SSD1306_BLACK);
      }
    }
  }
}

void drawEntities(){
  // player
  display.fillRect(player.x-3, player.y-3, 6, 6, SSD1306_WHITE);
  // bullets
  for(int i=0;i<6;i++){
    if(bullets[i].active){
      display.drawPixel(bullets[i].x, bullets[i].y, SSD1306_WHITE);
    }
  }
  // enemies
  for(int i=0;i<6;i++){
    if(enemies[i].active){
      display.drawRect(enemies[i].x-3, enemies[i].y-3, 6, 6, SSD1306_WHITE);
    }
  }
}

int readJoyAxis(int pin){
  int v = analogRead(pin); // 0..1023
  return v;
}

void loop() {
  // read joystick
  int vx = readJoyAxis(PIN_JOY_X);
  int vy = readJoyAxis(PIN_JOY_Y);
  int dx = 0, dy = 0;
  // deadzone
  const int DEAD = 100;
  if(vx < 512 - DEAD) dx = -PLAYER_SPEED;
  else if(vx > 512 + DEAD) dx = PLAYER_SPEED;
  if(vy < 512 - DEAD) dy = -PLAYER_SPEED;
  else if(vy > 512 + DEAD) dy = PLAYER_SPEED;

  movePlayer(dx, dy);

  // shoot direction: based on last movement or joystick tilt
  bool shoot = digitalRead(PIN_BTN_SHOOT) == LOW; // pressed to ground (with pullup)
  if(shoot){
    int sdx = 0, sdy = 0;
    // determine shoot direction from joystick tilt
    if(abs(vx - 512) > abs(vy - 512)){
      sdx = (vx > 512) ? 1 : -1;
    } else {
      sdy = (vy > 512) ? 1 : -1;
    }
    // fallback to right
    if(sdx == 0 && sdy == 0) sdx = 1;
    fireBullet(sdx, sdy);
  }

  updateBullets();
  updateEnemies();

  // draw
  display.clearDisplay();
  drawMap();
  drawEntities();
  display.display();

  delay(20);
}
