#include <VarSpeedServo.h>
#include <EEPROM.h>

#define espacoMemoria 199
#define tempoPausaEntreMovimentos 50  //configura o tempo de pausa entre cada movimento

#define potBase         A0  // Porta Analógica do Potenciometro para Controle da Base
#define potAlturaBraco  A1  // Porta Analógica do Potenciometro para Controle da Altura do Braço
#define potAnguloBraco  A2  // Porta Analógica do Potenciometro para Controle do Ângulo do Braço
#define potGarra        A3  // Porta Analógica do Potenciometro para Controle da Garra.

#define servoBase        3  // Porta Digital do Servo da Base
#define servoAlturaBraco 4  // Porta Digital do Servo da Altura do Braço
#define servoAnguloBraco 7  // Porta Digital do Servo do Ângulo do Braço
#define servoGarra       8  // Porta Digital do Servo da Garra

#define btnGravar       11  // Porta Digital do Botão Gravar
#define btnPlayStop         12  // Porta Digital do botão Play Stop

VarSpeedServo base; 
VarSpeedServo altura;
VarSpeedServo angulo;
VarSpeedServo garra;

//Variaveis para cálculo dos angulos
int valorPotBase;
int valorAngBase;
int valorPotAltura;
int valorAngAltura;
int valorPotAngulo;
int valorAngAngulo;
int valorPotGarra;
int valorAngGarra;

int ultMemoria;

bool isPlay = false;
byte modoAnt;

bool leBotao(int porta);
void setMemoria(byte posicao, byte servo, byte valor);
byte readMemoria(byte posicao, byte servo);

void setMemoria(byte posicao, byte servo, byte valor) {
  int posMem;

  posMem = ((posicao * 5) + servo) + 1;
  EEPROM.write(posMem, valor);
}

byte readMemoria(byte posicao, byte servo) {
  int posMem;

  posMem = ((posicao * 5) + servo) + 1;
  return EEPROM.read(posMem);
}

void setup() {
  Serial.begin(9600);

  pinMode(btnGravar, INPUT_PULLUP);
  pinMode(btnPlayStop, INPUT_PULLUP);

  //Configuração das Portas dos Servos
  base.attach(servoBase);
  altura.attach(servoAlturaBraco);
  angulo.attach(servoAnguloBraco);
  garra.attach(servoGarra); 
}

void loop() {
  static byte movimento = 0;
  static byte posMemoria = 0;
  static unsigned long delayPausa;

  /*
  if (leBotao(btnPlayStop) == 0) {
    modoAnt = 0;
    ultMemoria = -1;
    EEPROM.write(0, ultMemoria);
  }
  */

  if (leBotao(btnPlayStop)) {
    isPlay = !isPlay;
  }
  
  if (isPlay) {
    //executa um movimento  
    if (movimento == 0) {
      byte velocidade = 80;

      angulo.slowmove(readMemoria(posMemoria,0), velocidade);
      base.slowmove(readMemoria(posMemoria,1), velocidade);
      altura.slowmove(readMemoria(posMemoria,2), velocidade);
      garra.slowmove(readMemoria(posMemoria,3), velocidade);

      movimento = 1;
    }

    //aguarda término de um movimento para selecionar o próximo movimento         
    if (movimento == 1) {
      if ( (angulo.read() == readMemoria(posMemoria,0)) &&
           (base.read() == readMemoria(posMemoria,1)) &&
           (altura.read() == readMemoria(posMemoria,2)) &&
           (garra.read() == readMemoria(posMemoria,3)) ) {
  
         posMemoria++;
         if (posMemoria > ultMemoria) { posMemoria = 0; }
         
         delayPausa = millis();
         movimento = 2;
      }
    }

    //pausa entre movimentos
    if (movimento == 2) {
       if ((millis() - delayPausa) > tempoPausaEntreMovimentos) {
          movimento = 0;
       }
    }
  } else {
    valorPotBase = analogRead(potBase);
    valorAngBase = map(valorPotBase, 0, 1023, 0, 180);
    
    valorPotAltura = analogRead(potAlturaBraco);
    valorAngAltura = map(valorPotAltura, 0, 1023, 180, 0);
  
    valorPotAngulo = analogRead(potAnguloBraco);
    valorAngAngulo = map(valorPotAngulo, 0, 1023, 180, 0);
  
    valorPotGarra = analogRead(potGarra);
    valorAngGarra = map(valorPotGarra, 0, 1023, 0, 90);
  
    if (leBotao(btnGravar)) {
      if (modoAnt == 0) {
        ultMemoria = -1;         
        EEPROM.write(0, ultMemoria); 
      }
  
      Serial.println(ultMemoria);
  
      ultMemoria++;
      EEPROM.write(0, ultMemoria);
  
      //Serial.println(valorAngAngulo);
      //Serial.println(valorAngBase);
      //Serial.println(valorAngAltura);
      //Serial.println(valorAngGarra);
  
      setMemoria(ultMemoria, 0, valorAngAngulo);
      setMemoria(ultMemoria, 1, valorAngBase);
      setMemoria(ultMemoria, 2, valorAngAltura);
      setMemoria(ultMemoria, 3, valorAngGarra);
  
      Serial.println(ultMemoria);
  
      if ( ultMemoria == (espacoMemoria - 1)) {
          // modo = 0; 
      }
  
      modoAnt = 1;
    }
  
    base.write(valorAngBase);
    altura.write(valorAngAltura);
    angulo.write(valorAngAngulo);
    garra.write(valorAngGarra);
  }
}

bool leBotao(int porta) {
  static bool estadoAnterior[14] = {true,true,true,true,true,true,true,true,true,true,true,true,true,true};

  bool estadoBotao = digitalRead(porta);
  bool ligado = false;

  if (!estadoBotao && estadoAnterior[porta]) {
    ligado = true;
  }

  estadoAnterior[porta] = estadoBotao;

  return ligado;
}
