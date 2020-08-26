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
    aSlider.setValue(1.0);
    
    addAndMakeVisible(dSlider);
    dSlider.setRange(0.0, 3.0); //decay between 0 and 3 seconds
    dSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    dSlider.setValue(1.5);
    
    addAndMakeVisible(sSlider);
    sSlider.setRange(0.0, 1.0); //sustain between 0% and 100% of max volume
    sSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    sSlider.setValue(0.5);
    
    addAndMakeVisible(rSlider);
    rSlider.setRange(0.0, 4.0); //release between 0 and 4 seconds
    rSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 100, 20);
    rSlider.setValue(2.0);
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
void adsrAmplifier::getInitialLength(int sampleRate)
{
    double attackLengthSamples = attackVal * (double) sampleRate;
    double decayLengthSamples = decayVal * (double) sampleRate;
    envelopeSampleLength = (int) (attackLengthSamples + decayLengthSamples);
    printf("initial length setting: %d\n", envelopeSampleLength);
}

double adsrAmplifier::nextSampleAmplitude(int sampleRate, bool noteIsOn)
{
    updateSettings();
    double attackLengthSamples = attackVal * (double) sampleRate;
    //envelopeSampleLength += attackLengthSamples;
    double decayLengthSamples = decayVal * (double) sampleRate;
    //envelopeSampleLength += decayLengthSamples;
     double releaseLengthSamples = releaseVal * (double) sampleRate;
    double samplesBeforeSustain = attackLengthSamples + decayLengthSamples; //keep track of how many samples elapse before sustain
    printf("envelope length: %d\n", envelopeSampleLength);
    if(envelopeSampleIndex < attackLengthSamples && noteIsOn)
    {
        printf("In attack Stage\n");
        currentStage = Attack;
    } else if (envelopeSampleIndex < samplesBeforeSustain && noteIsOn)
    {
        printf("In decay Stage\n");
        currentStage = Decay;
    } else if (envelopeSampleIndex > (int)samplesBeforeSustain && noteIsOn)
    {
        envelopeSampleLength += 1;
        printf("In sustain Stage\n");
        currentStage = Sustain;
    } else if (envelopeSampleIndex > (envelopeSampleLength) && !noteIsOn && envelopeSampleIndex <= (envelopeSampleLength + releaseLengthSamples))
    {
        printf("In release Stage\n");
        currentStage = Release;
    } else
    {
        printf("In off Stage\n");
        currentStage = Off;
    }
    //second step: calculate the deltas per sample for the attack, decay, and release lines
    double attackDelta = 1.0 / (attackLengthSamples + 0.0001); //avoiding divide-by-zero error
    double decayDelta = (1.0 - sustainVal) / (decayLengthSamples + 0.0001);
    double releaseDelta = sustainVal / (releaseLengthSamples + 0.0001);
    //third: determine the correct coefficient for each delta based on the index
    double attackIndex = envelopeSampleIndex;
    double decayIndex = envelopeSampleIndex - attackLengthSamples;
    double releaseIndex = envelopeSampleIndex - envelopeSampleLength;
    //fourth: determine which delta/index pair to use with a switch statement
    double returnValue;
    switch(currentStage){
        case Attack:
            returnValue = attackDelta * attackIndex;
            break;
        case Decay:
            returnValue = 1.0 - (decayDelta * decayIndex);
            break;
        case Sustain:
            returnValue = sustainVal;
            break;
        case Release:
            returnValue = sustainVal - (releaseIndex * releaseDelta);
            break;
        case Off:
            returnValue = 0.0;
            envelopeSampleIndex = 0.0;
            envelopeSampleLength = 0.0;
            break;
    }
    printf("sample #: %d\n", envelopeSampleIndex);
    printf("amplitude setting: %f\n", returnValue);
    envelopeSampleIndex += 1.0;
    return returnValue;
}

