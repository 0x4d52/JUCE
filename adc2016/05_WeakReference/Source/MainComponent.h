#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent   : public Component
{
public:
    //==============================================================================
    MainContentComponent()
    {
        addAndMakeVisible (slider1 = new Slider());
        addAndMakeVisible (slider2 = new Slider());
        addAndMakeVisible (button1 = new TextButton ("Button 1"));
        addAndMakeVisible (button2 = new TextButton ("Button 2"));
        
        setSize (400, 300);
    }
    
    ~MainContentComponent()
    {
    }

    void paint (Graphics& g) override
    {
        g.setColour (Colours::wheat);
        g.fillRect (getLocalBounds());
    }
    
    void resized() override
    {
        Rectangle<int> area (getLocalBounds().reduced (8));
        
        slider1->setBounds (area.removeFromTop (30).reduced (2));
        slider2->setBounds (area.removeFromTop (30).reduced (2));
        button1->setBounds (area.removeFromTop (30).reduced (2));
        button2->setBounds (area.removeFromTop (30).reduced (2));
    }

private:
    ScopedPointer<Slider>     slider1;
    ScopedPointer<Slider>     slider2;
    ScopedPointer<TextButton> button1;
    ScopedPointer<TextButton> button2;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
