//
//  ControlComponents.hpp
//  MonoFM1 - App
//
//  Created by Hayden Setlik on 8/26/20.
//

#pragma  once
#include <JuceHeader.h>

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
private:
    AttackSlider aSlider;
    DecaySlider dSlider;
    SustainSlider sSlider;
    ReleaseSlider rSlider;
};

