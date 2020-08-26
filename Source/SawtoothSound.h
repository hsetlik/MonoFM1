//
//  StoothSound.h
//  MonoFM1
//
//  Created by Hayden Setlik on 8/25/20.
//

#pragma once
#include <JuceHeader.h>
#include "ControlComponents.hpp"

struct SawWaveSound : public juce::SynthesiserSound
{
    SawWaveSound() {};
    bool appliesToNote (int) override {return true;};
    bool appliesToChannel (int) override {return true;};
};


struct SawtoothVoice : public juce::SynthesiserVoice
{
    SawtoothVoice(adsrSliderSet* sliders) : envelopeLevel(sliders)
    {
        referenceSliders = sliders;
    }
    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SawWaveSound*>(sound) != nullptr;
    }
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        envelopeLevel.updateSettings();
        printf("Attack Value %f\n", referenceSliders->aSlider.getValue());
        printf("Decay Value %f\n", referenceSliders->dSlider.getValue());
        printf("Sustain Value %f\n", referenceSliders->sSlider.getValue());
        printf("Release Value %f\n", referenceSliders->rSlider.getValue());
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;
        noteIsHeld = true;
        envelopeIndex = 0;
        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();
        auto samplesPerCycle = 1 / cyclesPerSample;
        rampDelta = 2 / samplesPerCycle;
        //angleDelta will need to change for FM control
        angleDelta = cyclesPerSample * 2 * juce::MathConstants<double>::pi;
    }
    double sawFromAngle(int sampleIndex)
    {
        auto  samplesPerCycle = (int) (2 / rampDelta);
        int indexInCycle = samplesPerCycle % (sampleIndex + 1);
        double offset = indexInCycle * rampDelta;
        return (-1 + offset);
    }
    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        envelopeLevel.updateSettings();
        if(angleDelta != 0.0)
        {
            if(tailOff > 0.0)
            {
                while(--numSamples >=0)
                {
                    auto sampleAmp = envelopeLevel.nextSampleAmplitude(getSampleRate(), noteIsHeld);
                    auto currentSample = (float) (sawFromAngle(startSample) * sampleAmp * tailOff);
                    for(auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);
                    currentAngle += angleDelta;
                    ++startSample;
                    ++envelopeIndex;
                    
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
                  auto sampleAmp = envelopeLevel.nextSampleAmplitude(getSampleRate(), noteIsHeld);
                  auto currentSample = (float) (sawFromAngle(startSample) * sampleAmp);
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
        noteIsHeld = false;
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
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0, rampDelta = 0.0, currentAmp = 0.0;
    bool noteIsHeld = false;
    adsrSliderSet* referenceSliders;
    adsrAmplifier envelopeLevel;
    int envelopeIndex;
};





