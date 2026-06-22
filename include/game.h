#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <vector>
#include <ctime>
#include <iostream>
#include <sstream>

// Game engine class
// Wrapper

class Game {
public:

  // Destructor, constructor

  Game();
  virtual ~Game();

  // Methods
  
  void update();// Trigger game update
  void poll_events();// While event: check events, execute winfow methods
  void update_mouse_positions();
  void update_text();
  void render();// Clear window, draw objects, load new window
  void render_text(sf::RenderTarget& target);
  
  // Accesors
  const bool running() const;// Return if window is open
  const bool ended_game() const;
  
  void spawn_enemy();
  void update_enemies();
  void render_enemies(sf::RenderTarget& target);

private:

  // Interface methods
  
  void init_window();// Initialize window
  void init_variables();// Initialize variables(window)
  void init_enemies(); // Initialize enemies
  void init_fonts(); // Initialize fonts
  void init_text();

  // Game logic

  unsigned points;
  bool mouse_held;
  float enemy_spawn_timer;
  float enemy_spawn_timer_max;
  int max_enemies;
  int health;
  bool end_game;
  
  // Game objects

  std::vector<sf::RectangleShape> enemies;
  sf::RectangleShape enemy;

  // Rsources
  sf::Font font;

  // Text
  sf::Text ui_text;

  // Interface objects
  
  sf::Vector2i mouse_position_window;
  sf::Vector2f mouse_position_view;

  sf::RenderWindow* window;
  sf::Event event;
  sf::VideoMode video_mode;
};
