/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2017 - ROLI Ltd.

   JUCE is an open source library subject to commercial or open-source
   licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#pragma once


//==============================================================================
/**
    Utility class for linearly smoothed values like volume etc. that should
    not change abruptly but as a linear ramp, to avoid audio glitches.
*/

//==============================================================================
template <typename FloatType>
class LinearSmoothedValue
{
public:
    /** Constructor. */
    LinearSmoothedValue() noexcept
        : currentValue (0), target (0), step (0), countdown (0), stepsToTarget (0)
    {
    }

    /** Constructor. */
    LinearSmoothedValue (PARAMETER_TYPE (FloatType) initialValue) noexcept
        : currentValue (initialValue), target (initialValue), step (0), countdown (0), stepsToTarget (0)
    {
    }

    //==============================================================================
    /** Reset to a new sample rate and ramp length. */
    void reset (double sampleRate, double rampLengthInSeconds) noexcept
    {
        jassert (sampleRate > 0 && rampLengthInSeconds >= 0);
        stepsToTarget = (int) std::floor (rampLengthInSeconds * sampleRate);
        currentValue = target;
        countdown = 0;
    }

    /** Set a new target value. */
    void setValue (PARAMETER_TYPE (FloatType) newValue) noexcept
    {
        if (target != newValue)
        {
            target = newValue;
            countdown = stepsToTarget;

            if (countdown <= 0)
                currentValue = target;
            else
                step = (target - currentValue) / (FloatType) countdown;
        }
    }

    /** Compute the next value. */
    FloatType getNextValue() noexcept
    {
        if (countdown <= 0)
            return target;

        --countdown;
        currentValue += step;
        return currentValue;
    }

    /** Returns true if the current value is currently being interpolated. */
    bool isSmoothing() const noexcept
    {
        return countdown > 0;
    }

    /** Returns the target value towards which the smoothed value is currently moving. */
    FloatType getTargetValue() const noexcept
    {
        return target;
    }

private:
    //==============================================================================
    FloatType currentValue, target, step;
    int countdown, stepsToTarget;
};
