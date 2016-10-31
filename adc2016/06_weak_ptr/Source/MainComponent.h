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
            
            if (Component* const originalComp = highlightedComponentHolder->comp.get())
                originalComp->setColour (highlightedComponentHolder->colourId,
                                         highlightedComponentHolder->originalColour);
        }
    }
    
    void highlightComponent (Component* comp, int colourId, Colour highlightColour, int numTicks)
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
        WeakReference<Component> comp;
        int colourId;
        int ticksLeft;
    };

    Holder* findHolderForComponentAndColourId (Component* comp, int colourId)
    {
        for (int i = highlightedComponents.size(); --i >= 0;)
        {
            Holder* const highlightedComponentHolder = highlightedComponents.getUnchecked (i);

            if (highlightedComponentHolder->comp.get() == comp
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
                if (Component* const originalComp = highlightedComponentHolder->comp.get())
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
        highlighter.highlightComponent (slider, Slider::thumbColourId, Colours::red, 50);
    }
    
    void buttonClicked (Button* button) override
    {
        highlighter.highlightComponent (button, TextButton::textColourOffId, Colours::red, 50);
        
        // slightly contrived example but now doesn't break as we used a weak_ptr
        if (button == button2.get())
            button2 = nullptr;
    
    }
    
    //==========================================================================
    std::unique_ptr<Slider>     slider1;
    std::unique_ptr<Slider>     slider2;
    std::unique_ptr<TextButton> button1;
    std::unique_ptr<TextButton> button2;
    
    ComponentColourHighlighter highlighter;
    
    //==========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
