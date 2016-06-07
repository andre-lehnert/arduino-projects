#include "AlaLite.h"
#include "AlaLedLite.h"

#include "ExtNeoPixel.h"


Adafruit_NeoPixel *neopixels;




AlaLedLite::AlaLedLite()
{
	// set default values

	maxOut = 0xFFFFFF;
	speed = 1000;
	animSeqLen = 0;
	lastRefreshTime = 0;
	refreshMillis = 1000/50;
	pxPos = NULL;
	pxSpeed = NULL;
}

void AlaLedLite::initStripe(int numLeds, byte pin, byte type) // CHANGE: SINGLE STRIPE
{
	this->driver = ALA_WS2812;
	this->numLeds = numLeds;
	this->pins = 0;
	this->sides = 1;
	this->numLedsPerSide = numLeds;

	// allocate and clear leds array
	leds = (AlaColor *)malloc(3*numLeds);
	memset(leds, 0, 3*numLeds);

	neopixels = new Adafruit_NeoPixel(numLeds, pin, type);

	neopixels->begin();
}

void AlaLedLite::initBarStripes(byte pin, byte type) // CHANGE: 4 sides
{
	this->driver = ALA_WS2812;
	this->numLedsPerSide = 11;
	this->sides = 4;

	this->numLeds = numLedsPerSide * sides; // CHANGE
	this->pins = 0;

	// allocate and clear leds array
	leds = (AlaColor *)malloc(3*numLeds);
	memset(leds, 0, 3*numLeds);

	neopixels = new Adafruit_NeoPixel(numLeds, pin, type);

	neopixels->begin();
}


void AlaLedLite::setBrightness(AlaColor maxOut)
{
	this->maxOut = maxOut;
}

void AlaLedLite::setRefreshRate(int refreshRate)
{
	this->refreshMillis = 1000/refreshRate;
}

int AlaLedLite::getCurrentRefreshRate()
{
	return refreshRate;
}


void AlaLedLite::setAnimation(int animation, long speed, AlaColor color)
{
	// is there any change?
	if (this->animation == animation && this->speed == speed && this->palette.numColors == 1 && this->palette.colors[0] == color)
		return;

	// delete any previously allocated array
	if (pxPos!=NULL)
	{ delete[] pxPos; pxPos=NULL; }
	if (pxSpeed!=NULL)
	{ delete[] pxSpeed; pxSpeed=NULL; }

	this->animation = animation;
	this->speed = speed;

	this->palette.numColors = 1;
	this->palette.colors = (AlaColor*)malloc(3);
	this->palette.colors[0] = color;

	setAnimationFunc(animation);
	animStartTime = millis();
}

void AlaLedLite::setAnimation(int animation, long speed, AlaPalette palette)
{
	// is there any change?
	if (this->animation == animation && this->speed == speed && this->palette == palette)
		return;

	// delete any previously allocated array
	if (pxPos!=NULL)
	{ delete[] pxPos; pxPos=NULL; }
	if (pxSpeed!=NULL)
	{ delete[] pxSpeed; pxSpeed=NULL; }

	this->animation = animation;
	this->speed = speed;
	this->palette = palette;

	setAnimationFunc(animation);
	animStartTime = millis();
}


void AlaLedLite::setAnimation(AlaSeq animSeq[])
{
	this->animSeq = animSeq;

	// initialize animSeqDuration and animSeqLen variables
    animSeqDuration = 0;
    for(animSeqLen=0; animSeq[animSeqLen].animation!=ALA_ENDSEQ; animSeqLen++)
    {
		animSeqDuration = animSeqDuration + animSeq[animSeqLen].duration;
    }
}

void AlaLedLite::nextAnimation()
{
	currAnim = (currAnim+1)%animSeqLen;
}

bool AlaLedLite::runAnimation()
{
    // skip the refresh if not enough time has passed since last update
	unsigned long cTime = millis();
	if (cTime < lastRefreshTime + refreshMillis)
		return false;

	// calculate real refresh rate
	refreshRate = 1000/(cTime - lastRefreshTime);

	lastRefreshTime = cTime;


	if (animSeqLen != 0)
    {
		if(animSeq[currAnim].duration == 0)
		{
			setAnimation(animSeq[currAnim].animation, animSeq[currAnim].speed, animSeq[currAnim].palette);
		}
		else
		{
			long c = 0;
			long t = cTime % animSeqDuration;
			for(int i=0; i<animSeqLen; i++)
			{
				if (t>=c && t<(c+animSeq[i].duration))
				{
					setAnimation(animSeq[i].animation, animSeq[i].speed, animSeq[i].palette);
					break;
				}
				c = c + animSeq[i].duration;
			}
		}
    }

    if (animFunc != NULL)
    {
		(this->*animFunc)();

		// use an 8 bit shift to divide by 256

		if(driver==ALA_WS2812)
		{
			// this is not really so smart...
			for(int i=0; i<numLeds; i++)
				neopixels->setPixelColor(i, neopixels->Color((leds[i].r*maxOut.r)>>8, (leds[i].g*maxOut.g)>>8, (leds[i].b*maxOut.b)>>8));

			neopixels->show();
		}
	}

	return true;
}


///////////////////////////////////////////////////////////////////////////////

