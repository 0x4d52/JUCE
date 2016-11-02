
#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================

static int bench_make_unqiue_ptr (Random& rng)
{
    std::unique_ptr<int> val1 = std::make_unique<int> (rng.nextInt());
    std::unique_ptr<int> val2 = std::make_unique<int> (rng.nextInt());
    
    return (*val1) + (*val2);
};


//==============================================================================

static int bench_new_unqiue_ptr (Random& rng)
{
    std::unique_ptr<int> val1 (new int (rng.nextInt()));
    std::unique_ptr<int> val2 (new int (rng.nextInt()));
    
    return (*val1) + (*val2);
};

//==============================================================================

static int bench_ScopedPointer (Random& rng)
{
    ScopedPointer<int> val1 = new int (rng.nextInt());
    ScopedPointer<int> val2 = new int (rng.nextInt());
    
    return (*val1) + (*val2);
};

//==============================================================================

static int bench_make_shared_ptr (Random& rng)
{
    std::shared_ptr<int> val0 = std::make_shared<int> (rng.nextInt());
    std::shared_ptr<int> val1 = std::make_shared<int> (rng.nextInt());
    std::shared_ptr<int> val2 = std::make_shared<int> (rng.nextInt());
    std::shared_ptr<int> val3 = std::make_shared<int> (rng.nextInt());
    
    std::shared_ptr<int>* const valsA[] = { &val0, &val1, &val2, &val3 };
    const int numValsA = numElementsInArray (valsA);
    
    std::shared_ptr<int> val4 = *valsA[rng.nextInt (numValsA)];
    std::shared_ptr<int> val5 = *valsA[rng.nextInt (numValsA)];
    std::shared_ptr<int> val6 = *valsA[rng.nextInt (numValsA)];
    std::shared_ptr<int> val7 = *valsA[rng.nextInt (numValsA)];
    
    std::shared_ptr<int>* const valsB[] = { &val4, &val5, &val6, &val7 };
    const int numValsB = numElementsInArray (valsB);

    val0 = *valsB[rng.nextInt (numValsB)];
    val1 = *valsB[rng.nextInt (numValsB)];
    val2 = *valsB[rng.nextInt (numValsB)];
    val3 = *valsB[rng.nextInt (numValsB)];
    
    return (*val0) + (*val1) + (*val2) + (*val3);
};

//==============================================================================

static int bench_new_shared_ptr (Random& rng)
{
    std::shared_ptr<int> val0 (new int (rng.nextInt()));
    std::shared_ptr<int> val1 (new int (rng.nextInt()));
    std::shared_ptr<int> val2 (new int (rng.nextInt()));
    std::shared_ptr<int> val3 (new int (rng.nextInt()));
    
    std::shared_ptr<int>* const valsA[] = { &val0, &val1, &val2, &val3 };
    const int numValsA = numElementsInArray (valsA);
    
    std::shared_ptr<int> val4 = *valsA[rng.nextInt (numValsA)];
    std::shared_ptr<int> val5 = *valsA[rng.nextInt (numValsA)];
    std::shared_ptr<int> val6 = *valsA[rng.nextInt (numValsA)];
    std::shared_ptr<int> val7 = *valsA[rng.nextInt (numValsA)];
    
    std::shared_ptr<int>* const valsB[] = { &val4, &val5, &val6, &val7 };
    const int numValsB = numElementsInArray (valsB);
    
    val0 = *valsB[rng.nextInt (numValsB)];
    val1 = *valsB[rng.nextInt (numValsB)];
    val2 = *valsB[rng.nextInt (numValsB)];
    val3 = *valsB[rng.nextInt (numValsB)];
    
    return (*val0) + (*val1) + (*val2) + (*val3);
};

//==============================================================================

class RefCountedInt : public ReferenceCountedObject
{
public:
    typedef ReferenceCountedObjectPtr<RefCountedInt> Ptr;
    
    RefCountedInt (int v) noexcept : value (v) { }
    int operator*() const noexcept { return value; }
    
private:
    int value;
};

