//
//  SynthAudioSource.hpp
//  MonoFM1 - App
//
//  Created by Hayden Setlik on 8/25/20.
//

#pragma once
#include <JuceHeader.h>
#include "SineSound.h"
#include "SawtoothSound.h"

class SynthAudioSource : public juce::AudioSource
{
public:
    SynthAudioSource(juce::MidiKeyboardState &keyState)
    : keyboardState(keyState)
    {
        for (auto i = 0; i < 4; ++i)                // [1]
            synth.addVoice(new SawtoothVoice());
        
        synth.addSound (new SawWaveSound());
    };
    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        midiCollector.reset(sampleRate);
    }

    void releaseResources() override {}

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();

        juce::MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
        //pulling data from the keyboardState object into a buffer
        keyboardState.processNextMidiBuffer (incomingMidi, bufferToFill.startSample,
                                             bufferToFill.numSamples, true);
        
        //pasing the data from that buffer into the synth
        synth.renderNextBlock (*bufferToFill.buffer, incomingMidi,
                               bufferToFill.startSample, bufferToFill.numSamples); // [5]
    }
    
    juce::MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }

private:
    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
    juce::MidiMessageCollector midiCollector;
};
