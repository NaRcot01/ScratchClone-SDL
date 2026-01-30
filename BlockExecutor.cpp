//
// Created by Amir on 1/30/2026.
//
#include "BLOCK.h"
#include "Sprite.h"
#include <cmath>
#include <iostream>

const double pi = 3.141592653589;

void executeBlock (const Block &block , Sprite & sprite){
    switch (blokc.type) {
        case BlockType::MOVE:{
            if(!block.parameters.empty()){
                steps = block.parameters[0];
            }
            double rad = spirite.direction * (pi / 180.0)
            double dx = steps * std::cos(rad);
            double dy = steps * std::sin(rad);

            sprite.x +=dx ;
            sprite.y +=dy;

            std::cout << "Moved " << steps << " steps. New Pos: (" //in khat ha faghat baray test va debug hast.
                      << sprite.x << ", " << sprite.y << ")" << std::endl;
            break;

        }
        default:
            break;
    }
}