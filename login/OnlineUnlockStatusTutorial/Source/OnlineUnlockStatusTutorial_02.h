/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             OnlineUnlockStatusTutorial
 version:          2.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Showcases online plugin registration.

 dependencies:     juce_core, juce_cryptography, juce_data_structures,
                   juce_events, juce_graphics, juce_gui_basics, juce_gui_extra,
                   juce_product_unlocking
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
class TutorialMarketplaceStatus  : public juce::OnlineUnlockStatus
{
public:
    TutorialMarketplaceStatus() = default;

    juce::String getProductID() override
    {
        return "TestApp";
    }

    bool doesProductIDMatch (const juce::String& returnedIDFromServer) override
    {
        return getProductID() == returnedIDFromServer;
    }

    juce::RSAKey getPublicKey() override
    {
        return juce::RSAKey ("INSERT_PUBLIC_KEY_HERE");///
    }

    void saveState (const juce::String&) override {}
    juce::String getState() override { return {}; }

    juce::String getWebsiteName() override
    {
        return "juce.com";
    }

    juce::URL getServerAuthenticationURL() override
    {
        return juce::URL ("http://localhost:8080/api/users");

    }
        
    juce::String readReplyFromWebserver (const juce::String& email, const juce::String& password) override
    {
        juce::URL url (getServerAuthenticationURL()
                    .withParameter ("product", getProductID())
                    .withParameter ("email", email)
                    .withParameter ("pw", password)
        
                    .withParameter ("os", juce::SystemStats::getOperatingSystemName())
                    .withParameter ("mach", getLocalMachineIDs()[0]));

        DBG ("Trying to unlock via URL: " << url.toString (true));
        
        
        {
            juce::ScopedLock lock (streamCreationLock);
            stream.reset (new juce::WebInputStream (url, true));
        }

        if (stream->connect (nullptr))
        {
            auto* thread = juce::Thread::getCurrentThread();

            if (thread->threadShouldExit() || stream->isError())
                return {};

            auto contentLength = stream->getTotalLength();
            auto downloaded    = 0;

            const size_t bufferSize = 0x8000;
            juce::HeapBlock<char> buffer (bufferSize);

            while (! (stream->isExhausted() || stream->isError() || thread->threadShouldExit()))
            {
                auto max = juce::jmin ((int) bufferSize, contentLength < 0 ? std::numeric_limits<int>::max()
                                                                     : static_cast<int> (contentLength - downloaded));

                auto actualBytesRead = stream->read (buffer.get() + downloaded, max - downloaded);

                if (actualBytesRead < 0 || thread->threadShouldExit() || stream->isError())
                    break;

                downloaded += actualBytesRead;

                if (downloaded == contentLength)
                    break;
            }

            if (thread->threadShouldExit() || stream->isError() || (contentLength > 0 && downloaded < contentLength))
                return {};

            return { juce::CharPointer_UTF8 (buffer.get()) };
        }

        return {};
    }

    void userCancelled() override
    {
        juce::ScopedLock lock (streamCreationLock);

        if (stream != nullptr)
            stream->cancel();
    }

private:
    juce::CriticalSection streamCreationLock;
    std::unique_ptr<juce::WebInputStream> stream;
};

//==============================================================================
//! [TutorialUnlockForm]
class TutorialUnlockForm    : public juce::OnlineUnlockForm
{
public:
    TutorialUnlockForm (TutorialMarketplaceStatus& status)
        : OnlineUnlockForm (status, "Please provide your email and password.")
    {}
//! [TutorialUnlockForm]

//! [TutorialUnlockForm dismiss]
    void dismiss() override
    {
        setVisible (false);
    }
};
//! [TutorialUnlockForm dismiss]

//==============================================================================
//! [MainContentComponent constructor top]
class MainContentComponent   : public juce::Component,
                               private juce::Timer
                                
{
public:
    //==============================================================================
    MainContentComponent()
        : unlockForm (marketplaceStatus)
    {
//! [MainContentComponent constructor top]
        addAndMakeVisible (unlockLabel);
        unlockLabel.setFont (juce::Font (15.0f, juce::Font::bold));
        unlockLabel.setColour (juce::Label::textColourId, juce::Colours::red);

        addAndMakeVisible (unlockButton);
        unlockButton.onClick = [this] { showForm(); };

//        addAndMakeVisible (secretButton);
//        secretButton.setEnabled (false);
//        secretButton.onClick = [this] { checkFeature(); };
        
//! [MainContentComponent constructor bottom]
        addChildComponent (unlockForm);

        setSize (400, 250);
        startTimer (100);
    }
//! [MainContentComponent constructor bottom]

    void paint (juce::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    }

//! [MainContentComponent resized]
    void resized() override
    {
        unlockForm  .centreWithSize (300, 200);
//! [MainContentComponent resized]
        unlockLabel .centreWithSize (100, 40);
        unlockButton.centreWithSize (100, 40);
        //secretButton.centreWithSize (100, 40);

        unlockLabel .setBounds (unlockLabel .getBounds().translated (0, -60));
        unlockButton.setBounds (unlockButton.getBounds().translated (0, -10));
        //secretButton.setBounds (secretButton.getBounds().translated (0,  40));
    }

public:
    //==============================================================================
//! [MainContentComponent timerCallback]
    void timerCallback() override
    {
        if (! isUnlocked && marketplaceStatus.isUnlocked())
        {
            isUnlocked = true;
            unlockApp();
        }
    }
//! [MainContentComponent timerCallback]

//! [MainContentComponent showForm]
    void showForm()
    {
        
        unlockForm.setVisible (true);
    }
//! [MainContentComponent showForm]

//! [MainContentComponent unlockApp]
    void unlockApp()
    {
       // secretButton.setEnabled (true);
        unlockButton.setEnabled (false);

        unlockLabel.setText ("Status: Unlocked", juce::dontSendNotification);
        unlockLabel.setColour (juce::Label::textColourId, juce::Colours::green);
    }
//! [MainContentComponent unlockApp]

//! [MainContentComponent checkFeature]
    void checkFeature()
    {
        if (marketplaceStatus.isUnlocked())
            DBG ("App unlocked!");
        else
            DBG ("Beware of hackers!");
    }
//! [MainContentComponent checkFeature]

    juce::Label      unlockLabel  { {}, "Status: Locked" };
    juce::TextButton unlockButton { "Unlock" };
                    // secretButton { "Super Secret Feature" };

//! [MainContentComponent members]
    TutorialMarketplaceStatus marketplaceStatus;
    TutorialUnlockForm unlockForm;
    //MainContentComponent maincontentComponent;
    bool isUnlocked = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
//! [MainContentComponent members]
