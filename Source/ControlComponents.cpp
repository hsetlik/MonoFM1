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

adsrAmplifier::adsrAmplifier(adsrSliderSet* sliders)
{
    //remember that sourceSliderSet is a pointer
    sourceSliderSet = sliders;
    attackVal = sourceSliderSet->aSlider.getValue();
    decayVal = sourceSliderSet->dSlider.getValue();
    sustainVal = sourceSliderSet->sSlider.getValue();
    releaseVal = sourceSliderSet->rSlider.getValue();
}

void adsrAmplifier::updateSettings()
{
    attackVal = (double) sourceSliderSet->aSlider.getValue();
    //printf("updated attack value is: %f\n", attackVal);
    decayVal = (float) sourceSliderSet->dSlider.getValue();
    sustainVal = (float) sourceSliderSet->sSlider.getValue();
    releaseVal = (float) sourceSliderSet->rSlider.getValue();
}

double adsrAmplifier::nextSampleAmplitude(int sampleRate, bool noteIsOn)
{
    updateSettings();
    int sampleIndex = envelopeSampleIndex;
    double attackLengthSamples = sampleRate * (attackVal + 0.000001);//avoiding a divide by zero error
    //printf("Attack length: %f\n", attackLengthSamples);
    double decayLengthSamples = sampleRate * (decayVal + 0.0001);
    double releaseLengthSamples = sampleRate * (releaseVal + 0.0001);
    double sustainLevel = sustainVal;
    //delta values by sample for each phase
    double decayDelta = (1.0 - sustainVal) / decayLengthSamples;
    double releaseDelta = sustainLevel / releaseLengthSamples;
    if(sampleIndex < attackLengthSamples) //rules for the attack phase
    {
        ++gateSampleLength;
        double attackDelta = (1.0 / attackLengthSamples);
        //printf("attack delta = 1 / %d\n", attackLengthSamples);
        //printf("decimal attackDelta = %f\n", attackDelta);
        double attackOutput = (attackDelta * (double)(sampleIndex + 0.000001));
        //printf("pre-division output = %f\n", attackOutput);
        printf("attack amp: %f at sample %d\n", attackOutput, sampleIndex);
        ++envelopeSampleIndex;
        return attackOutput;
    } else if(sampleIndex < (attackLengthSamples + decayLengthSamples)) //rules for the decay phase
    {
        ++gateSampleLength;
        double decaySampleIndex = sampleIndex - attackLengthSamples;
        printf("decay amp: %f\n", 1.0 - (decayDelta * decaySampleIndex));
        ++envelopeSampleIndex;
        return 1.0 - (decayDelta * decaySampleIndex);
    } else if(noteIsOn) //sustain phase
    {
        ++gateSampleLength;
        printf("sustain amp: %f\n", sustainLevel);
        ++envelopeSampleIndex;
        return sustainLevel;
    } else if ((gateSampleLength + 0.000001) < sampleIndex)// release phase
    {
        int releaseIndex = sampleIndex - (double)gateSampleLength;
        printf("release amp: %f\n", sustainLevel - (releaseDelta * releaseIndex));
        ++envelopeSampleIndex;
        return sustainLevel - (releaseDelta * releaseIndex);
    } else {
        envelopeSampleIndex = 0;
        return 0.0;
    }
}

