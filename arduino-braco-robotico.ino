#include <Servo.h>
  
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
Servo base; 
Servo altura;
Servo angulo;
Servo garra;

//Variaveis para cálculo dos angulos
int valorPotBase;
int valorAngBase = 90;
int valorPotAltura;
int valorAngAltura;
int valorPotAngulo;
int valorAngAngulo;
int valorPotGarra;
int valorAngGarra;

//Variaveis para controle dos botões
int estadoBotaoC, estadoBotaoD;
int estadoAntBotaoC = HIGH;
int estadoAntBotaoD = HIGH;
boolean congelado = false;

void setup(){
  Serial.begin(9600);
  
  pinMode(botaoCongela, INPUT_PULLUP);
  pinMode(botaoDescongela, INPUT_PULLUP);
    
  //Configuração das Portas dos Servos
  base.attach(servoBase);
  altura.attach(servoAlturaBraco);
  angulo.attach(servoAnguloBraco);
  garra.attach(servoGarra); 

  base.write(valorAngBase);
}


void loop() {
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
