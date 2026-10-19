#include "PluginEditor.h"
#include "BinaryData.h"
void FeltRoomEditor::CopperLook::drawRotarySlider(juce::Graphics& g,int x,int y,int w,int h,float pos,float start,float end,juce::Slider&){
 auto r=juce::Rectangle<float>((float)x,(float)y,(float)w,(float)h).reduced(8);auto center=r.getCentre();float rad=juce::jmin(r.getWidth(),r.getHeight())*0.42f;
 g.setColour(juce::Colour(0xff181716));g.fillEllipse(center.x-rad,center.y-rad,rad*2,rad*2);
 juce::Path track;track.addCentredArc(center.x,center.y,rad,rad,0,start,end,true);g.setColour(juce::Colour(0xff50453b));g.strokePath(track,juce::PathStrokeType(3));
 juce::Path active;active.addCentredArc(center.x,center.y,rad,rad,0,start,start+(end-start)*pos,true);g.setColour(juce::Colour(0xffd6a979));g.strokePath(active,juce::PathStrokeType(3));
 auto a=start+(end-start)*pos;juce::Point<float> p(center.x+std::sin(a)*rad*0.67f,center.y-std::cos(a)*rad*0.67f);g.setColour(juce::Colours::ivory);g.drawLine(center.x,center.y,p.x,p.y,2);
}
FeltRoomEditor::FeltRoomEditor(FeltRoomProcessor& p):AudioProcessorEditor(p),proc(p){
 piano=juce::ImageFileFormat::loadFrom(BinaryData::piano_jpg,BinaryData::piano_jpgSize);
 const char* names[]={"PREDELAY","DECAY","SIZE","DAMPING","LOWCUT","HIGHCUT","MIX"};
 for(int i=0;i<7;++i){knobs[i]=std::make_unique<juce::Slider>();auto& k=*knobs[i];k.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);k.setTextBoxStyle(juce::Slider::TextBoxBelow,false,110,23);k.setColour(juce::Slider::textBoxTextColourId,juce::Colour(0xffd8c6b4));k.setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::transparentBlack);k.setLookAndFeel(&look);addAndMakeVisible(k);attachments[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(proc.params,names[i],k);}
 presets.addItem("Intimate Room",1);presets.addItem("Warm Room",2);presets.addItem("Cinematic Hall",3);presets.setSelectedId(1,juce::dontSendNotification);presets.onChange=[this]{const float vals[3][7]={{20,3.5f,70,45,100,12000,35},{10,2.2f,50,65,160,9500,25},{45,7.0f,95,25,80,15000,55}};int idx=juce::jlimit(0,2,presets.getSelectedId()-1);const char* ids[]={"PREDELAY","DECAY","SIZE","DAMPING","LOWCUT","HIGHCUT","MIX"};for(int i=0;i<7;++i)if(auto* q=proc.params.getParameter(ids[i]))q->setValueNotifyingHost(q->convertTo0to1(vals[idx][i]));};addAndMakeVisible(presets);
 setSize(1200,800);
}
void FeltRoomEditor::paint(juce::Graphics& g){auto area=getLocalBounds();g.fillAll(juce::Colour(0xff090a0b));
 auto imageArea=area.removeFromTop(625);if(piano.isValid())g.drawImage(piano,imageArea.toFloat(),juce::RectanglePlacement::fillDestination);
 juce::ColourGradient shade(juce::Colours::black.withAlpha(0.66f),0,0,juce::Colours::black.withAlpha(0.12f),0,570,false);g.setGradientFill(shade);g.fillRect(imageArea);
 g.setColour(juce::Colour(0xfff4e9de));g.setFont(juce::Font(juce::FontOptions(44)));g.drawFittedText("F E L T R O O M",0,42,getWidth(),65,juce::Justification::centred,1);
 g.setFont(juce::Font(juce::FontOptions(13)));g.setColour(juce::Colour(0xffd2bda8));g.drawFittedText("F E L T   R E V E R B",0,108,getWidth(),28,juce::Justification::centred,1);
 g.setFont(juce::Font(juce::FontOptions(17)));g.drawText("D.A",35,33,100,40,juce::Justification::left);g.setFont(juce::Font(juce::FontOptions(10)));g.drawText("INSTRUMENTS",35,70,100,16,juce::Justification::left);
 g.setColour(juce::Colour(0xe6090909));g.fillRect(0,540,getWidth(),260);
 const char* labels[]={"PRE-DELAY","DECAY","SIZE","DAMPING","LOW CUT","HIGH CUT","MIX"};
 g.setFont(juce::Font(juce::FontOptions(12)));g.setColour(juce::Colour(0xffdfc8ae));for(int i=0;i<7;++i){auto cx=60+i*155+60;g.drawFittedText(labels[i],cx-73,680,146,22,juce::Justification::centred,1);}
 g.setColour(juce::Colour(0xff665548));g.drawHorizontalLine(733,40.f,1160.f);
 g.setColour(juce::Colour(0xffa79583));g.drawText("PRESETS",40,748,130,26,juce::Justification::left);
 g.setColour(juce::Colour(0xffc49a70));g.drawText("●  REVERB",970,748,200,26,juce::Justification::right);
 g.setColour(juce::Colour(0xff685646));g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(8),12,1);
}
void FeltRoomEditor::resized(){for(int i=0;i<7;++i)knobs[i]->setBounds(60+i*155,565,120,110);presets.setBounds(400,745,400,32);}