static int bench_RefCountedInt (Random& rng)
{
    RefCountedInt::Ptr val0 = new RefCountedInt (rng.nextInt());
    RefCountedInt::Ptr val1 = new RefCountedInt (rng.nextInt());
    RefCountedInt::Ptr val2 = new RefCountedInt (rng.nextInt());
    RefCountedInt::Ptr val3 = new RefCountedInt (rng.nextInt());
    
    RefCountedInt::Ptr* const valsA[] = { &val0, &val1, &val2, &val3 };
    const int numValsA = numElementsInArray (valsA);
    
    RefCountedInt::Ptr val4 = *valsA[rng.nextInt (numValsA)];
    RefCountedInt::Ptr val5 = *valsA[rng.nextInt (numValsA)];
    RefCountedInt::Ptr val6 = *valsA[rng.nextInt (numValsA)];
    RefCountedInt::Ptr val7 = *valsA[rng.nextInt (numValsA)];
    
    RefCountedInt::Ptr* const valsB[] = { &val4, &val5, &val6, &val7 };
    const int numValsB = numElementsInArray (valsB);
    
    val0 = *valsB[rng.nextInt (numValsB)];
    val1 = *valsB[rng.nextInt (numValsB)];
    val2 = *valsB[rng.nextInt (numValsB)];
    val3 = *valsB[rng.nextInt (numValsB)];
    
    return (**val0) + (**val1) + (**val2) + (**val3);
};

//==============================================================================

class InlineRefCountedInt : public ReferenceCountedObject
{
public:
    typedef ReferenceCountedObjectPtr<InlineRefCountedInt> Ptr;
    
    forcedinline InlineRefCountedInt (int v) noexcept : value (v) { }
    forcedinline int operator*() const noexcept { return value; }
    
private:
    int value;
};

static int bench_InlineRefCountedInt (Random& rng)
{
    InlineRefCountedInt::Ptr val0 = new InlineRefCountedInt (rng.nextInt());
    InlineRefCountedInt::Ptr val1 = new InlineRefCountedInt (rng.nextInt());
    InlineRefCountedInt::Ptr val2 = new InlineRefCountedInt (rng.nextInt());
    InlineRefCountedInt::Ptr val3 = new InlineRefCountedInt (rng.nextInt());
    
    InlineRefCountedInt::Ptr* const valsA[] = { &val0, &val1, &val2, &val3 };
    const int numValsA = numElementsInArray (valsA);
    
    InlineRefCountedInt::Ptr val4 = *valsA[rng.nextInt (numValsA)];
    InlineRefCountedInt::Ptr val5 = *valsA[rng.nextInt (numValsA)];
    InlineRefCountedInt::Ptr val6 = *valsA[rng.nextInt (numValsA)];
    InlineRefCountedInt::Ptr val7 = *valsA[rng.nextInt (numValsA)];
    
    InlineRefCountedInt::Ptr* const valsB[] = { &val4, &val5, &val6, &val7 };
    const int numValsB = numElementsInArray (valsB);
    
    val0 = *valsB[rng.nextInt (numValsB)];
    val1 = *valsB[rng.nextInt (numValsB)];
    val2 = *valsB[rng.nextInt (numValsB)];
    val3 = *valsB[rng.nextInt (numValsB)];
    
    return (**val0) + (**val1) + (**val2) + (**val3);
};

//==============================================================================

static int bench_weak_ptr_Component (Random& rng)
{
    std::shared_ptr<Slider> slider1 = std::make_shared<Slider>();
    std::shared_ptr<Slider> slider2 = std::make_shared<Slider>();
    std::shared_ptr<Slider> slider3 = std::make_shared<Slider>();
    std::shared_ptr<Slider> slider4 = std::make_shared<Slider>();
    
    slider1->setValue (rng.nextDouble() * 10.0);
    slider2->setValue (rng.nextDouble() * 10.0);
    slider3->setValue (rng.nextDouble() * 10.0);
    slider4->setValue (rng.nextDouble() * 10.0);
    
    std::weak_ptr<Slider> weak1 (slider1);
    std::weak_ptr<Slider> weak2 (slider2);
    std::weak_ptr<Slider> weak3 (slider3);
    std::weak_ptr<Slider> weak4 (slider4);
    
    if (rng.nextBool()) slider1 = nullptr;
    if (rng.nextBool()) slider2 = nullptr;
    if (rng.nextBool()) slider3 = nullptr;
    if (rng.nextBool()) slider4 = nullptr;
    
    double result = 0.0;
    
    if (std::shared_ptr<Slider> slider = weak1.lock()) result += slider->getValue();
    if (std::shared_ptr<Slider> slider = weak2.lock()) result += slider->getValue();
    if (std::shared_ptr<Slider> slider = weak3.lock()) result += slider->getValue();
    if (std::shared_ptr<Slider> slider = weak4.lock()) result += slider->getValue();

    return (int) result;
}

