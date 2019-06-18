#include <VarSpeedServo.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#define espacoMemoria 199
#define tempoPausaEntreMovimentos 50  //configura o tempo de pausa entre cada movimento
  
#define servoBase        3  // Porta Digital do Servo da Base
#define servoAlturaBraco 4  // Porta Digital do Servo da Altura do Braço
#define servoAnguloBraco 7  // Porta Digital do Servo do Ângulo do Braço
#define servoGarra       8  // Porta Digital do Servo da Garra

#define potAlturaBraco  A2  // Porta Analógica do Potenciometro para Controle da Altura do Braço
#define potBase         A1  // Porta Analógica do Potenciometro para Controle da Base
#define potAnguloBraco  A0  // Porta Analógica do Potenciometro para Controle do Ângulo do Braço
#define potGarra        A3  // Porta Analógica do Potenciometro para Controle da Garra.

#define botaoCongela     12  // Porta Digital do Botao Congela
#define botaoDescongela  11  // Porta Digital do Botao Descongela

//Instanciação dos Objetos de Controle dos Servos
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

int valorBtnBase = 80;
int valorBtnAltura = 90;
int valorBtnAngulo = 40;
int valorBtnGarra = 45;

//Variaveis para controle dos botões
int estadoBotaoC, estadoBotaoD;
int estadoAntBotaoC = HIGH;
int estadoAntBotaoD = HIGH;
boolean congelado = false;
boolean controleWifi = false;

bool isPlay = false;

int ultMemoria;

void setMemoria(byte posicao, byte servo, byte valor);
byte readMemoria(byte posicao, byte servo);

byte modoAnt;

void setup(){
  Serial.begin(9600);

  Wire.begin(8);                /* aderir ao bus i2c com o endereço 8 */
  Wire.onReceive(receiveEvent); /* iniciar serial para depuração */
  
  pinMode(botaoCongela, INPUT_PULLUP);
  pinMode(botaoDescongela, INPUT_PULLUP);
    
  //Configuração das Portas dos Servos
  base.attach(servoBase);
  altura.attach(servoAlturaBraco);
  angulo.attach(servoAnguloBraco);
  garra.attach(servoGarra); 

  base.write(valorAngBase);

  ultMemoria = EEPROM.read(0);
}

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

