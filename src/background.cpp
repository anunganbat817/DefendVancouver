//
// Created by Anun Ganbat on 2018-02-09.
//
#include <vector>
#include <iostream>
#include <cmath>
#include "background.hpp"
#include "world.hpp"
#include <OpenGL/OpenGL.h>


Texture background::background_texture;

background::background(World &world) : Entity(world) {}

bool background::initTexture() {
    //load texture
    if (!background_texture.is_valid()) {
        if (!background_texture.load_from_file(textures_path("skyline.png"))) {
            fprintf(stderr, "Failed to load background texture!");
            return false;
        }
    }
    return true;
}

bool background::init() {
    // The position corresponds to the center of the texture
    float wr = background_texture.width * 0.5f;
    float hr = background_texture.height * 0.5f;

    TexturedVertex vertices[4];
    vertices[0].position = {-wr, +hr, -0.01f};
    vertices[0].texcoord = {0.f, 1.f};
    vertices[1].position = {+wr, +hr, -0.01f};
    vertices[1].texcoord = {1.f, 1.f,};
    vertices[2].position = {+wr, -hr, -0.01f};
    vertices[2].texcoord = {1.f, 0.f};
    vertices[3].position = {-wr, -hr, -0.01f};
    vertices[3].texcoord = {0.f, 0.f};

    uint16_t indices[] = {0, 3, 1, 1, 3, 2};
    // Clearing errors
    gl_flush_errors();

    // Vertex Buffer creation
    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedVertex) * 4, vertices, GL_STATIC_DRAW);

    // Index Buffer creation
    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, indices, GL_STATIC_DRAW);

    // Vertex Array (Container for Vertex + Index buffer)
    glGenVertexArrays(1, &mesh.vao);
    if (gl_has_errors())
        return false;

    // Loading shaders
    if (!effect.load_from_file(shader_path("textured.vs.glsl"), shader_path("textured.fs.glsl")))
        return false;
    m_scale.x = 1.0f;
    m_scale.y = 1.0f;
    m_position.x = 1500;
    m_position.y = 1259;
    m_health = 1000;

    return true;
}

void background::update(float ms) {

}

void background::destroy() {

}

void background::draw(const mat3 &projection) {
    // Transformation code, see Rendering and Transformation in the template specification for more info
    // Incrementally updates transformation matrix, thus ORDER IS IMPORTANT
    // Setting shaders
    transform_begin();
    transform_translate(m_position);
    transform_scale(m_scale);
    transform_end();
    glUseProgram(effect.program);

    // Enabling alpha channel for textures
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Getting uniform locations for glUniform* calls
    GLint transform_uloc = glGetUniformLocation(effect.program, "transform");
    GLint color_uloc = glGetUniformLocation(effect.program, "fcolor");
    GLint projection_uloc = glGetUniformLocation(effect.program, "projection");

    // Setting vertices and indices
    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    // Input data location as in the vertex buffer
    GLint in_position_loc = glGetAttribLocation(effect.program, "in_position");
    GLint in_texcoord_loc = glGetAttribLocation(effect.program, "in_texcoord");
    glEnableVertexAttribArray(in_position_loc);
    glEnableVertexAttribArray(in_texcoord_loc);
    glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) 0);
    glVertexAttribPointer(in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex), (void *) sizeof(vec3));

    // Enabling and binding texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, background_texture.id);

    // Setting uniform values to the currently bound program
    glUniformMatrix3fv(transform_uloc, 1, GL_FALSE, (float *) &transform);
    float color[] = {1.f, 1.f, 1.f};
    glUniform3fv(color_uloc, 1, color);
    glUniformMatrix3fv(projection_uloc, 1, GL_FALSE, (float *) &projection);

    // Drawing!
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
}

int background::getHealth() {
    return m_health;
}

void background::addHealth() {
    m_health++;
}

void background::decreaseHealth() {
    m_health--;
}

Region background::getBoundingBox() const {
    vec2 boxSize = {std::fabs(m_scale.x) * background_texture.width, std::fabs(m_scale.y) * background_texture.height};
    vec2 boxOrigin = { m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};

    return {boxOrigin, boxSize};
}

