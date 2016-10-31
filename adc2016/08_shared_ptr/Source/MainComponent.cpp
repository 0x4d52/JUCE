#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


class MainContentComponent   : public AudioAppComponent,
                               private Button::Listener,
                               private Thread
{
public:
    class ReferenceCountedBuffer
    {
    public:
        typedef std::shared_ptr<ReferenceCountedBuffer> Ptr;

        ReferenceCountedBuffer (const String& nameToUse,
                                int numChannels,
                                int numSamples)
           : position (0),
             name (nameToUse),
             buffer (numChannels, numSamples)
        {
            DBG (String ("Buffer named '") + name + "' constructed. numChannels = " + String (numChannels) + ", numSamples = " + String (numSamples));
        }

        ~ReferenceCountedBuffer()                 { DBG (String ("Buffer named '") + name + "' destroyed"); }
        AudioSampleBuffer* getAudioSampleBuffer() { return &buffer; }

        int position;

    private:
        String name;
        AudioSampleBuffer buffer;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReferenceCountedBuffer)
    };


    MainContentComponent()  : Thread ("Background Thread")
    {
        addAndMakeVisible (openButton);
        openButton.setButtonText ("Open...");
        openButton.addListener (this);

        addAndMakeVisible (clearButton);
        clearButton.setButtonText ("Clear");
        clearButton.addListener (this);

        setSize (300, 200);

        formatManager.registerBasicFormats();
        setAudioChannels (0, 2);

        startThread();
    }

    ~MainContentComponent()
    {
        stopThread (4000);
        shutdownAudio();
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double /*sampleRate*/) override
    {
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        ReferenceCountedBuffer::Ptr retainedCurrentBuffer (currentBuffer);

        if (retainedCurrentBuffer == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }

        AudioSampleBuffer* const currentAudioSampleBuffer (retainedCurrentBuffer->getAudioSampleBuffer());
        int position = retainedCurrentBuffer->position;

        const int numInputChannels = currentAudioSampleBuffer->getNumChannels();
        const int numOutputChannels = bufferToFill.buffer->getNumChannels();

        int outputSamplesRemaining = bufferToFill.numSamples;
        int outputSamplesOffset = 0;

        while (outputSamplesRemaining > 0)
        {
            int bufferSamplesRemaining = currentAudioSampleBuffer->getNumSamples() - position;
            int samplesThisTime = jmin (outputSamplesRemaining, bufferSamplesRemaining);

            for (int channel = 0; channel < numOutputChannels; ++channel)
            {
                bufferToFill.buffer->copyFrom (channel,
                                               bufferToFill.startSample + outputSamplesOffset,
                                               *currentAudioSampleBuffer,
                                               channel % numInputChannels,
                                               position,
                                               samplesThisTime);
            }

            outputSamplesRemaining -= samplesThisTime;
            outputSamplesOffset    += samplesThisTime;
            position               += samplesThisTime;

            if (position == currentAudioSampleBuffer->getNumSamples())
                position = 0;
        }

        retainedCurrentBuffer->position = position;
    }

    void releaseResources() override
    {
        currentBuffer = nullptr;
    }

    void resized() override
    {
        openButton.setBounds (10, 10, getWidth() - 20, 20);
        clearButton.setBounds (10, 40, getWidth() - 20, 20);
    }

private:
    void buttonClicked (Button* button) override
    {
        if (button == &openButton)   openButtonClicked();
        if (button == &clearButton)  clearButtonClicked();
    }

    void run() override
    {
        while (! threadShouldExit())
        {
            checkForPathToOpen();
            checkForBuffersToFree();
            wait (500);
        }
    }

    void checkForBuffersToFree()
    {
        if (buffers.size() > 0)
        {
            buffers.erase (std::remove_if (buffers.begin(), buffers.end(),
                           [&](ReferenceCountedBuffer::Ptr buf)
                           {
                               return buf.use_count() == 3;
                           }
            ));
        }
    }

    void checkForPathToOpen()
    {
        String pathToOpen;
        swapVariables (pathToOpen, chosenPath);

        if (pathToOpen.isNotEmpty())
        {
            const File file (pathToOpen);
            ScopedPointer<AudioFormatReader> reader (formatManager.createReaderFor (file));

            if (reader != nullptr)
            {
                const double duration = reader->lengthInSamples / reader->sampleRate;

                if (duration < 2)
                {
                    ReferenceCountedBuffer::Ptr newBuffer = std::make_shared<ReferenceCountedBuffer>(file.getFileName(),
                                                                                                     reader->numChannels,
                                                                                                     reader->lengthInSamples);

                    reader->read (newBuffer->getAudioSampleBuffer(), 0, reader->lengthInSamples, 0, true, true);
                    currentBuffer = newBuffer;
                    buffers.push_back (newBuffer);
                }
                else
                {
                    // handle the error that the file is 2 seconds or longer..
                }
            }
        }
    }

    void openButtonClicked()
    {
        FileChooser chooser ("Select a Wave file shorter than 2 seconds to play...",
                             File::nonexistent,
                             "*.wav");

        if (chooser.browseForFileToOpen())
        {
            const File file (chooser.getResult());
            String path (file.getFullPathName());
            swapVariables (chosenPath, path);
            notify();
        }
    }

    void clearButtonClicked()
    {
        currentBuffer = nullptr;
    }

    //==========================================================================
    TextButton openButton;
    TextButton clearButton;

    AudioFormatManager formatManager;
    std::vector<ReferenceCountedBuffer::Ptr> buffers;

    ReferenceCountedBuffer::Ptr currentBuffer;
    String chosenPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
