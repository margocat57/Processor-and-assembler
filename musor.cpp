#include <SFML/Graphics.hpp>
#include <cmath>

int main() {
    const int width = 100;
    const int height = 100;
    const int pixelSize = 5;
    const int RAM_SIZE = width * height * 3;

    unsigned char RAM[RAM_SIZE];

    int cx = width / 2;
    int cy = height / 2;
    int r = 30;

    // Заполняем RAM цветами
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int i = (y * width + x) * 3; // индекс первого байта пикселя
            double dx = x - cx;
            double dy = y - cy;

            if (dx * dx + dy * dy <= r * r) {
                RAM[i] = 255;   // R
                RAM[i + 1] = 0; // G
                RAM[i + 2] = 0; // B
            } else {
                RAM[i] = RAM[i + 1] = RAM[i + 2] = 0; // чёрный фон
            }
        }
    }

    sf::RenderWindow window(sf::VideoMode(width * pixelSize, height * pixelSize), "Круг через RAM");

    sf::RectangleShape pixel(sf::Vector2f(pixelSize, pixelSize));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();

        // Отрисовка пикселей из RAM
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int i = (y * width + x) * 3;
                pixel.setFillColor(sf::Color(RAM[i], RAM[i + 1], RAM[i + 2]));
                pixel.setPosition(x * pixelSize, y * pixelSize);
                window.draw(pixel);
            }
        }

        window.display();
    }
}
