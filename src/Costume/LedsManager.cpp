/*
 *  LedsManager.cpp
 *  LED Costume
 *
 *  Created by Imanol Gomez on 09/07/15.
 *
 */


#include "ofMain.h"

#include "LedsManager.h"
#include "AppManager.h"


const string LedsManager::LEDS_LIST_PATH = "leds/";


LedsManager::LedsManager(): Manager()
{
	//Intentionally left empty
}


LedsManager::~LedsManager()
{
    ofLogNotice() <<"LedsManager::Destructor" ;
}


void LedsManager::setup()
{
	if(m_initialized)
		return;


	Manager::setup();
    
    this->setupBoundingBox();
    this->setupLeds();
    
    ofLogNotice() <<"LedsManager::initialized" ;
    
}

void LedsManager::setupBoundingBox()
{
    //m_boundingBox.setX(-1.0);
    //m_boundingBox.setY(-1.0);
    //m_boundingBox.setWidth(604.001 - m_boundingBox.getX());
    //m_boundingBox.setHeight(362.899 - m_boundingBox.getY());

    
    m_boundingBox.setX(166.081);
    m_boundingBox.setY(362.7);
    m_boundingBox.setWidth(401.301 - m_boundingBox.getX());
    m_boundingBox.setHeight(706.770 - m_boundingBox.getY());
}



void LedsManager::setupLeds()
{
    this->readLedsPositions();
    this->normalizeLeds();
}

void LedsManager::readLedsPositions()
{
    
    int numSections = 1;
    int id = 0;
    readLedsPositionFromGroup("A", id, numSections);
    
}

void LedsManager::normalizeLeds()
{
    
    float width = AppManager::getInstance().getCostumeManager().getWidth();
    float height = AppManager::getInstance().getCostumeManager().getHeight();
    
    
    ofLogNotice() <<"LedsManager::normalizeLeds -> width " << width<< ", height = "  << height;
    
    for (auto& ledmap : m_leds)
    {
        auto& ledVector = ledmap.second;
        
        for (auto led: ledVector)
        {
            led->normalize(m_boundingBox);
            //ofLogNotice() <<"LedsManager::normalizeLeds -> id " << led->getId() << ", channel = "  << led->getChannel()  << ", x = "  << led->getPosition().x << ", y = "  << led->getPosition().y << ", z = " << led->getPosition().z ;
        }
    }
    
    
}

void LedsManager::readLedsPositionFromGroup(const string& groupName, int& id, int numberOfSections)
{
    int channel = 0;
    LedVector leds;
    IntVector channelSizes;
    
    for(int i = 1; i <= numberOfSections; i++)
    {
        string led_section_path = LEDS_LIST_PATH + groupName + ofToString(i) + ".txt";
        ofBuffer buffer = ofBufferFromFile(led_section_path);
        
        int channelSize = 0;
        
        if(buffer.size())
        {
            while(buffer.isLastLine() == false)
            {
                string line = buffer.getNextLine();
                ofPoint ledPosition;
                if(parseLedLine(line,ledPosition)){
                    createLed(ledPosition, id, channel, leds);
                    channelSize++;
                }
                
            }
            
            ofLogNotice() <<"LedsManager::readLedsPositionFromGroup -> groupName " << groupName << ", group numLeds = "  << channelSize;
            channelSizes.push_back(channelSize);
            channel++;
        }
        
       
    }
    
    ofLogNotice() <<"LedsManager::readLedsPositionFromGroup -> groupName " << groupName << ", numLeds = "  << leds.size();
    m_leds[groupName] = leds;
    m_ledChannels[groupName] = channelSizes;
}



void LedsManager::createLed(const ofPoint& position, int& id, int channel, LedVector& leds)
{
    ofPtr<Led> led = ofPtr<Led> (new Led ( position, id, channel ) );
    led->setColor(ofColor::black);
    leds.push_back(led);
    
    id++;
    
    //ofLogNotice() <<"LedsManager::createLed -> id " << led->getId() << ", channel = "  << led->getChannel()  << ", x = "  << led->getPosition().x << ", y = "  << led->getPosition().y << ", z = " << led->getPosition().z ;
}

bool LedsManager::parseLedLine(string& line, ofPoint& position)
{
    if(line.size() == 0){
        return false;
    }

    char chars[] = "{}";
    removeCharsFromString(line, chars);
    vector <string> strings = ofSplitString(line, ", " );
    
    position.x = ofToFloat(strings[0]);
    position.y = ofToFloat(strings[1]);
    position.z = ofToFloat(strings[2]);
    
    return true;
}

void LedsManager::removeCharsFromString( string &str, char* charsToRemove ) {
    for ( unsigned int i = 0; i < strlen(charsToRemove); ++i ) {
        str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
    }
}

void LedsManager::update()
{
    //
}

void LedsManager::setPixels(ofPixelsRef pixels)
{
    this->setLedColors(pixels);
    
    AppManager::getInstance().getImageManager().update();
}

void LedsManager::setLedColors(ofPixelsRef pixels)
{
    for (auto& ledmap : m_leds)
    {
        auto& ledVector = ledmap.second;
        
        for (auto led: ledVector)
        {
            led->setPixelColor(pixels);
        }
    }
}


void LedsManager::draw()
{
    this->drawLeds();
}

void LedsManager::draw(int width, int height)
{
    this->drawLeds(width, height);
}

void LedsManager::drawLeds(int width, int height)
{
    for (auto& ledmap : m_leds)
    {
        auto& ledVector = ledmap.second;
        
        for (auto led: ledVector)
        {
            led->draw(width,height);
        }
    }
}



int LedsManager::getNumberLeds(const string& key) const
{
    if (m_leds.find(key) == m_leds.end()) {
        return 0;
    }
    
    return m_leds.at(key).size();
}



