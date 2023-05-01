/*
  ==============================================================================

	Credit: https://github.com/remberg/juceCustomSliderSample

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class customLookAndFeel : public LookAndFeel_V4
{
public:
	customLookAndFeel();

	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;

	Typeface::Ptr getTypefaceForFont(const Font& f) override;

private:
	Image img;
};	
