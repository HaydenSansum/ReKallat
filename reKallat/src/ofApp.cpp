#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // Constants
    ww = 800;
    hh = 800;
    
    // Parameters - Sand
    sand_particle_n = 25000;
    max_force = 1.0;
    max_velocity = 3.0;
    distance_limit = 30;
    
    // Parameters - Agent
    explorer_color = ofColor(255,0,0);
    explorer_size = 5;
    explorer = ofVec2f(5, hh/2);
       
    // Canvas
    ofBackground(230);
    
    
    for (int i = 0; i < sand_particle_n; i++) {
    
        ofVec2f initial_pos = ofVec2f((int) ofRandom(ww/2 - 100, ww/2 + 100), (int) ofRandom(hh/2 - 100, hh/2 + 100));
        float random_mass = ofRandom(1,3);
        
        sand fresh_sand;
        fresh_sand.set_size(1);
        fresh_sand.initialize(initial_pos, random_mass, max_force, max_velocity);
        sand_particles.push_back(fresh_sand);
    }
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

    for (int i = 0; i < sand_particle_n; i++) {
        
        // Get mouse position and distance to sand
        ofVec2f cursor_pos = ofVec2f(explorer.x, explorer.y);
        
        float dist_to_cursor = cursor_pos.distance(sand_particles[i].pos);

        // If close enough then push the sand with some force and noise
        if (dist_to_cursor < (distance_limit / sand_particles[i].mass)) {
 
            ofVec2f force = (sand_particles[i].pos - cursor_pos) / 10;
            sand_particles[i].apply_force(force, 0.5);
            sand_particles[i].move();
            
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetColor(30,5,1,180);
    
    // Draw the sand
    for (int i = 0; i < sand_particle_n; i++) {
        sand_particles[i].draw();
    }
    
    // Draw the explorer
    ofSetColor(explorer_color);
    ofDrawCircle(explorer.x, explorer.y, explorer_size);
}

//--------------------------------------------------------------
void ofApp::move_left(){
    explorer.x = explorer.x - 5;
    if (explorer.x < 5) {
        explorer.x = 5;
    }
}

//--------------------------------------------------------------
void ofApp::move_right(){
    explorer.x = explorer.x + 5;
    if (explorer.x > ww - 5) {
        explorer.x = ww = 5;
    }
}

//--------------------------------------------------------------
void ofApp::move_up(){
    explorer.y = explorer.y - 5;
    if (explorer.y < 5) {
        explorer.y = 5;
    }
}

//--------------------------------------------------------------
void ofApp::move_down(){
    explorer.y = explorer.y + 5;
    if (explorer.y > hh - 5) {
        explorer.y = hh - 5;
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == OF_KEY_LEFT) {
        move_left();
    } else if (key == OF_KEY_RIGHT) {
        move_right();
    } else if (key == OF_KEY_UP) {
        move_up();
    } else if (key == OF_KEY_DOWN) {
        move_down();
    }
    
    else if (key == ' ') {
        save_img.grabScreen(0,0, ofGetWidth(), ofGetHeight());
        save_img.save(ofGetTimestampString() + ".png");
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
