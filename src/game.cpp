#include "../include/game.h"
#include <SFML/Graphics/RectangleShape.hpp>

// Constructor, destructor

Game::Game() {
  this->init_variables();
  this->init_window();
  this->init_enemies();
  this->init_fonts();
  this->init_text();
}

Game::~Game() { delete this->window; }

// Public

void Game::update() {
  this->poll_events();
  if (!this->end_game) {
    this->update_enemies();
    this->update_mouse_positions();
    this->update_text();
    }

  // End game condition

  if (this->health == 0)
    this->end_game = true;
}

void Game::poll_events() {
  while (this->window->pollEvent(this->event)) {
    switch (this->event.type) {
    case sf::Event::Closed:
      this->window->close();
      break;
    case sf::Event::KeyPressed:
      if (this->event.key.code == sf::Keyboard::Escape)
        this->window->close();
      break;
    }
  }
}

void Game::update_text() {
  std::stringstream ss;
  ss << "Points: " << this->points << "\n"
    << "Health: " << this->health;
  this->ui_text.setString(ss.str());
}

void Game::update_mouse_positions() {
  this->mouse_position_window = sf::Mouse::getPosition(*this->window);
  this->mouse_position_view = this->window->mapPixelToCoords(this->mouse_position_window);
}

void Game::render() {
  // Clear window
  this->window->clear();
  // Draw game objects
  this->render_enemies(*this->window);
  // Render text
  this->render_text(*this->window);
  // Display objects
  this->window->display();
}

void Game::render_text(sf::RenderTarget& target) {
  target.draw(this->ui_text);
}

const bool Game::running() const { return this->window->isOpen(); }

const bool Game::ended_game() const { return this->end_game; }

void Game::spawn_enemy() {
  this->enemy.setPosition(
      static_cast<float>(
        rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)
        ),
      0.f
      );
  // Randomize enemy types
  int type = rand() % 5;

  switch(type) {
    case 0:
      this->enemy.setSize(sf::Vector2f(30.f,30.f));
      this->enemy.setFillColor(sf::Color::Magenta);
      break;
    case 1:
      this->enemy.setSize(sf::Vector2f(45.f,45.f));
      this->enemy.setFillColor(sf::Color::Blue);
      break;
    case 2:
      this->enemy.setSize(sf::Vector2f(50.f,50.f));
      this->enemy.setFillColor(sf::Color::Cyan);
      break;
    case 3:
      this->enemy.setSize(sf::Vector2f(70.f,70.f));
      this->enemy.setFillColor(sf::Color::Red);
      break;
    case 4:
      this->enemy.setSize(sf::Vector2f(100.f,100.f));
      this->enemy.setFillColor(sf::Color::Green);
      break;
    default:
      this->enemy.setSize(sf::Vector2f(100.f,100.f));
      this->enemy.setFillColor(sf::Color::Yellow);
        break;
  }

  this->enemies.push_back(this->enemy);// Spawn enemy
}

void Game::update_enemies() {
  // Updating timer for enemy spawning
  if (this->enemies.size() < this->max_enemies) {
    if(this->enemy_spawn_timer == this->enemy_spawn_timer_max) {
      this->spawn_enemy();
      this->enemy_spawn_timer = 0.f;
    }
    else
      this->enemy_spawn_timer += 1.f;
  }

  // Move and update enemies. Removes enemies if they are below the screen
  for(int i=0; i<this->enemies.size(); i++) {
    bool del = false;
    // Move the enemy
    this->enemies[i].move(0.f,2.f);
    // Delete enemies: Check if enemy goes out of bounds
    if (this->enemies[i].getPosition().y > this->window->getSize().y) {
      this->enemies.erase(this->enemies.begin() + i);
      this->health-=1;
      std::cout << "Health: " << this->health << std::endl;
    }
  }

    // Delete enemies: Check if mouse is pressed (add points if we kill an enemy)
  if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    if (this->mouse_held == false) {
      this->mouse_held = true;
      bool del = false;
      for (size_t i = 0; (i < this->enemies.size()) && (del == false); i++) {
        if (this->enemies[i].getGlobalBounds().contains(this->mouse_position_view)) {
            // Gain pointsa
            if (this->enemies[i].getFillColor() == sf::Color::Magenta)
              this->points += 10;
            else if (this->enemies[i].getFillColor() == sf::Color::Blue)
              this->points += 7;
            else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
              this->points += 5;
            else if (this->enemies[i].getFillColor() == sf::Color::Red)
              this->points += 3;
            else 
              this->points += 1;
            std::cout << "Points: " << this->points << std::endl;
            // Delete the enemy
            del = true;
            this->enemies.erase(this->enemies.begin() + i);
        }
      }
    }
  }
  else {
    this->mouse_held = false;
  }
}

void Game::render_enemies(sf::RenderTarget& target) {
  for(auto& enemy : enemies)
    target.draw(enemy);

}

// Private

void Game::init_window() {

  this->video_mode.height = 600;
  this->video_mode.width = 800;
  this->window = new sf::RenderWindow(this->video_mode, "First game",
                                      sf::Style::Titlebar | sf::Style::Close);
  this->window->setFramerateLimit(60);
}

void Game::init_variables() {
  this->window = nullptr; // Dynamic allocation
  this->points = 0;
  this->enemy_spawn_timer_max = 5.f;
  this->enemy_spawn_timer = this->enemy_spawn_timer_max;
  this->max_enemies = 5;
  this->mouse_held = false;
  this->health = 10;
  this->end_game = false;
}

void Game::init_enemies() {
  this->enemy.setPosition(10.f,10.f);
  this->enemy.setSize(sf::Vector2f(100.f,100.f));
  this->enemy.setScale(sf::Vector2f(sf::Vector2f(0.5f,0.5f)));
  this->enemy.setFillColor(sf::Color::Cyan);
  //this->enemy.setOutlineColor(sf::Color::Green);
  //this->enemy.setOutlineThickness(1.f);
}

void Game::init_fonts() {
  if(this->font.loadFromFile("fonts/RecursiveSansCslSt-Italic.ttf"))
    std::cout << "ERROR. GAME::INIT_FONTS. Failed to load font." << std::endl;
}

void Game::init_text() {
  this->ui_text.setFont(this->font);
  this->ui_text.setCharacterSize(24);
  this->ui_text.setFillColor(sf::Color::White);
  this->ui_text.setString("NONE");
}
