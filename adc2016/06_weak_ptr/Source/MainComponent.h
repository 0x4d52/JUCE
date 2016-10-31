#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../common/ComponentCpp11.h"

class ComponentColourHighlighter   : private Timer
{
public:
    ComponentColourHighlighter()
    {
        startTimer (10);
    }
    
    ~ComponentColourHighlighter()
    {
        for (int i = highlightedComponents.size(); --i >= 0;)
        {
            Holder* const highlightedComponentHolder = highlightedComponents.getUnchecked (i);
            
            if (std::shared_ptr<Component> originalComp = highlightedComponentHolder->comp.lock())
                originalComp->setColour (highlightedComponentHolder->colourId,
                                         highlightedComponentHolder->originalColour);
        }
    }
    
    void highlightComponent (const std::shared_ptr<Component>& comp, int colourId, Colour highlightColour, int numTicks)
    {
        Holder* highlightedComponentHolder = findHolderForComponentAndColourId (comp, colourId);
        
        if (highlightedComponentHolder == nullptr)
        {
            highlightedComponentHolder = new Holder();
            highlightedComponentHolder->originalColour = comp->findColour (colourId);
            highlightedComponentHolder->comp           = comp;
            highlightedComponentHolder->colourId       = colourId;
            highlightedComponents.add (highlightedComponentHolder);
        }
        
        highlightedComponentHolder->ticksLeft = jmax (1, numTicks);
        
        comp->setColour (colourId, highlightColour);
    }
    
private:
    struct Holder
    {
        Colour originalColour;
        std::weak_ptr<Component> comp;
        int colourId;
        int ticksLeft;
    };

    Holder* findHolderForComponentAndColourId (const std::shared_ptr<Component>& comp, int colourId)
    {
        for (int i = highlightedComponents.size(); --i >= 0;)
        {
            Holder* const highlightedComponentHolder = highlightedComponents.getUnchecked (i);

            if (highlightedComponentHolder->comp.lock() == comp
                &&  highlightedComponentHolder->colourId == colourId)
                return highlightedComponentHolder;
        }
        
        return nullptr;
    }
    
    void timerCallback() override
    {
        for (int i = highlightedComponents.size(); --i >= 0;)
        {
            Holder* const highlightedComponentHolder = highlightedComponents.getUnchecked (i);
            
            if (--highlightedComponentHolder->ticksLeft == 0)
            {
                if (std::shared_ptr<Component> originalComp = highlightedComponentHolder->comp.lock())
                    originalComp->setColour (highlightedComponentHolder->colourId,
                                             highlightedComponentHolder->originalColour);
                    
                highlightedComponents.remove (i);
            }
        }
    }
    
    //==========================================================================
    
    OwnedArray<Holder> highlightedComponents;
};

//==============================================================================

class MainContentComponent   : public ComponentCpp11,
                               private Slider::Listener,
                               private Button::Listener
{
public:
    //==========================================================================
    MainContentComponent()
    {
        slider1 = addAndMakeVisible (std::make_unique<Slider>());
        slider2 = addAndMakeVisible (std::make_unique<Slider>());
        button1 = addAndMakeVisible (std::make_unique<TextButton> ("Button 1"));
        button2 = addAndMakeVisible (std::make_unique<TextButton> ("Button 2"));
        
        slider1->addListener (this);
        slider2->addListener (this);
        button1->addListener (this);
        button2->addListener (this);
        
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
    void sliderValueChanged (Slider* slider) override
    {
        if (slider == slider1.get())
            highlighter.highlightComponent (slider1, Slider::thumbColourId, Colours::red, 50);
        else if (slider == slider2.get())
            highlighter.highlightComponent (slider2, Slider::thumbColourId, Colours::green, 50);
    }
    
    void buttonClicked (Button* button) override
    {
        if (button == button1.get())
        {
            highlighter.highlightComponent (button1, TextButton::textColourOffId, Colours::red, 50);
        }
        else if (button == button2.get())
        {
            highlighter.highlightComponent (button2, TextButton::textColourOffId, Colours::green, 50);

            if (button == button2.get())
                button2 = nullptr;
        }
    
    }
    
    //==========================================================================
    std::shared_ptr<Slider>     slider1;
    std::shared_ptr<Slider>     slider2;
    std::shared_ptr<TextButton> button1;
    std::shared_ptr<TextButton> button2;
    
    ComponentColourHighlighter highlighter;
    
    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
