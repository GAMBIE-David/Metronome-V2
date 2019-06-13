/*
 Name:		Metronome_V2.ino
 Created:	14/08/2018 10:54:26
 Author:	Zalman
*/

// the setup function runs once when you press reset or power the board


// Include
//#include <gfxfont.h>
//#include <Adafruit_SPITFT_Macros.h>
//#include <Adafruit_SPITFT.h>
//#include <gfxfont.h>
//#include <Adafruit_SPITFT_Macros.h>
//#include <Adafruit_SPITFT.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
//#include <SPI.h>
#include <Adafruit_SSD1306.h>


//Define pour l'ecran et les boutons
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define BTN_UP 13
#define BTN_DWN 11
#define BTN_LFT 12
#define BTN_RGT 7
#define BUZZER 4
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnMENU 4

//Variable Metronome
int glbTemps = 0; //variable pour les temps des mesures
int glbMesure = 1;
int glbMesureMoinsUn = 1;
int nbTemps = 4;
int nbMesure = 12;

// In millisecondes.
const long FREQUENCY = 700;
int tempo;
long currentTime;
long oldTime;
bool etatLed;

//Vérification pour les bibliotheques
#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup() {
	//Initialisation des sorties
	pinMode(BTN_LFT, INPUT_PULLUP);
	pinMode(BTN_DWN, INPUT_PULLUP);
	pinMode(BTN_UP, INPUT_PULLUP);
	pinMode(BTN_RGT, INPUT_PULLUP);
	pinMode(BUZZER, OUTPUT);


	//init de l'ecran:
	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
												// init done
												// Clear the buffer.
	display.clearDisplay();

	currentTime = 0;
	oldTime = 0;
	//etatLed = LOW;
	tempo = 120;
	Serial.begin(9600);
	afficherTexteAvecSaut("METRONOME for FG", 2);
	delay(1000);
	display.clearDisplay();

}

// the loop function runs over and over again until power down or reset
void loop() {


	if (appuisBtn () != 100)
	{
		reglageTempo();
	}
	metronome(tempo);	
	menuPrincipal(appuisBtn());

}

int ecriture(String str, int16_t x, int16_t y, int taille, int couleur , int inversion)
{
	display.setTextColor(WHITE);
	if (inversion)
		display.setTextColor(BLACK, WHITE);
	display.setTextSize(taille);
	display.setTextColor(couleur);
	display.setCursor(x, y);
	display.println(str);
	display.display();
}

int ecranPrincipal(int mesure, int tempo)
{	
	display.clearDisplay();
	String strMesure = String(mesure);
	String strTempo = String(tempo);
	strMesure = "Mesure : " + strMesure;
	strTempo = "Tempo : " + strTempo + " bpm";

	ecriture(" METRONOME", 0, 0, 2, WHITE,0);
	ecriture(strTempo, 0, 16, 1, WHITE,0);
	ecriture(strMesure, 0, 24, 1, WHITE,0);
}

int appuisBtn() //Fonction pour la gestion des boutons
{
	if (digitalRead(BTN_UP) == 0)
	{
		
		if ((digitalRead(BTN_UP) == 0) && (digitalRead(BTN_DWN) == 0))
		{
			//delay(50);
			return btnMENU;
		}
		delay(10);
		return btnUP;
	}

	if (digitalRead(BTN_DWN) == 0)
	{
		delay(10);
		return btnDOWN;
	}

	if (digitalRead(BTN_LFT) == 0)
	{
		delay(10);
		return btnLEFT;
	}

	if (digitalRead(BTN_RGT) == 0)
	{
		delay(10);
		return btnRIGHT;
	}

	

	return 100;
}

//Fonction reglage du tempo
int reglageTempo() {
	if (appuisBtn() == btnUP)
	{
		tempo = tempo + 1;
		delay(200);
	}

	if (appuisBtn() == btnDOWN)
	{
		tempo = tempo - 1;
		delay(200);
	}

	if (appuisBtn() == btnRIGHT)
	{
		tempo = tempo + 10;
		delay(200);
	}

	if (appuisBtn() == btnLEFT)
	{
		tempo = tempo - 10;
		delay(200);
	}

	return tempo;

}

//fonction calcul de la fr�quence par rapport au tempo
float temps(int Tempo) {
	float frequence = 0;
	float temps = 0;
	frequence = (float)60 / tempo;
	temps = frequence * 1000;
	return temps;

}

//Fonction pour le compte des mesures
int mesure() {
	glbTemps = glbTemps + 1;

	if (glbTemps > nbTemps)
	{
		glbMesure = glbMesure + 1;
		glbTemps = 1;
		if (glbMesure >= nbMesure + 1)
		{
			glbMesure = 1;
		}

	}
	return glbMesure;
}

//fontion pour le reglage du temps
void reglageTemps()
{
	bool menu = HIGH;
	int temps = nbTemps;
	display.clearDisplay();
	display.clearDisplay();
	ecriture("Rgl temps", 0, 0, 2, WHITE, 0);
	ecriture("Nombre temps :" + String(temps), 0, 20, 1, WHITE, 0);

	while (menu)
	{

		if (appuisBtn() == btnUP)
		{
			temps = temps + 1;
			display.clearDisplay();
			ecriture("Rgl temps", 0, 0, 2, WHITE, 0);
			ecriture("Nombre temps :" + String(temps), 0, 20, 1, WHITE, 0);
			delay(200);
		}

		if (appuisBtn() == btnDOWN)
		{
			temps = temps - 1;
			display.clearDisplay();
			ecriture("Rgl temps", 0, 0, 2, WHITE, 0);
			ecriture("Nombre temps :" + String(temps), 0, 20, 1, WHITE, 0);
			delay(200);
		}

		if (appuisBtn() == btnRIGHT)
		{
			nbTemps = temps;
			display.clearDisplay();
			ecriture("Rgl temps", 0, 0, 2, WHITE, 0);
			ecriture("Reglage temps ok", 0, 20, 1, WHITE, 0);
			delay(1000);
			menu = LOW;
			display.clearDisplay();
		}

		if (appuisBtn() == btnLEFT)
		{
			display.clearDisplay();
			ecriture("Rgl temps", 0, 0, 2, WHITE, 0);
			ecriture("ANNULE", 0, 20, 1, WHITE, 0);
			delay(1000);
			menu = LOW;
			display.clearDisplay();
		}
	}
}

