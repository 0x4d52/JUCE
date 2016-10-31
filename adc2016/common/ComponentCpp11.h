#ifndef COMPONENTCPP11_H_INCLUDED
#define COMPONENTCPP11_H_INCLUDED

class ComponentCpp11 : public Component
{
public:
    /** Convenience for adding a child component, making visible and returning a std::unique_ptr holding a component type. */
    template<typename ComponentType>
    std::unique_ptr<ComponentType> addAndMakeVisible (std::unique_ptr<ComponentType>&& comp, int zOrder = -1)
    {
        Component::addAndMakeVisible (comp.get(), zOrder);
        return std::move (comp);
    }
    
    /** Convenience for adding a child component, making visible and returning a std::unique_ptr holding a component type. */
    template<typename ComponentType>
    std::shared_ptr<ComponentType> addAndMakeVisible (std::shared_ptr<ComponentType>&& comp, int zOrder = -1)
    {
        Component::addAndMakeVisible (comp.get(), zOrder);
        return std::move (comp);
    }
};

#endif  // COMPONENTCPP11_H_INCLUDED
