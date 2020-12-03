#pragma once

#include "ofMain.h"
#include "sand.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        ofVec2f hilbert_step(ofVec2f start_pos, int size, int direction);
  
        ofPath build_hilbert(ofVec2f start_pos, int width, int order);
        ofVec2f calc_hilbert_pos(int i, int order);
    
        void move_left();
        void move_right();
        void move_up();
        void move_down();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // Particles
    vector<sand> sand_particles;
    int sand_particle_n;
    int max_force;
    int max_velocity;
    float distance_limit;
    
    // Explorer
    ofVec2f explorer;
    ofColor explorer_color;
    int explorer_size;
    
    // Constants
    int ww;
    int hh;
    int framerate;
    int counter;
    
    // Hilbert
    int n_grid;
    int n_points;
    vector<ofPath> h_paths;
    vector<glm::vec3> large_maze;
    vector<glm::vec3> medium_maze;
    vector<glm::vec3> small_maze;
    
    // Wind
    glm::vec3 wind;
    glm::vec3 wind_stop;
    glm::vec3 wind_start;
    float wind_speed;
    
    int current_node;
    int node_steps;
    
    // Imgs
    ofImage save_img;
		
};
