/*
  ==============================================================================

    xypad.cpp
    Created: 9 Apr 2021 5:40:42pm
    Author:  Stewart Blackwood

  ==============================================================================
*/

#include "xypad.h"


    XYPad::XYPad()
    {
        XYPad::MouseListener();
        setFramesPerSecond(60);
    }

    void XYPad::update()
    {
        //put the gui into the center
        if(!started){
        x = getWidth()/2;
        y = getHeight()/2;
            started = true;
        }
    }
    
    void XYPad::paint (juce::Graphics& g)
    {
        //set the GUI based on the slider value
        int pointx = xsliderconversion(returnxsliderval);
        int pointy = ysliderconversion(returnysliderval);
        xslidervalue = returnxsliderval;
        yslidervalue = returnysliderval;
        juce::Point<float> p (pointx,pointy);
        
        //setup the rectangle
        g.fillAll (juce::Colours::whitesmoke);
        g.drawRect(0, 0, getWidth(), getHeight(), 3);
        
        //animate the clickable circle GUI
        float bob = radius * ((sin((getFrameCounter() % 60)/20.0f) * .05) + .95);
        //fill the gui
        g.fillEllipse (p.x - (radius/2), p.y - (radius/2), bob, bob);
        
    }

    void XYPad::mouseDown (const juce::MouseEvent& event)
    {
        //get the x/y position
        x = event.getMouseDownX();
        y = event.getMouseDownY();
        
        //convert it to the
        xslidervalue = ((float)(x)/getWidth() * 2) - 1;
        yslidervalue = (((float)(y)/getHeight() * 2) - 1) * -1;
        
        //has the dot been clicked
        clicked = true;
    }
    
    void XYPad::mouseUp (const juce::MouseEvent& event)
    {
        //has the dot been clicked
        clicked = false;
    }
    void XYPad::mouseDrag (const juce::MouseEvent& event)
    {
        //k is a point, and you get the position in pixel cooridantes
        k = event.getPosition();
        
        //check to see if the point leaves the box
        float tempxcheck = (float) k.getX()/getWidth();
        float tempycheck = (float) k.getY()/getHeight();
        if (tempxcheck <= 1 && tempxcheck >= 0)
        x = k.getX();
        if (tempycheck <= 1 && tempycheck >= 0)
        y = k.getY();
        
        //get a value of -1 to 1 from pixel #
        xslidervalue = ((float)(x)/getWidth() * 2) - 1;
        yslidervalue = (((float)(y)/getHeight() * 2) - 1) * -1;
    
    }

    void XYPad::mouseDoubleClick (const juce::MouseEvent& event)
    {
        //put the gui and slider values to 0,0
        x = getWidth()/2;
        y = getHeight()/2;
        
        xslidervalue = ((float)(x)/getWidth() * 2) - 1;
        yslidervalue = (((float)(y)/getHeight() * 2) - 1) * -1;
        clicked = true;
    }

    int XYPad::xsliderconversion (float sliderval)
    {
        //convert from slider value to pixel #
        int xval = (getWidth() * (sliderval + 1))/2;
        return xval;
    }
    
    int XYPad::ysliderconversion (float sliderval)
    {
        //convert from slider value to pixel #
        int yval = (getHeight() * (-1 * sliderval + 1))/2;
        
        return yval;
    }
