//
//  sand.cpp
//  reKallat
//
//  Created by Hayden on 12/1/20.
//


#include <stdio.h>
#include "sand.hpp"

sand::sand(){
}

void sand::set_size(int size_set) {
    size = size_set;
}

void sand::initialize(ofVec2f position, float mass_val, float mspeed, float mforce) {
    
    pos = position;
    mass = mass_val;
    vel = ofVec2f(0, 0);
    acc = ofVec2f(0, 0);
    
    max_speed = mspeed;
    max_force = mforce;
    
}

void sand::draw() {
    ofDrawCircle(pos.x, pos.y, size);
}

void sand::apply_force(ofVec2f force, float noise) {
    
    float noise_x = ofRandom(-noise, noise);
    float noise_y = ofRandom(-noise, noise);
    
    ofVec2f noise_vec = ofVec2f(noise_x, noise_y);
    
    force.x = ofClamp(force.x, -max_force, max_force);
    force.y = ofClamp(force.y, -max_force, max_force);
    acc = acc + (force / mass) + noise_vec;
}

void sand::move() {
    // Update velocity
    vel = vel + acc;
    vel.x = ofClamp(vel.x, -max_speed, max_speed);
    vel.y = ofClamp(vel.y, -max_speed, max_speed);
    
    // Update Position
    pos = pos + vel;
    
    // Reset Acceleration
    acc = ofVec2f(0,0);
}




