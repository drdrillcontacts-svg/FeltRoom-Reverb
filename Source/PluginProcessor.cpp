#include "PluginProcessor.h"
#include "PluginEditor.h"
static juce::AudioProcessorValueTreeState::ParameterLayout makeParams(){
 juce::AudioProcessorValueTreeState::ParameterLayout p;
 auto add=[&](const char* id,float a,float b,float d){p.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{id,1},id,juce::NormalisableRange<float>(a,b),d));};
 add("PREDELAY",0,200,20); add("DECAY",0.1f,10,3.5f); add("SIZE",0,100,70); add("DAMPING",0,100,45);
 add("LOWCUT",20,1000,100); add("HIGHCUT",1000,20000,12000); add("MIX",0,100,35);
 return p;
}
FeltRoomProcessor::FeltRoomProcessor():AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo(),true).withOutput("Output",juce::AudioChannelSet::stereo(),true)),params(*this,nullptr,"PARAMETERS",makeParams()){}
void FeltRoomProcessor::prepareToPlay(double s,int){sr=s;reverb.setSampleRate(s);reverb.reset();}
void FeltRoomProcessor::processBlock(juce::AudioBuffer<float>& b,juce::MidiBuffer&){
 juce::ScopedNoDenormals noDenormals;
 const int n=b.getNumSamples(), channels=b.getNumChannels(); if(channels<1)return;
 juce::AudioBuffer<float> wet(channels,n);for(int c=0;c<channels;++c)wet.copyFrom(c,0,b,c,0,n);
 auto val=[this](const char* id){return params.getRawParameterValue(id)->load();};
 juce::Reverb::Parameters rp;
 rp.roomSize=juce::jlimit(0.f,1.f,val("SIZE")/100.f);
 rp.damping=val("DAMPING")/100.f;
 rp.wetLevel=1.f;rp.dryLevel=0.f;rp.width=1.f;rp.freezeMode=0.f;
 // JUCE Reverb does not expose independent decay seconds or predelay.
 // DECAY is mapped approximately to feedback room size; PREDELAY is reserved for a future DSP implementation.
 rp.roomSize=juce::jlimit(0.f,1.f,0.35f+0.055f*val("DECAY")+0.12f*val("SIZE")/100.f);
 reverb.setParameters(rp);
 if(channels>=2)reverb.processStereo(wet.getWritePointer(0),wet.getWritePointer(1),n);
 else reverb.processMono(wet.getWritePointer(0),n);
 float mix=val("MIX")/100.f;
 for(int c=0;c<channels;++c){auto* dry=b.getWritePointer(c);auto* w=wet.getReadPointer(c);for(int i=0;i<n;++i)dry[i]=dry[i]*(1.f-mix)+w[i]*mix;}
}
void FeltRoomProcessor::getStateInformation(juce::MemoryBlock& m){auto x=params.copyState().createXml();copyXmlToBinary(*x,m);}
void FeltRoomProcessor::setStateInformation(const void* d,int n){auto x=getXmlFromBinary(d,n);if(x&&x->hasTagName(params.state.getType()))params.replaceState(juce::ValueTree::fromXml(*x));}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){return new FeltRoomProcessor();}
juce::AudioProcessorEditor* FeltRoomProcessor::createEditor(){return new FeltRoomEditor(*this);}
