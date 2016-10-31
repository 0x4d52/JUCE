#ifndef COMPONENTCPP11_H_INCLUDED
#define COMPONENTCPP11_H_INCLUDED

class ComponentCpp11 : public Component
{
public:
    template<typename ComponentType>
    std::unique_ptr<ComponentType> addAndMakeVisible (std::unique_ptr<ComponentType>&& comp, int zOrder = -1)
    {
        Component::addAndMakeVisible (comp.get(), zOrder);
        return std::move (comp);
    }
};

#endif  // COMPONENTCPP11_H_INCLUDED
