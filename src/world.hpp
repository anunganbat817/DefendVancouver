#pragma once

// internal
#include "common.hpp"
#include "entities/player.hpp"
#include "entities/enemies/shooter.hpp"
#include "entities/enemies/bomber.hpp"
#include "entities/enemies/chaser.hpp"
#include "background.hpp"
#include "camera.hpp"
#include "entities/bombs/normalBomb.hpp"
#include "entities/bombs/bomberBomb.hpp"
#include "entities/bullets/playerBullet.hpp"
#include "explosion.hpp"
#include "collisions/quadTreeNode.hpp"
#include "entities/powerups/OneUp.hpp"
#include "entities/powerups/Shield.hpp"

// stlib
#include <vector>
#include <random>
#include <SFML/Audio.hpp>

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class World {
public:
    World();

    ~World();

    // Creates a window, sets up events and begins the game
    bool init(vec2 screenSize, vec2 worldSize);

    // Releases all associated resources
    void destroy();

    // Steps the game ahead by ms milliseconds
    void update(float ms);

    // Renders our scene
    void draw();

    // Should the game be over ?
    bool is_over();

    vec2 getPlayerPosition() const;

    std::vector<vec2> getBombPositions() const;

    vec2 getCityPosition() const;

    void addEntity(std::shared_ptr<Entity> entity);

    vec2 getSize() const;

    bool isEntityInView(const Entity &entity) const;

    float bulletAngleRelativeToPlayer;
    vec2 bulletDirectionRelativeToPlayer;
    bool isGraphCreated = false;

private:
    bool initTextures();
    std::shared_ptr<Player> getPlayer() const;
    std::shared_ptr<background> getBackground() const;

    void playerBounce(const NormalBomb &bomb);

    // !!! INPUT CALLBACK FUNCTIONS
    void onKey(GLFWwindow *, int key, int, int action, int mod);

    void onMouseMove(GLFWwindow *window, double xpos, double ypos);

    void onMouseClick(GLFWwindow *window, int button, int action, int mod);

private:
    // Window handle
    GLFWwindow *m_window;

    // Number of fish eaten by the salmon, displayed in the window title
    unsigned int m_points;

    /**
     * All entities in the world. The background will always be the first entity, followed by the player.
     */
    std::vector<std::shared_ptr<Entity>> m_entities;
    std::vector<std::shared_ptr<Shooter>> m_shooters;
    float m_next_shooter_spawn;
    float m_next_chaser_spawn;
    float m_next_bomber_spawn;
    float m_next_nbomb_spawn;
    float m_next_bbomb_spawn;
    float m_next_oneup_spawn;
    float m_next_shield_spawn;
    background *m_background;

    sf::Music m_background_music;
    sf::Music m_playershooting_music;
    sf::Music m_itemcollect_music;
    sf::Music m_aliendeath_music;
    sf::Music m_bomb_music;
    sf::Music m_hit_music;
    sf::Music m_gameover_music;
    // C++ rng
    std::default_random_engine m_rng;
    std::uniform_real_distribution<float> m_dist; // default 0..1
    static Texture world_texture;

    // World size
    vec2 m_size;
    Camera m_camera;

    QuadTreeNode m_quad;
    
    int totalEnemies;
    int waveNo;
    
};
