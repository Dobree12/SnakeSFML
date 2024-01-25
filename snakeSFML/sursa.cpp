#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int blockSize = 30;
const int screenWidth = 1080;
const int screenHeight = 800;

class Snake {
public:
    Snake();
    void handleInput();
    void update();
    void render(sf::RenderWindow& window);
    bool checkCollision(sf::Vector2f position);
    void spawnFood();
    void updateScoreText();
    void render(sf::RenderWindow& window, sf::Texture& backgroundTexture, sf::Texture& headTexture);
private:
    sf::RectangleShape snakeBody;
    std::vector<sf::RectangleShape> snake;
    sf::Vector2f direction;
    sf::RectangleShape food;
    int score;

    sf::Font font;
    sf::Text scoreText;
};

Snake::Snake() {
    snakeBody.setSize(sf::Vector2f(blockSize, blockSize));
    snakeBody.setFillColor(sf::Color::Green);
    snake.push_back(snakeBody);
    direction = sf::Vector2f(1, 0); // Initial direction: right
    score = 0;

    // Load font
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set up score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    updateScoreText(); // Call a function to set the initial score text

    food.setSize(sf::Vector2f(blockSize, blockSize));
    food.setFillColor(sf::Color::Red);
    spawnFood();
}

void Snake::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction.y == 0) {
        direction = sf::Vector2f(0, -1);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction.y == 0) {
        direction = sf::Vector2f(0, 1);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction.x == 0) {
        direction = sf::Vector2f(-1, 0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction.x == 0) {
        direction = sf::Vector2f(1, 0);
    }
}

void Snake::update() {
    // Move the head of the snake
    snake[0].move(sf::Vector2f(direction.x * blockSize, direction.y * blockSize));

    sf::Vector2f headPosition = snake[0].getPosition();

    // Check collision with walls
    if (headPosition.x < 0 || headPosition.x >= screenWidth ||
        headPosition.y < 0 || headPosition.y >= screenHeight) {
        std::cout << "Game Over! You hit the wall. Score: " << score << std::endl;
        exit(EXIT_SUCCESS);
    }

    // Check collision with itself
    if (checkCollision(headPosition)) {
        std::cout << "Game Over! You collided with yourself. Score: " << score << std::endl;
        exit(EXIT_SUCCESS);
    }

    // Check if the snake ate food
    if (headPosition.x == food.getPosition().x && headPosition.y == food.getPosition().y) {
        spawnFood();
        // Increase snake size
        snake.push_back(sf::RectangleShape(snakeBody));
        // Increase the score
        score++;
        // Update the score text
        updateScoreText();
    }

    // Move the body of the snake
    for (size_t i = snake.size() - 1; i > 0; --i) {
        sf::Vector2f prevPosition = snake[i - 1].getPosition();
        snake[i].setPosition(prevPosition);
    }
}
void Snake::render(sf::RenderWindow& window, sf::Texture& backgroundTexture, sf::Texture& headTexture) {
    // Draw the background first
    sf::Sprite background(backgroundTexture);
    window.draw(background);

    // Draw the snake head with texture and scale it
    sf::Sprite headSprite(headTexture);
    headSprite.setPosition(snake[0].getPosition());

    // Scale the head sprite to fit the blockSize
    float scaleFactor = blockSize / static_cast<float>(headTexture.getSize().x);
    headSprite.setScale(scaleFactor, scaleFactor);

    window.draw(headSprite);

    // Draw the remaining snake segments
    for (size_t i = 1; i < snake.size(); ++i) {
        window.draw(snake[i]);
    }

    // Draw the food and score text
    window.draw(food);
    window.draw(scoreText);
}



bool Snake::checkCollision(sf::Vector2f position) {
    for (size_t i = 1; i < snake.size(); ++i) {
        if (position == snake[i].getPosition()) {
            return true;
        }
    }
    return false;
}

void Snake::spawnFood() {
    // Randomly place food on the screen
    srand(static_cast<unsigned>(time(nullptr)));
    int x = rand() % (screenWidth / blockSize);
    int y = rand() % (screenHeight / blockSize);
    food.setPosition(x * blockSize, y * blockSize);
}

void Snake::updateScoreText() {
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setPosition(screenWidth - 150, 10);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Snake Game");
    window.setFramerateLimit(10);

    Snake snake;

    // Load background texture
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.jpg")) {
        std::cerr << "Failed to load background image." << std::endl;
        return EXIT_FAILURE;
    }

    // Load snake head texture
    sf::Texture headTexture;
    if (!headTexture.loadFromFile("snake_head.png")) {
        std::cerr << "Failed to load snake head image." << std::endl;
        return EXIT_FAILURE;
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        snake.handleInput();
        snake.update();

        window.clear();
        snake.render(window, backgroundTexture, headTexture);
        window.display();
    }

    return 0;
}
