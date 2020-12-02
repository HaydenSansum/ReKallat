//
//  sand.hpp
//  reKallat
//
//  Created by Hayden on 12/1/20.
//

#ifndef sand_hpp
#define sand_hpp

#include "ofMain.h"
#include <stdio.h>

class sand {
    
    public:
        sand();
        ofVec2f pos;
        float mass;
        void set_size(int size_set);
        void initialize(ofVec2f position, float mass_val, float mspeed, float mforce);
        void draw();
        void apply_force(ofVec2f force, float noise);
        void move();
    
    private:
        // Variables
        int size = 1;
        ofVec2f vel;
        ofVec2f acc;
        float max_speed;
        float max_force;
    
};


#endif /* sand_hpp */
