#include "MainComponent.h"

//==============================================================================

MainComponent::~MainComponent()
{
    shutdownAudio();
}
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    audioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    audioSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    audioSource.releaseResources();
}

//==============================================================================
void MainComponent::resized()
{
    midiInputList.setBounds(200, 10, getWidth() - 210, 20);
    keyboardComponent.setBounds (10, 40, getWidth() - 20, getHeight() - 130);
    envelopeSliders.setBounds(10, 40 + (getHeight() - 130), getWidth() - 150, 80);
}

void MainComponent::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}

void MainComponent::setMidiInput(int index)
{
    auto list = juce::MidiInput::getAvailableDevices();
    
    deviceManager.removeMidiInputDeviceCallback(list[lastInputIndex].identifier, audioSource.getMidiCollector());
    auto newInput = list[index];
    if(!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
        deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);
    deviceManager.addMidiInputDeviceCallback(newInput.identifier, audioSource.getMidiCollector());
    midiInputList.setSelectedId(index + 1, juce::dontSendNotification);
    
    lastInputIndex = index;
    
}