//Fonction pour le reglage de la mesure
void reglageMesure()
{
	bool menu = HIGH;
	int mesure = nbMesure;
	display.clearDisplay();
	ecriture("Rgl mesure", 0, 0, 2, WHITE, 0);
	ecriture("Nombre mesure :" + String(mesure), 0, 20, 1, WHITE, 0);

	while (menu)
	{

		if (appuisBtn() == btnUP)
		{
			mesure = mesure + 1;
			display.clearDisplay();
			ecriture("Rgl mesure", 0, 0, 2, WHITE, 0);
			ecriture("Nombre mesure :" + String(mesure), 0, 20, 1, WHITE, 0);
			delay(200);
		}

		if (appuisBtn() == btnDOWN)
		{
			mesure = mesure - 1;
			display.clearDisplay();
			ecriture("Rgl mesure", 0, 0, 2, WHITE, 0);
			ecriture("Nombre mesure :" + String(mesure), 0, 20, 1, WHITE, 0);
			delay(200);
		}

		if (appuisBtn() == btnRIGHT)
		{
			nbMesure = mesure;
			display.clearDisplay();
			ecriture("Rgl mesure", 0, 0, 2, WHITE, 0);
			ecriture("Reglage mesure OK", 0, 20, 1, WHITE, 0);
			delay(1000);
			menu = LOW;
			display.clearDisplay();
		}

		if (appuisBtn() == btnLEFT)
		{
			display.clearDisplay();
			ecriture("Rgl mesure", 0, 0, 2, WHITE, 0);
			ecriture("ANNULE", 0, 20, 1, WHITE, 0);
			menu = LOW;
			delay(1000);
			display.clearDisplay();
		}
	}


}

void menuPrincipal(int navigation)
{
	bool menu = HIGH;
	int positionMenu = 0;

	if (navigation == btnMENU)
	{
		navigation = 0;
		display.clearDisplay();
		do
		{

			navigation = appuisBtn();

			if (navigation == btnUP)
			{
				positionMenu = (positionMenu + 1) % 2;
				delay(200);
				display.clearDisplay();
			}

			if (navigation == btnDOWN)
			{
				positionMenu = (positionMenu - 1) % 2;
				if (positionMenu == -1)
					positionMenu = 1;
				delay(200);
				display.clearDisplay();
			}

			if (navigation == btnLEFT)
			{
				menu = LOW;
				positionMenu = 10;
				delay(200);
			}

			if (navigation == btnRIGHT)
			{
				delay(200);
				switch (positionMenu)
				{
				case 0:
					reglageMesure();
					break;

				case 1:
					reglageTemps();
					break;

				default:
					break;
				}

			}

			switch (positionMenu)
			{
			case 0:
				ecriture("MENU : ", 0, 0, 2, WHITE, 0);
				ecriture("->REGLAGE MESURE", 0, 15, 1, WHITE,1);
				ecriture("Reglage temps", 0, 23, 1, WHITE, 0);
				Serial.println("Reglage mesure");
				break;

			case 1:
				ecriture("MENU : ", 0, 0, 2, WHITE, 0);
				ecriture("->REGLAGE TEMPS", 0, 23, 1, WHITE,1);
				ecriture("Reglage mesure", 0, 15, 1, WHITE, 0);
				Serial.println("Reglage temps");
				break;

			default:
				break;
			}

		} while (menu);
	}

}

void metronome(int tempo)
{
	currentTime = millis();
	if (currentTime >= (oldTime + temps(tempo)))
	{
		digitalWrite(BUZZER, HIGH);
		display.invertDisplay(true);
		mesure();
		delay(100);
		digitalWrite(BUZZER, LOW);
		display.invertDisplay(false);
		Serial.print(tempo);
		oldTime = currentTime;
		ecranPrincipal(glbMesure, tempo);
	}
}

int afficherTexte(char chaine[], int taille, int ligne)
{
	char c = 0;	// pour stocker le caract�re en cours
	int i = 0;	// pour parcourir la chaine
	int x = 0;	//position x dans l'affichage
	ligne = ligne * (taille * 7);

	//display.clearDisplay();
	while (chaine[i] != '\0')
	{
		c = chaine[i];
		display.drawChar(x, ligne, c, WHITE, 0, taille);
		display.display();
		x = x + (taille * 6);
		i++;
	}



	return 0;
}

int afficherTexteAvecSaut(char chaine[], int taille)
{
	char c = 0;
	int i = 0;
	int x = 0;
	int y = 0;

	while (chaine[i] != '\0')
	{
		c = chaine[i];
		display.drawChar(x, y, c, 1, 0, taille);
		display.drawChar(x, y, c, WHITE, 0, taille);
		display.display();
		x = x + 15;
		i++;
		if (x > 120)
		{
			x = 0;
			y = y + 15;
		}

	}


	return 0;
}