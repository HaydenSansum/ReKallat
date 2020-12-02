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
    
    ofSetColor(20,15,9,120);
    
    // Draw the sand
    for (int i = 0; i < sand_particle_n; i++) {
        sand_particles[i].draw();
    }
    
    // Draw the explorer
    ofSetColor(explorer_color);
    ofDrawCircle(explorer.x, explorer.y, explorer_size);
    
    
    // Build a hilbert pattern
    ofSetColor(166,143,119);
    ofSetLineWidth(4);
    build_hilbert_left(ofVec2f(ww/2 - 5, 100), 70);
    
}

//--------------------------------------------------------------
ofVec2f ofApp::hilbert_step(ofVec2f start_pos, int size, int direction){
    
    // 0 = left, 1 = right, 2 = up, 3 = down
    ofVec2f movement;
    
    if (direction == 0) {
        movement = ofVec2f(-size, 0);
    } else if (direction == 1) {
        movement = ofVec2f(size, 0);
    } else if (direction == 2) {
        movement = ofVec2f(0, -size);
    } else if (direction == 3) {
        movement = ofVec2f(0, size);
    } else {
        movement = ofVec2f(0, 0);
    }
    
    ofVec2f new_position = start_pos + movement;
    
    return new_position;
    
}


//--------------------------------------------------------------
ofVec2f ofApp::build_hilbert_left(ofVec2f start_pos, int size){
    
    // Take all the required steps
    ofVec2f position1 = hilbert_step(start_pos, size, 3); // down
    ofVec2f position2 = hilbert_step(position1, size, 0); // left
    ofVec2f position3 = hilbert_step(position2, size, 2); // up
    ofVec2f position4 = hilbert_step(position3, size, 0); // left
    ofVec2f position5 = hilbert_step(position4, size, 0); // left
    ofVec2f position6 = hilbert_step(position5, size, 3); // down
    ofVec2f position7 = hilbert_step(position6, size, 1); // right
    ofVec2f position8 = hilbert_step(position7, size, 3); // down
    ofVec2f position9 = hilbert_step(position8, size, 0); // left
    ofVec2f position10 = hilbert_step(position9, size, 3); // down
    ofVec2f position11 = hilbert_step(position10, size, 1); // right
    ofVec2f position12 = hilbert_step(position11, size, 1); // right
    ofVec2f position13 = hilbert_step(position12, size, 2); // up
    ofVec2f position14 = hilbert_step(position13, size, 1); // right
    ofVec2f position15 = hilbert_step(position14, size, 3); // down

    // Draw all the lines
    ofDrawLine(start_pos, position1);
    ofDrawLine(position1, position2);
    ofDrawLine(position2, position3);
    ofDrawLine(position3, position4);
    ofDrawLine(position4, position5);
    ofDrawLine(position5, position6);
    ofDrawLine(position6, position7);
    ofDrawLine(position7, position8);
    ofDrawLine(position8, position9);
    ofDrawLine(position9, position10);
    ofDrawLine(position10, position11);
    ofDrawLine(position11, position12);
    ofDrawLine(position12, position13);
    ofDrawLine(position13, position14);
    ofDrawLine(position14, position15);
    
    return position15;
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