void loop() {  
  static byte movimento = 0;
  static byte posMemoria = 0;
  static unsigned long delayPausa;

  if (controleWifi) {
    if (isPlay) {
      //executa um movimento  
      if (movimento == 0) {
        // angulo.write(readMemoria(posMemoria,0));
        // base.write(readMemoria(posMemoria,1));
        // altura.write(readMemoria(posMemoria,2));
        // garra.write(readMemoria(posMemoria,3));

        byte velocidade = 80;
          
        angulo.slowmove(readMemoria(posMemoria,0), velocidade);
        base.slowmove(readMemoria(posMemoria,1), velocidade);
        altura.slowmove(readMemoria(posMemoria,2), velocidade);
        garra.slowmove(readMemoria(posMemoria,3), velocidade);
  
        Serial.print("Angulo: ");
        Serial.print(angulo.read());
        
        movimento = 1;
      }
  
      //aguarda término de um movimento para selecionar o próximo movimento         
      if (movimento == 1) {
        Serial.print("Angulo: ");
        Serial.println(angulo.read());
        Serial.println(readMemoria(posMemoria,0));
  
        
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
    }  
  }
  
  if (!controleWifi) {
    valorPotBase = analogRead(potBase);
    valorAngBase = map(valorPotBase, 0, 1023, 0, 180);
    
    if (!congelado) {  
      valorPotAltura = analogRead(potAlturaBraco);
      valorAngAltura = map(valorPotAltura, 0, 1023, 180, 0);
    
      valorPotAngulo = analogRead(potAnguloBraco);
      valorAngAngulo = map(valorPotAngulo, 0, 1023, 180, 0);
    
      valorPotGarra = analogRead(potGarra);
      valorAngGarra = map(valorPotGarra, 0, 1023, 0, 90);
    
      altura.write(valorAngAltura);
      angulo.write(valorAngAngulo);
      garra.write(valorAngGarra);
    }
    
    base.write(valorAngBase);
    
    //Se o botao de congelar foi apertado
    estadoBotaoC = digitalRead(botaoCongela); 
    if (estadoBotaoC != estadoAntBotaoC) {
       congelado = true;
    }
    estadoAntBotaoC = estadoBotaoC;
    
    //Se o botao de descongelar foi apertado
    estadoBotaoD = digitalRead(botaoDescongela); 
    if (estadoBotaoD != estadoAntBotaoD) {
       congelado = false;
    }
    estadoAntBotaoD = estadoBotaoD;
  }

  delay(100);

}

// função que executa quando os dados são recebidos do mestre
void receiveEvent(int howMany) {
  String data = "";

  while (0 <Wire.available()) {
    char c = Wire.read();  // recebe byte como personagem
    data += c;
  }

  Serial.println(data);  // imprimir os dados da solicitação

  processCall(data);  // para nova linha
}

void processCall(String command){  
  // RESET
  if (command == "RESET") {
    modoAnt = 0;
    ultMemoria = -1;         
    EEPROM.write(0, ultMemoria); 
  }
  
  // GRAVAR
  if (command == "GRAVAR") {
    Serial.println("Gravar");

    if (modoAnt == 0) {
      ultMemoria = -1;         
      EEPROM.write(0, ultMemoria); 
    }

    Serial.println(ultMemoria);

    ultMemoria++;
    EEPROM.write(0, ultMemoria);

    Serial.println(valorBtnAngulo);
    Serial.println(valorBtnBase);
    Serial.println(valorBtnAltura);
    Serial.println(valorBtnGarra);

    setMemoria(ultMemoria, 0, valorBtnAngulo);
    setMemoria(ultMemoria, 1, valorBtnBase);
    setMemoria(ultMemoria, 2, valorBtnAltura);
    setMemoria(ultMemoria, 3, valorBtnGarra);

    Serial.println(ultMemoria);

    if ( ultMemoria == (espacoMemoria - 1)) {
        // modo = 0; 
    }

    modoAnt = 1;
  }

  // PLAY
  if (command == "PLAY") {
    Serial.println("Play");

    isPlay = !isPlay;
  }
  
  // DESLIGAR
  if (command == "DESLIGAR") {
    controleWifi = false;

    angulo.write(valorBtnAngulo);
    base.write(valorBtnBase);
    altura.write(valorBtnAltura);
    garra.write(valorBtnGarra);
  }
  
  // LIGAR
  if (command == "LIGAR") {
    controleWifi = true;

    Serial.println(controleWifi);
    Serial.println("Ligar");

    angulo.write(valorBtnAngulo);
    base.write(valorBtnBase);
    altura.write(valorBtnAltura);
    garra.write(valorBtnGarra);
  }

  if (controleWifi) {
    // ANGULO_UP
    if (command == "{\"angulo\":\"up\"}") {
      if (valorBtnAngulo < 180) {
        valorBtnAngulo = valorBtnAngulo + 10; 

        Serial.println(valorBtnAngulo);
      }
    }
  
    // ANGULO_DOWN
    if (command == "{\"angulo\":\"down\"}") {
      if (valorBtnAngulo != 0) {
        valorBtnAngulo = valorBtnAngulo - 10; 

        Serial.println(valorBtnAngulo);
      }
    }

    // Serial.print("Angulo: ");
    // Serial.println(valorBtnAngulo);
  
    // BASE_LEFT
    if (command == "{\"base\":\"left\"}") {
      if (valorBtnBase < 180) {
        valorBtnBase = valorBtnBase + 10; 

        Serial.println(valorBtnBase);
      }
    }
  
    // BASE_RIGHT
    if (command == "{\"base\":\"right\"}") {
      if (valorBtnBase != 0) {
        valorBtnBase = valorBtnBase - 10; 

        Serial.println(valorBtnBase);
      }
    }

    // Serial.print("Base: ");
    // Serial.println(valorBtnBase);
  
    // ALTURA_UP
    if (command == "{\"altura\":\"up\"}") {
      if (valorBtnAltura < 180) {
        valorBtnAltura = valorBtnAltura + 10; 

        Serial.println(valorBtnAltura);
      }
    }
  
    // ALTURA_DOWN
    if (command == "{\"altura\":\"down\"}") {
      if (valorBtnAltura != 0) {
        valorBtnAltura = valorBtnAltura - 10; 

        Serial.println(valorBtnAltura);
      }
    }

    // Serial.print("Altura: ");
    // Serial.println(valorBtnAltura);
  
    // GARRA_LEFT
    if (command == "{\"garra\":\"left\"}") {
      if (valorBtnGarra < 90) {
        valorBtnGarra = valorBtnGarra + 5; 

        Serial.println(valorBtnGarra);
      }
    }
  
    // GARRA_RIGHT
    if (command == "{\"garra\":\"right\"}") {
      if (valorBtnGarra != 0) {
        valorBtnGarra = valorBtnGarra - 5; 

        Serial.println(valorBtnGarra);
      }
    }
    
    // Serial.print("Garra: ");
    // Serial.println(valorBtnGarra); 

    angulo.write(valorBtnAngulo);
    base.write(valorBtnBase);
    altura.write(valorBtnAltura);
    garra.write(valorBtnGarra);
  }

  delay(100);
}