//==============================================================================

static int bench_WeakReference_Component (Random& rng)
{
    ScopedPointer<Slider> slider1 = new Slider();
    ScopedPointer<Slider> slider2 = new Slider();
    ScopedPointer<Slider> slider3 = new Slider();
    ScopedPointer<Slider> slider4 = new Slider();
    
    slider1->setValue (rng.nextDouble() * 10.0);
    slider2->setValue (rng.nextDouble() * 10.0);
    slider3->setValue (rng.nextDouble() * 10.0);
    slider4->setValue (rng.nextDouble() * 10.0);
    
    WeakReference<Component> weak1 (slider1);
    WeakReference<Component> weak2 (slider2);
    WeakReference<Component> weak3 (slider3);
    WeakReference<Component> weak4 (slider4);
    
    if (rng.nextBool()) slider1 = nullptr;
    if (rng.nextBool()) slider2 = nullptr;
    if (rng.nextBool()) slider3 = nullptr;
    if (rng.nextBool()) slider4 = nullptr;
    
    double result = 0.0;
    
    if (Slider* slider = static_cast<Slider*> (weak1.get())) result += slider->getValue();
    if (Slider* slider = static_cast<Slider*> (weak2.get())) result += slider->getValue();
    if (Slider* slider = static_cast<Slider*> (weak3.get())) result += slider->getValue();
    if (Slider* slider = static_cast<Slider*> (weak4.get())) result += slider->getValue();
    
    return (int) result;
}

//==============================================================================

int main (int argc, char* argv[])
{
    using BenhmarkFunction = std::function<int(Random&)>;
    
    struct FunctionNamePair
    {
        int              index;
        String           name;
        BenhmarkFunction fn;
    };
    
    // build the tests
    using BenhmarkFunctions = std::vector<FunctionNamePair>;
    int index = 0;
    BenhmarkFunctions functions;
    functions.push_back({ index++, "bench_make_unqiue_ptr",             bench_make_unqiue_ptr       });
    functions.push_back({ index++, "bench_new_unqiue_ptr",              bench_new_unqiue_ptr        });
    functions.push_back({ index++, "bench_ScopedPointer",               bench_ScopedPointer         });
    functions.push_back({ index++, "bench_make_shared_ptr",             bench_make_shared_ptr       });
    functions.push_back({ index++, "bench_new_shared_ptr",              bench_new_shared_ptr        });
    functions.push_back({ index++, "bench_RefCountedInt",               bench_RefCountedInt         });
    functions.push_back({ index++, "bench_InlineRefCountedInt",         bench_InlineRefCountedInt   });
    functions.push_back({ index++, "bench_weak_ptr_Component",          bench_InlineRefCountedInt   });
    functions.push_back({ index++, "bench_WeakReference_Component",     bench_InlineRefCountedInt   });
    
    // set up somewhere to store the results
    using BenchmarkTimeMap = HashMap<String, double>;
    BenchmarkTimeMap timingMap;
    
    for (const auto& fn : functions)
        timingMap.set (fn.name, 0.0);
    
    const int numIters = 10000;
    Random& rng = Random::getSystemRandom();

    Logger::writeToLog ("================================================================================");

    // run the tests
    int numItersRemaining = numIters;

    while (numItersRemaining--)
    {
        std::random_shuffle (functions.begin(), functions.end());
    
        for (const auto& fn : functions)
        {
            const double startTime = Time::getMillisecondCounterHiRes();
            const int result = fn.fn (rng);
            const double deltaTime = Time::getMillisecondCounterHiRes() - startTime;
            
            Logger::writeToLog ("Performed: " + fn.name + " result=" + String (result) + " time=" + String (deltaTime, 6) + "ms");

            const double currentTiming = timingMap[fn.name];
            timingMap.set (fn.name, currentTiming + deltaTime);
        }
    }
    
    Logger::writeToLog ("================================================================================");
    
    // collate the results, sorted back by index
    
    std::sort (functions.begin(), functions.end(),
        [](const FunctionNamePair& a, const FunctionNamePair& b) -> bool
        {
            return a.index < b.index;
        }
    );
    
    for (const auto& fn : functions)
    {
        String prefix ("Benchmark: " + fn.name);
        prefix = prefix.paddedRight (' ', 40);
        Logger::writeToLog (prefix + " average=" + String (timingMap[fn.name] / numIters, 6) + "ms / iteration");
    }
    
    Logger::writeToLog ("================================================================================");

}
