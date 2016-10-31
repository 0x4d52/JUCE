#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MainContentComponent   : public Component
{
public:
    //==============================================================================
    MainContentComponent()
    {
        int numSliders = 4;
        
        while (numSliders--)
            addAndMakeVisible (sliders.add (new Slider()));

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
        
        for (int i = 0; i < sliders.size(); ++i)
            sliders.getUnchecked (i)->setBounds (area.removeFromTop (30).reduced (2));
    }

private:
    OwnedArray<Slider> sliders;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
