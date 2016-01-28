//
//  Led.cpp
//  Led Costume
//
//  Created by Imanol Gomez on 09/07/15.
//
//--------------------------------------------------------------

#include "Led.h"


const int Led::SIZE = 15;

Led::Led(const ofPoint& position, int id, int channel): BasicVisual(position, SIZE, SIZE), m_id(id), m_channel(channel)
{
    //Intentionaly left empty
}

Led::~Led()
{
    //Intentionaly left empty
}

void Led::draw()
{
    ofPushMatrix();
    ofPushStyle();
    
    ofTranslate(m_position);
    ofScale(m_scale.x, m_scale.y);
    ofTranslate(-m_width*0.5,-m_height*0.5);
    
    ofRotateX(m_rotation.x);
    ofRotateY(m_rotation.y);
    
    ofSetColor(m_color);
    ofSetCircleResolution(20);
    ofFill();
    
        ofRect(0, 0, m_width, m_height);
        //ofCircle(0, 0, m_width);

    ofPopStyle();
    ofPopMatrix();
}

void Led::draw(int width, int height)
{
    
    ofPushMatrix();
    ofTranslate(m_position.x * width, m_position.y * height);
        this->draw();
    ofPopMatrix();
}

void Led::normalize(const ofRectangle& boundingBox)
{
    m_position.x = (m_position.x - boundingBox.getX()) / boundingBox.getWidth();
    
    ///HACK TO PARSE TO THE NEW BUTTERFLY IMAGE!!!! CHANGE WHEN NEW IMAGES
    m_position.x  = ofMap(m_position.x, 0.0, 1.0, 0.466,0.864);

    m_position.y = (m_position.y - boundingBox.getY()) / boundingBox.getHeight();
    
    ///HACK TO PARSE TO THE NEW BUTTERFLY IMAGE!!!! CHANGE WHEN NEW IMAGES
    m_position.y  = ofMap(m_position.y, 0.0, 1.0, 0.145,0.879);
    
    m_position.y = 1 - m_position.y;
}


void Led::setPixelColor(ofPixelsRef pixels)
{
    m_color = pixels.getColor(m_position.x * pixels.getWidth(), m_position.y * pixels.getHeight());
}

