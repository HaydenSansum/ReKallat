#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    // Constants
    ww = 1400;
    hh = 600;
    framerate = 30;
    ofSetFrameRate(framerate);
    counter = 0;
    
    draw_structure = false;
    draw_explorer = false;
    
    // Parameters - Wind
    wind_speed_large = 24;
    wind_speed_medium = 28;
    wind_speed_small = 30;
    wind_active = false;
    
    // Parameters - Sand
    sand_particle_n = 150000; // 300000 for performance limit
    max_force = 1.0;
    max_velocity = 2.0;
    distance_limit = 30;
    num_s_colors = 8;
    s_alpha = 180;
    generate_sand_colors();
    
    // Parameters - Agent
    explorer_color = ofColor(255,0,0);
    explorer_size = 5;
    explorer = ofVec2f(5, hh/2);
       
    // Canvas
    ofBackground(230);
    
    for (int i = 0; i < sand_particle_n; i++) {
    
        ofVec2f initial_pos = ofVec2f((int) ofRandom(75, ww-75), (int) ofRandom(100, hh-100));
        float random_mass = ofRandom(1,3);
        
        sand fresh_sand;
        fresh_sand.set_size(1);
        fresh_sand.initialize(initial_pos, random_mass, max_force, max_velocity);
        sand_particles.push_back(fresh_sand);
    }
    
    // Specify Hilbert Fractals
    h_paths.push_back(build_hilbert(ofVec2f(75, 100), 400, 3));
    h_paths.push_back(build_hilbert(ofVec2f(500, 100), 400, 4));
    h_paths.push_back(build_hilbert(ofVec2f(933, 108), 415, 5));

    // Get Mazes as points
    vector<ofPolyline> large_maze_path = h_paths[0].getOutline();
    vector<ofPolyline> medium_maze_path = h_paths[1].getOutline();
    vector<ofPolyline> small_maze_path = h_paths[2].getOutline();
    large_maze = large_maze_path[0].getVertices();
    medium_maze = medium_maze_path[0].getVertices();
    small_maze = small_maze_path[0].getVertices();
    
    ofSleepMillis(1500);
}

//--------------------------------------------------------------
void ofApp::update(){

    // Update counter
    counter++;
    
    
    // If Wind isn't active - small small chance for it to spawn
    if (wind_active == false) {
        float trigger = ofRandom(1);
        if (trigger < 0.08) {
            wind_active = true;
            
            maze_type = (int) ofRandom(5);
            
            if(maze_type == 0) {
                current_node = (int) ofRandom(0, (large_maze.size()-8));
                node_steps = (int) ofRandom(2, 8);
                wind_start = large_maze[current_node];
                wind_stop = large_maze[current_node + 1];
                wind_speed = wind_speed_large;
                distance_limit = 58;
            } else if (maze_type < 3) {
                current_node = (int) ofRandom(0, (medium_maze.size()-12));
                node_steps = (int) ofRandom(2, 12);
                wind_start = medium_maze[current_node];
                wind_stop = medium_maze[current_node + 1];
                wind_speed = wind_speed_medium;
                distance_limit = 28;
            } else {
                current_node = (int) ofRandom(0, (small_maze.size()-16));
                node_steps = (int) ofRandom(2, 16);
                wind_start = small_maze[current_node];
                wind_stop = small_maze[current_node + 1];
                wind_speed = wind_speed_small;
                distance_limit = 15;
            }
            node_counter = 0;
            
        }
    }
    
    
    for (int i = 0; i < sand_particle_n; i++) {
        
        // Get mouse position and distance to sand
        ofVec2f cursor_pos = ofVec2f(wind.x, wind.y);
        
        float dist_to_cursor = cursor_pos.distance(sand_particles[i].pos);

        // If close enough then push the sand with some force and noise
        if (dist_to_cursor < (distance_limit / sand_particles[i].mass)) {
 
            ofVec2f force = (sand_particles[i].pos - cursor_pos) / 10;
            sand_particles[i].apply_force(force, 0.5);
            sand_particles[i].move();
            
        }
    }
    
    // Update the wind
    float wind_step = ofMap(counter, 0, (90 / wind_speed), 0, 1, true);
    wind = glm::mix(wind_start, wind_stop, wind_step);

    if (counter % (int) ((90 / wind_speed) + 1) == 0) {
        counter = 0; // Reset counter
        
        // Update to next node
        current_node++;
        node_counter++;
        
        if (node_counter < node_steps) {
            if (maze_type == 0) {
                wind_start = large_maze[current_node];
                wind_stop = large_maze[current_node + 1];
            } else if (maze_type < 3) {
                wind_start = medium_maze[current_node];
                wind_stop = medium_maze[current_node + 1];
            } else {
                wind_start = small_maze[current_node];
                wind_stop = small_maze[current_node + 1];
            }
        } else {
            wind_active = false;
            wind_start = glm::vec3(0.f, 0.f, 0.f);
            wind_stop = glm::vec3(0.f, 0.f, 0.f);
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    // Draw the sand
    ofSeedRandom(13);
    for (int i = 0; i < sand_particle_n; i++) {
        
        int rand_color = (int) ofRandom(num_s_colors);
        
        ofSetColor(sand_colors[rand_color]);
        sand_particles[i].draw();
    }
    ofSeedRandom();
    
    if (draw_explorer) {
        // Draw the explorer
        ofSetColor(explorer_color);
        ofDrawCircle(explorer.x, explorer.y, explorer_size);
        
    }
    if (draw_structure) {
        
        // Build a hilbert pattern
        for (int i = 0; i < h_paths.size(); i++) {
            
            // Specific rules for rotation
            h_paths[i].setStrokeColor(ofColor(166,143,119));
            h_paths[i].setFilled(false);
            h_paths[i].setStrokeWidth(4);
            h_paths[i].draw();
        }
        
        // Draw the wind
        ofSetColor(0,255,255);
        ofDrawCircle(wind, 5);
    }
    
    
    
}


//--------------------------------------------------------------
ofPath ofApp::build_hilbert(ofVec2f start_pos, int width, int order){
    
    n_grid = (int) pow(2, order);
    n_points = n_grid * n_grid;
    ofPath h_path;
    
    float length = width / n_grid;
       
    for (int i = 0; i < n_points; i++) {
        
        ofVec2f new_point = calc_hilbert_pos(i, order);
        
        // Stretch to the right size and offset
        new_point.x = new_point.x * length + length/2;
        new_point.y = new_point.y * length + length/2;
        
        if (i == 0) {
            h_path.moveTo(start_pos + new_point);
        } else {
            h_path.lineTo(start_pos + new_point);
        }
        
    }
    
    return h_path;
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
        
//--------------------------------------------------------------
void ofApp::generate_sand_colors(){
    
    int reds [8] = {161,93,35,47,36,48,141,105};
    int greens [8] = {118,53,19,43,12,37,129,91};
    int blues [8] = {88,24,8,35,4,28,107,73};
    
    for (int i = 0; i < num_s_colors; i ++) {
        sand_colors.push_back(ofColor(reds[i], greens[i], blues[i], s_alpha));
    }
    
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
        explorer.x = ww - 5;
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
