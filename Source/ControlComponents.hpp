//
//  ControlComponents.hpp
//  MonoFM1 - App
//
//  Created by Hayden Setlik on 8/26/20.
//

#pragma  once
#include <JuceHeader.h>
#include <stdio.h>

class AttackSlider : public juce::Slider
{
public:
    AttackSlider() {}
    double getValueFromText (const juce::String& text) override
        {
            return text.getDoubleValue() ;// [2]
        }
    juce::String getTextFromValue (double value) override
    {
        juce::String displayValue = juce::String(value);
        auto displayText = displayValue.substring(0, 4);
        juce::String unitLabel = juce::String(" s");
        return displayText + unitLabel;
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AttackSlider)
};

class DecaySlider : public juce::Slider
{
public:
    DecaySlider() {}
    double getValueFromText (const juce::String& text) override
        {
            return text.getDoubleValue() ;// [2]
        }
    juce::String getTextFromValue (double value) override
    {
        juce::String displayValue = juce::String(value);
        auto displayText = displayValue.substring(0, 4);
        juce::String unitLabel = juce::String(" s");
        return displayText + unitLabel;
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DecaySlider)
};

class SustainSlider : public juce::Slider
{
public:
    SustainSlider() {}
    double getValueFromText (const juce::String& text) override
        {
            return text.getDoubleValue() ;// [2]
        }
    juce::String getTextFromValue (double value) override
    {
        double percentValue = value * 100;
        juce::String displayValue = juce::String(percentValue);
        auto displayText = displayValue.substring(0, 4);
        juce::String unitLabel = juce::String("%");
        return displayText + unitLabel;
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SustainSlider)
};

class ReleaseSlider : public juce::Slider
{
public:
    ReleaseSlider() {}
    double getValueFromText (const juce::String& text) override
        {
            return text.getDoubleValue() ;// [2]
        }
    juce::String getTextFromValue (double value) override
    {
        juce::String displayValue = juce::String(value);
        auto displayText = displayValue.substring(0, 4);
        juce::String unitLabel = juce::String(" s");
        return displayText + unitLabel;
    }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReleaseSlider)
};


class adsrSliderSet : public juce::Component
{
public:
    adsrSliderSet();
    void resized() override;
    AttackSlider aSlider;
    DecaySlider dSlider;
    SustainSlider sSlider;
    ReleaseSlider rSlider;
};

class adsrAmplifier
{
public:
    adsrAmplifier(adsrSliderSet* sliders);
    enum stage
    {
        Attack,
        Decay,
        Sustain,
        Release,
        Off
    };
    void getInitialLength(int sampleRate);
    void updateSettings();
    double nextSampleAmplitude(int sampleRate, bool noteIsOn);
private:
    double attackVal = 0.0, decayVal = 0.0, sustainVal = 0.0, releaseVal = 0.0;
    adsrSliderSet* sourceSliderSet;
    int envelopeSampleLength = 0;
    int envelopeSampleIndex = 0;
    stage currentStage;
};
