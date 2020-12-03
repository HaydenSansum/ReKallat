#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // Constants
    ww = 1200;
    hh = 600;
    
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
    
    build_hilbert(ofVec2f(ww/2 - 100, hh/2 - 100), 200, 3);

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
//    for (int i = 0; i < sand_particle_n; i++) {
//        sand_particles[i].draw();
//    }
    
    // Draw the explorer
    ofSetColor(explorer_color);
    ofDrawCircle(explorer.x, explorer.y, explorer_size);
    
    
    // Build a hilbert pattern
    h_path.setStrokeColor(ofColor(166,143,119));
    h_path.setFilled(false);
    h_path.setStrokeWidth(4);
    h_path.draw();
    
}


//--------------------------------------------------------------
void ofApp::build_hilbert(ofVec2f start_pos, int width, int order){
    
    n_grid = (int) pow(2, order);
    n_points = n_grid * n_grid;
   
    float length = width / n_grid;
       
    for (int i = 0; i < n_points; i++) {
        
        ofVec2f new_point = calc_hilbert_pos(i, order);
        cout << new_point << endl;
        
        // Stretch to the right size and offset
        new_point.x = new_point.x * length + length/2;
        new_point.y = new_point.y * length + length/2;
        
        if (i == 0) {
            h_path.moveTo(start_pos + new_point);
        } else {
            h_path.lineTo(start_pos + new_point);
        }
        
    }
    
}


//--------------------------------------------------------------
ofVec2f ofApp::calc_hilbert_pos(int i, int order){
    
    // Create a vector of cells
    vector<ofVec2f> cells;
    cells.push_back(ofVec2f(0,0));
    cells.push_back(ofVec2f(0,1));
    cells.push_back(ofVec2f(1,1));
    cells.push_back(ofVec2f(1,0));
    
    int cell_index = i & 3;
    ofVec2f point = cells[cell_index];
    
    for (int j = 1; j < order; j++) {
        
        // Shift i over two and recalculatge sub index
        i = (i >> 2);
        int cell_sub_index = i & 3;
        
        // Calculate length based on order
        float length = pow(2, j);
        
        // If top right subcell rotate and mirror (flip x and y)
        if (cell_sub_index == 0) {
            float temp_x = point.x;
            point.x = point.y;
            point.y = temp_x;
        } else if (cell_sub_index == 1) {
            point.y = point.y + length;
        } else if (cell_sub_index == 2) {
            point.x = point.x + length;
            point.y = point.y + length;
        } else {
            float temp_x = length - 1 - point.x;
            point.x = length - 1 - point.y;
            point.y = temp_x;
            point.x = point.x + length;
        }
    }
    
    return point;
    
}
        
//
//        if (pos == 0) {
//            if (begin == 1) {
//                h_path.moveTo(center + ofVec2f(-size/2, -size/2));
//                begin = 0;
//            } else {
//                if (pos2 == 3) {
//                    h_path.lineTo(center + ofVec2f(+size/2, +size/2));
//                } else {
//                    h_path.lineTo(center + ofVec2f(-size/2, -size/2));
//                }
//            }
//        } else if (pos == 1) {
//            if (pos2 == 0) {
//                h_path.lineTo(center + ofVec2f(+size/2, -size/2));
//            } else {
//                h_path.lineTo(center + ofVec2f(-size/2, +size/2));
//            }
//        } else if (pos == 2) {
//            if (pos2 == 3) {
//                h_path.lineTo(center + ofVec2f(-size/2, -size/2));
//            } else {
//                h_path.lineTo(center + ofVec2f(+size/2, +size/2));
//            }
//        } else {
//            if (pos2 == 0) {
//                h_path.lineTo(center + ofVec2f(-size/2, +size/2));
//            } else {
//                h_path.lineTo(center + ofVec2f(+size/2, -size/2));
//            }
//        }
        


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
