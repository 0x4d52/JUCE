#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


class ComponentCpp11 : public Component
{
public:
    template<typename ComponentType>
    std::unique_ptr<ComponentType>&& addAndMakeVisible (std::unique_ptr<ComponentType>&& comp, int zOrder = -1)
    {
        Component::addAndMakeVisible (comp.get(), zOrder);
        return std::move (comp);
    }
};

class MainContentComponent   : public ComponentCpp11
{
public:
    //==============================================================================
    MainContentComponent()
    {
        int numSliders = 4;
        
        while (numSliders--)
            sliders.emplace_back (addAndMakeVisible (std::make_unique<Slider>()));

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
        
        for (const auto& slider : sliders)
            slider->setBounds (area.removeFromTop (30).reduced (2));
    }

private:
    std::vector<std::unique_ptr<Slider>> sliders;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
