//
//  ControlComponents.cpp
//  MonoFM1 - App
//
//  Created by Hayden Setlik on 8/26/20.
//

#include "ControlComponents.hpp"

adsrSliderSet::adsrSliderSet()
{
    addAndMakeVisible(aSlider);
    aSlider.setRange(0.0, 2.0); //attack length can be between 0 and 2 seconds
    aSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    
    addAndMakeVisible(dSlider);
    dSlider.setRange(0.0, 3.0); //decay between 0 and 3 seconds
    dSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    
    addAndMakeVisible(sSlider);
    sSlider.setRange(0.0, 1.0); //sustain between 0% and 100% of max volume
    sSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    
    addAndMakeVisible(rSlider);
    rSlider.setRange(0.0, 4.0); //release between 0 and 4 seconds
    rSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
}

void adsrSliderSet::resized()
{
    aSlider.setBounds(10, getHeight() - 80, getWidth(), 20);
    dSlider.setBounds(10 , getHeight() - 60, getWidth(), 20);
    sSlider.setBounds(10, getHeight() - 40, getWidth(), 20);
    rSlider.setBounds(10, getHeight() - 20, getWidth(), 20);
}
