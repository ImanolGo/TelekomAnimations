/*
 *  LedsManager.h
 *  LED Costume
 *
 *  Created by Imanol Gomez on 09/07/15.
 *
 */


#pragma once

#include "Manager.h"
#include "Led.h"


//========================== class LedsManager ==============================
//============================================================================
/** \class LedsManager LedsManager.h
 *	\brief Class managing the LEDs
 *	\details It controls the postion and color of the LEDs
 */


class LedsManager: public Manager
{

    static const string LEDS_LIST_PATH;
    
    public:
    
        typedef vector<int> IntVector;
        typedef vector< ofPtr<Led> > LedVector;
        typedef map< string, LedVector> LedMap;
        typedef map< string, IntVector>   ChannelMap;
    
    public:

        //! Constructor
        LedsManager();

        //! Destructor
        ~LedsManager();

        //! Setup the Halo Manager
        void setup();

        //! Update the Led Manager
        void update();
    
        //! Draw the Led Manager
        void draw();
    
        //! Draw the Led Manager according to a certain width or height
        void draw(int width, int height);
    
        const LedMap& getLeds() const {return m_leds;}
    
        const ChannelMap& getLedChannels() const {return m_ledChannels;}
    
        int getNumberLeds(const string& key) const;
    
        void setPixels(ofPixelsRef pixels);
    
        void setLedColors(ofPixelsRef pixels);
    
    
    private:
    
        void setupBoundingBox();
    
        void setupLeds();
    
        void readLedsPositions();
    
        void normalizeLeds();
    
        void readLedsPositionFromGroup(const string& groupName, int& id, int numberOfSections);
    
        bool parseLedLine(string& line, ofPoint& position);
    
        void removeCharsFromString( string &str, char* charsToRemove );
    
        void createLed(const ofPoint& position, int& id, int channel, LedVector& leds);
    
        void drawLeds(int width = 1, int height = 1);
    
    private:
    
    
    
        LedMap          m_leds;
        ChannelMap      m_ledChannels;
        ofRectangle     m_boundingBox;
    
};



