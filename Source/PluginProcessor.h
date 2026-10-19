#pragma once
#include <JuceHeader.h>
class FeltRoomProcessor final : public juce::AudioProcessor {
public:
 FeltRoomProcessor();
 void prepareToPlay(double, int) override;
 void releaseResources() override {}
 void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
 juce::AudioProcessorEditor* createEditor() override;
 bool hasEditor() const override { return true; }
 const juce::String getName() const override { return "FeltRoom"; }
 bool acceptsMidi() const override { return false; }
 bool producesMidi() const override { return false; }
 bool isMidiEffect() const override { return false; }
 double getTailLengthSeconds() const override { return 4.0; }
 int getNumPrograms() override { return 1; }
 int getCurrentProgram() override { return 0; }
 void setCurrentProgram(int) override {}
 const juce::String getProgramName(int) override { return "Intimate Room"; }
 void changeProgramName(int,const juce::String&) override {}
 void getStateInformation(juce::MemoryBlock&) override;
 void setStateInformation(const void*,int) override;
 juce::AudioProcessorValueTreeState params;
private:
 juce::Reverb reverb;
 juce::dsp::IIR::Filter<float> lowL, lowR, highL, highR;
 double sr=44100.0;
 bool isBusesLayoutSupported(const BusesLayout& layouts) const override {
  return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet() &&
   (layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono() || layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo());
 }
 JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FeltRoomProcessor)
};
