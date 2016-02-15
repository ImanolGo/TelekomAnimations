/*
 *  VideoManager.cpp
 *  LED Costume
 *
 *  Created by Imanol Gomez on 20/07/15.
 *
 */


#include "ofMain.h"

#include "VideoManager.h"
#include "AppManager.h"


const string VideoManager::VIDEO_PATH = "videos/";

VideoManager::VideoManager(): Manager(), m_playVideo(false), m_recordVideoLoops(false)
{
    //Intentionally left empty
}


VideoManager::~VideoManager()
{
    ofLogNotice() <<"VideoManager::Destructor" ;
}


void VideoManager::setup()
{
    if(m_initialized)
        return;
    
    
    Manager::setup();
    
    this->loadVideos();
    this->setupBoundingBox();
    this->setupFbo();
    
    ofLogNotice() <<"VideoManager::initialized" ;
    
}


void VideoManager::loadVideos()
{
    ofLogNotice()<< "VideoManager::loadVideos->loading samples from \""<<VIDEO_PATH<<"\"";
    ofDirectory dir(VIDEO_PATH);
    dir.allowExt("mp4");
    dir.allowExt("mov");
    dir.allowExt("avi");
    //populate the directory object
    if(dir.listDir()==0){
        ofLogNotice()<< "VideoManager::loadVideos-> No samples found in \""<<VIDEO_PATH<<"\"";
    }
    
    //go through and print out all the paths
    for(int i = 0; i < dir.numFiles(); i++)
    {
        string videoPath = dir.getPath(i);
        m_videos.push_back(videoPath);
        ofLogNotice()<< "VideoManager::loadVideos-> Loaded: \""<<videoPath<<"\"";
    }
    
    m_currentVideo = m_videos.front();
    m_videoPlayer.loadMovie(m_currentVideo);
    //m_videoPlayer.setLoopState(OF_LOOP_NONE);
    m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
}

void VideoManager::setupBoundingBox()
{
    float ratio = ((float)ofGetWidth())/ofGetHeight();
    
    m_boundingBox.width = AppManager::getInstance().getGuiManager().getWidth();
    m_boundingBox.height = m_boundingBox.width/ratio;
    
    
    m_boundingBox.y = AppManager::getInstance().getGuiManager().getPosition().y + AppManager::getInstance().getGuiManager().getHeight() + 20;
    m_boundingBox.x = AppManager::getInstance().getGuiManager().getPosition().x;
}

void VideoManager::setupFbo()
{
    m_fbo.allocate(m_videoPlayer.getPixelsRef().getWidth(), m_videoPlayer.getPixelsRef().getHeight());
    m_fbo.begin(); ofClear(0); m_fbo.end();
}

void VideoManager::resetPosition()
{
    setupBoundingBox();
}

void VideoManager::update()
{
    if (!m_playVideo) {
        return;
    }
    
    
    if(m_videoPlayer.isFrameNew())
    {
        ofPixels pixels;
        m_fbo.readToPixels(pixels);
        AppManager::getInstance().getLedsManager().setPixels(pixels);
    }
    
    if(m_recordVideoLoops && !m_videoPlayer.isPlaying()){
        m_playVideo = false;
        AppManager::getInstance().getImageManager().onRecordingChange(m_playVideo);
        m_playVideo = true;
        onNextVideoChange();
    }
    
    
    m_videoPlayer.update();
}


void VideoManager::draw()
{
    if (!m_playVideo) {
        return;
    }
    
    //ofEnableAlphaBlending();
    m_fbo.begin();
    ofPushStyle();
    
        ofClear(0);
        ofSetColor(m_color);
        m_videoPlayer.draw(0,0);
    
        
    ofPopStyle();
    m_fbo.end();
    
    m_fbo.draw(m_boundingBox);
    
    //ofDisableAlphaBlending();
    
}

void VideoManager::onPlayVideoChange(bool value)
{
    if(value){
        m_playVideo = true;
        m_videoPlayer.play();
    }
    else{
        m_playVideo = false;
        m_videoPlayer.stop();
    }
}

void VideoManager::onRecordVideoLoopsChange(bool& value) {
    
    m_recordVideoLoops = value;
    
    if(m_recordVideoLoops){
        m_videoPlayer.setLoopState(OF_LOOP_NONE);
    }
    else{
        m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
    }
    
}

void VideoManager::onNextVideoChange()
{
    for (int i = 0; i < m_videos.size(); i++) {
        if(m_videos[i] == m_currentVideo){
            int index = (i+1)%m_videos.size();
            m_currentVideo = m_videos[index];
            break;
        }
    }
    
    
    m_videoPlayer.loadMovie(m_currentVideo);
    
    if(m_playVideo){
        
        m_videoPlayer.setLoopState(OF_LOOP_NORMAL);
        
        if(m_recordVideoLoops){
            m_videoPlayer.setLoopState(OF_LOOP_NONE);
        }
        
        m_videoPlayer.play();
        AppManager::getInstance().getImageManager().onRecordingChange(m_playVideo);
        
    }
    
    this->setupFbo();
}




