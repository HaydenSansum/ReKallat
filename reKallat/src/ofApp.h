#pragma once

#include "ofMain.h"
#include "sand.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        ofVec2f hilbert_step(ofVec2f start_pos, int size, int direction);
  
        ofVec2f build_hilbert_left(ofVec2f start_pos, int size);
        ofVec2f build_hilbert_right(ofVec2f start_pos, int size);
        ofVec2f build_hilbert_up(ofVec2f start_pos, int size);
        ofVec2f build_hilbert_down(ofVec2f start_pos, int size);
    
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
    
    // Imgs
    ofImage save_img;
		
};
