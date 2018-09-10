//
// Created by Anun Ganbat on 2018-02-09.
//


#pragma once

#include "common.hpp"
#include "entities/entity.hpp"
#include "entities/bullets/shooterBullet.hpp"

class background : public Entity, public Renderable {

    static Texture background_texture;
public:
    static bool initTexture();

    background(World &world);

    bool init() override;

    void update(float ms) override;

    void draw(const mat3 &projection);

    void destroy() override;

    Region getBoundingBox() const override;
    Region getBoundingBox1() const ;
    Region getBoundingBox2() const;

    Region getBoundingBox3() const;

    Region getBoundingBox4() const;

    Region getBoundingBox5() const;

    int getHealth();

    void addHealth();

    void decreaseHealth();

    std::string getName() const override;
//
//    bool colCheck1(ShooterBullet &sb);
//    bool colCheck2(ShooterBullet &sb);
//
//    bool colCheck(ShooterBullet &sb);
//
//    bool colCheck3(ShooterBullet &sb);
//
//    bool colCheck4(ShooterBullet &sb);
//    bool colCheck5(ShooterBullet &sb);

private:
    int m_health;

//    Region getBoundingBox1() const;

//    vec2 getBoundingBox2() const;
//
//    vec2 getBoundingBox3() const;
//
//    vec2 getBoundingBox4() const;
//
//    vec2 getBoundingBox5() const;

};