//Region background::getBoundingBox1() const {
//    // fabs is to avoid negative scale due to the facing direction
////    return {(std::fabs(m_scale.x) * (646.27f / 646.27f)) * 4981.50f, (std::fabs(m_scale.y) * (472.91f / 472.91f)) * 453.26f};
//    vec2 boxSize = {(std::fabs(m_scale.x) * (646.27f - 182.35f)) + 182.35f, (std::fabs(m_scale.y) * (472.91f - 231.85f)) + 231.85f};
//    vec2 boxOrigin = {m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};
//    return {boxOrigin, boxSize};
//}
//
//Region background::getBoundingBox2() const {
//    vec2 boxSize = {(std::fabs(m_scale.x) * (774.05f - 648.93f)) + 648.93f, (std::fabs(m_scale.y) * (472.91f - 13.07f)) + 13.07f};
//    vec2 boxOrigin = {m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};
//    return {boxOrigin, boxSize};
//}
//
//Region background::getBoundingBox3() const {
//    vec2 boxSize = {(std::fabs(m_scale.x) * (3138.70f - 772.43f)) + 772.43f, (std::fabs(m_scale.y) * (466.86f - 466.86f)) + 466.86f};
//    vec2 boxOrigin = {m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};
//    return {boxOrigin, boxSize};
//}
//
//Region background::getBoundingBox4() const {
//    vec2 boxSize = {(std::fabs(m_scale.x) * (3308.46f - 3197.19f)) + 3197.19f, (std::fabs(m_scale.y) * (474.85f - 47.73f)) + 47.73f};
//    vec2 boxOrigin = {m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};
//    return {boxOrigin, boxSize};
//}
//
//Region background::getBoundingBox5() const {
//    vec2 boxSize = {(std::fabs(m_scale.x) * (4986.41f - 3314.76f)) + 3314.76f, (std::fabs(m_scale.y) * (474.09f - 258.45f)) + 258.45f};
//    vec2 boxOrigin = {m_position.x - boxSize.x / 2, m_position.y - boxSize.y / 2};
//    return {boxOrigin, boxSize};
//}
//
std::string background::getName() const {
    return "background";
}
//
//bool background::colCheck1(ShooterBullet &sb) {
//    float dx = (m_position.x - sb.getPosition().x);
//    float dy = (m_position.y - sb.getPosition().y);
//    float d_sq = dx * dx + dy * dy;
//    float other_r = std::max(sb.getBoundingBox().size.x, sb.getBoundingBox().size.y);
//    float my_r = std::max(getBoundingBox().size.x, getBoundingBox().size.y);
//    auto combinedRadii = (my_r + other_r) / 2;
//    return d_sq < combinedRadii * combinedRadii;
//}
//
//bool background::colCheck(ShooterBullet &sb) {
//    float dx = (m_position.x - sb.getPosition().x);
//    float dy = (m_position.y - sb.getPosition().y);
//    float d_sq = dx * dx + dy * dy;
//    float other_r = std::max(sb.getBoundingBox().size.x, sb.getBoundingBox().size.y);
//    float my_r = std::max(getBoundingBox1().size.x, getBoundingBox1().size.y);
//    float r = std::max(other_r, my_r);
//    r *= 0.5f;
//    if (d_sq < r * r)
//    {
//        return true;
//    }
//    return false;
////    auto combinedRadii = (my_r + other_r) / 2;
////    return d_sq < combinedRadii * combinedRadii;
//}
//
//bool background::colCheck2(ShooterBullet &sb) {
//    float dx = (m_position.x - sb.getPosition().x);
//    float dy = (m_position.y - sb.getPosition().y);
//    float d_sq = dx * dx + dy * dy;
//    float other_r = std::max(sb.getBoundingBox().size.x, sb.getBoundingBox().size.y);
//    float my_r = std::max(getBoundingBox2().size.x, getBoundingBox2().size.y);
////    auto combinedRadii = (my_r + other_r) / 2;
////    return d_sq < combinedRadii * combinedRadii;
//    float r = std::max(other_r, my_r);
//    r *= 0.5f;
//    if (d_sq < r * r)
//    {
//        return true;
//    }
//    return false;
//}
//
//bool background::colCheck3(ShooterBullet &sb) {
//    float dx = (m_position.x - sb.getPosition().x);
//    float dy = (m_position.y - sb.getPosition().y);
//    float d_sq = dx * dx + dy * dy;
//    float other_r = std::max(sb.getBoundingBox().size.x, sb.getBoundingBox().size.y);
//    float my_r = std::max(getBoundingBox3().size.x, getBoundingBox3().size.y);
////    auto combinedRadii = (my_r + other_r) / 2;
////    return d_sq < combinedRadii * combinedRadii;
//    float r = std::max(other_r, my_r);
//    r *= 0.5f;
//    if (d_sq < r * r)
//    {
//        return true;
//    }
//    return false;
//}
//
//bool background::colCheck4(ShooterBullet &sb) {
//    float dx = (m_position.x - sb.getPosition().x);
//    float dy = (m_position.y - sb.getPosition().y);
//    float d_sq = dx * dx + dy * dy;
//    float other_r = std::max(sb.getBoundingBox().size.x, sb.getBoundingBox().size.y);
//    float my_r = std::max(getBoundingBox4().size.x, getBoundingBox4().size.y);
////    auto combinedRadii = (my_r + other_r) / 2;
////    return d_sq < combinedRadii * combinedRadii;
//    float r = std::max(other_r, my_r);
//    r *= 0.5f;
//    if (d_sq < r * r)
//    {
//        return true;
//    }
//    return false;
//}
//
//bool background::colCheck5(ShooterBullet &sb) {
//    float dx = (m_position.x - sb.getPosition().x);
//    float dy = (m_position.y - sb.getPosition().y);
//    float d_sq = dx * dx + dy * dy;
//    float other_r = std::max(sb.getBoundingBox().size.x, sb.getBoundingBox().size.y);
//    float my_r = std::max(getBoundingBox5().size.x, getBoundingBox5().size.y);
////    auto combinedRadii = (my_r + other_r) / 2;
////    return d_sq < combinedRadii * combinedRadii;
//    float r = std::max(other_r, my_r);
//    r *= 0.5f;
//    if (d_sq < r * r)
//    {
//        return true;
//    }
//    return false;
//}
