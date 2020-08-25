//
//  SineSound.h
//  MonoFM1
//
//  Created by Hayden Setlik on 8/25/20.
//

#pragma once
#include <JuceHeader.h>

struct SineWaveSound : public juce::SynthesiserSound
{
    SineWaveSound() {};
    bool appliesToNote (int) override {return true;};
    bool appliesToChannel (int) override {return true;};
};

struct SineWaveVoice : public juce::SynthesiserVoice
{
    SineWaveVoice() {}
    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*>(sound) != nullptr;
    }
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;
        
        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();
        angleDelta = cyclesPerSample * 2 * juce::MathConstants<double>::pi;
    }
    
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if(angleDelta != 0.0)
        {
            if(tailOff > 0.0)
            {
                while(--numSamples >=0)
                {
                    auto currentSample = (float) (std::sin(currentAngle) * level * tailOff);
                    for(auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);
                    currentAngle += angleDelta;
                    ++startSample;
                    
                    tailOff *= 0.99;
                    if(tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        angleDelta = 0.0;
                        break;
                    }
                }
            } else
            {
              while(--numSamples >= 0)
              {
                  auto currentSample = (float) (std::sin(currentAngle) * level);
                  for(auto i = outputBuffer.getNumChannels(); --i >= 0;)
                      outputBuffer.addSample(i, startSample, currentSample);
                  currentAngle += angleDelta;
                  ++startSample;
              }
            }
            
        }
    }
    /*note: the wheel and controller functions MUST be defined and overridden before the
    SineWaveVoice can be instantiated anywhere else*/
    void pitchWheelMoved (int) override {}
    void controllerMoved (int, int) override {}
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        if(allowTailOff)
        {
            if(tailOff == 0.0)
                tailOff = 1.0;
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }
private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
};