void AlaLedLite::setAnimationFunc(int animation)
{

    switch(animation)
	{
		case ALA_ON:                    animFunc = &AlaLedLite::on;                    break;
		case ALA_OFF:                   animFunc = &AlaLedLite::off;                   break;
		case ALA_BLINK:                 animFunc = &AlaLedLite::blink;                 break;

		case ALA_PIXELSMOOTHSHIFTRIGHT: animFunc = &AlaLedLite::pixelSmoothShiftRight; break;
		case ALA_PIXELSMOOTHSHIFTLEFT:  animFunc = &AlaLedLite::pixelSmoothShiftLeft;  break;
		case ALA_PIXELSMOOTHBOUNCE:     animFunc = &AlaLedLite::pixelSmoothBounce;     break;
		case ALA_COMET:                 animFunc = &AlaLedLite::comet;                 break;

		case ALA_LARSONSCANNER:         animFunc = &AlaLedLite::larsonScanner;         break;

		case ALA_GLOW:                  animFunc = &AlaLedLite::glow;                  break;

		default:                        animFunc = &AlaLedLite::off;
	}

}


void AlaLedLite::on()
{
	for(int i=0; i<numLeds; i++)
	{
		leds[i] = palette.colors[0];
	}
}

void AlaLedLite::off()
{
	for(int i=0; i<numLeds; i++)
	{
		leds[i] = 0x000000;
	}
}


void AlaLedLite::blink()
{
	int t = getStep(animStartTime, speed, 2);
	int k = (t+1)%2;
	for(int x=0; x < numLeds; x++) // <-- CHANGE
	{
			leds[x] = palette.colors[0].scale(k);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////
// Shifting effects
////////////////////////////////////////////////////////////////////////////////////////////


void AlaLedLite::pixelSmoothShiftRight() // CHANGE
{
	float t = getStepFloat(animStartTime, speed, numLeds+1);
	float tx = getStepFloat(animStartTime, speed, palette.numColors);
	AlaColor c = palette.getPalColor(tx);

	for(int x = 0; x < numLedsPerSide; x++)
	{
			float k = max(0, (-abs(t-1-x)+1));
			for (int s = 0; s < sides; s++) {
				switch (s) {
					case 0: leds[numLedsPerSide - (x+1)] = c.scale(k); 				break;	// Side A
					case 1: leds[numLedsPerSide + x] = c.scale(k); 						break; 	// Side B
					case 2: leds[3*numLedsPerSide - (x+1)] = c.scale(k); 			break;	// Side C
					case 3: leds[3*numLedsPerSide + x] = c.scale(k); 					break; 	// Side D
					default: break;
				}
			}
	}
}

void AlaLedLite::pixelSmoothShiftLeft()
{
	float t = getStepFloat(animStartTime, speed, numLeds+1);
	float tx = getStepFloat(animStartTime, speed, palette.numColors);
	AlaColor c = palette.getPalColor(tx);

	for(int x=0; x<numLedsPerSide; x++)
	{
		float k = max(0, (-abs(numLeds-t-x)+1));
		for (int s = 0; s < sides; s++) {
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = c.scale(k); 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = c.scale(k); 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = c.scale(k); 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = c.scale(k); 					break; 	// Side D
				default: break;
			}
		}
	}
}

void AlaLedLite::comet()
{
	float l = numLeds/2;  // length of the tail
	float t = getStepFloat(animStartTime, speed, 2*numLeds-l);
	float tx = getStepFloat(animStartTime, speed, palette.numColors);
	AlaColor c = palette.getPalColor(tx);

	for(int x=0; x<numLedsPerSide; x++)
	{
		float k = constrain( (((x-t)/l+1.2f))*(((x-t)<0)? 1:0), 0, 1);
		for (int s = 0; s < sides; s++) {
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = c.scale(k); 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = c.scale(k); 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = c.scale(k); 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = c.scale(k); 					break; 	// Side D
				default: break;
			}
		}
	}
}
void AlaLedLite::pixelSmoothBounce()
{
	// see larsonScanner
	float t = getStepFloat(animStartTime, speed, 2*numLeds-2);
	AlaColor c = palette.getPalColor(getStepFloat(animStartTime, speed, palette.numColors));

	for(int x=0; x<numLedsPerSide; x++)
	{
		float k = constrain((-abs(abs(t-numLeds+1)-x)+1), 0, 1);
		for (int s = 0; s < sides; s++) {
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = c.scale(k); 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = c.scale(k); 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = c.scale(k); 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = c.scale(k); 					break; 	// Side D
				default: break;
			}
		}
	}
}


void AlaLedLite::larsonScanner()
{
	float l = numLeds/4;
	float t = getStepFloat(animStartTime, speed, 2*numLeds-2);
	AlaColor c = palette.getPalColor(getStepFloat(animStartTime, speed, palette.numColors));

	for(int x=0; x<numLedsPerSide; x++)
	{
		float k = constrain((-abs(abs(t-numLeds+1)-x)+l), 0, 1);
		for (int s = 0; s < sides; s++) {
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = c.scale(k); 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = c.scale(k); 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = c.scale(k); 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = c.scale(k); 					break; 	// Side D
				default: break;
			}
		}
	}
}




////////////////////////////////////////////////////////////////////////////////////////////
// Fading effects
////////////////////////////////////////////////////////////////////////////////////////////

void AlaLedLite::glow()
{
	float s = getStepFloat(animStartTime, speed, TWO_PI);
	float k = (-cos(s)+1)/2;

	for(int x=0; x<numLedsPerSide; x++)
	{
		//leds[x] = palette.colors[0].scale(k);
		for (int s = 0; s < sides; s++) {
			switch (s) {
				case 0: leds[numLedsPerSide - (x+1)] = palette.colors[0].scale(k); 				break;	// Side A
				case 1: leds[numLedsPerSide + x] = palette.colors[0].scale(k); 						break; 	// Side B
				case 2: leds[3*numLedsPerSide - (x+1)] = palette.colors[0].scale(k); 			break;	// Side C
				case 3: leds[3*numLedsPerSide + x] = palette.colors[0].scale(k); 					break; 	// Side D
				default: break;
			}
		}
	}
}
