// #include <SFML/Graphics.hpp>
// #include <iostream>

// int main() {
//     // Create a window
//     sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Mouse Input Example");

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             // Close window event
//             if (event.type == sf::Event::Closed) {
//                 window.close();
//             }

//             // Detect mouse button press
//             if (event.type == sf::Event::MouseButtonPressed) {
//                 if (event.mouseButton.button == sf::Mouse::Left) {
//                     std::cout << "Left mouse button pressed at ("
//                               << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
//                 } else if (event.mouseButton.button == sf::Mouse::Right) {
//                     std::cout << "Right mouse button pressed at ("
//                               << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
//                 }
//             }

//             // Detect mouse button release
//             if (event.type == sf::Event::MouseButtonReleased) {
//                 if (event.mouseButton.button == sf::Mouse::Left) {
//                     std::cout << "Left mouse button released at ("
//                               << event.mouseButton.x << ", " << event.mouseButton.y << ")\n";
//                 }
//             }
//         }

//         // Get the mouse position relative to the window
//         sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
//         std::cout << "Mouse position relative to window: ("
//                   << mousePosition.x << ", " << mousePosition.y << ")\r";

//         // Clear and display the window
//         window.clear(sf::Color::Black);
//         window.display();
//     }

//     return 0;
// }

// -------------------------------------------------------------

// #include <SFML/Graphics.hpp>

// int main() {
//     sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

//     sf::CircleShape shape(50); // Circle with radius 50
//     shape.setFillColor(sf::Color::Green);

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         window.clear(); // Clear screen
//         window.draw(shape); // Draw the shape
//         window.display(); // Display the content
//     }

//     return 0;
// }

// -------------------------------------------------------------

// #include <SFML/Graphics.hpp>
// #include <SFML/Window.hpp>
// #include<iostream>

// int main() {
//     sf::RenderWindow window(sf::VideoMode(800, 600), "Input Example");

//     while (window.isOpen()) {
//         sf::Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//         }

//         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
//             std::cout << "Left Arrow Pressed" << std::endl;
//             break;
//         }
//     }

//     return 0;
// }

// -------------------------------------------------------------

// #include <SFML/System.hpp>
// #include <iostream>

// int main() {
//     sf::Clock clock;

//     // Simulating some workload
//     sf::sleep(sf::seconds(1.5f));

//     sf::Time elapsed = clock.getElapsedTime();
//     std::cout << "Elapsed time: " << elapsed.asSeconds() << " seconds" << std::endl;

//     return 0;
// }

// -------------------------------------------------------------

#include <SFML/Audio.hpp>
#include <iostream>

int main() {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("sound.wav")) {
        std::cout << "Error loading sound!" << std::endl;
        return -1;
    }

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();

    sf::Music music;
    if (!music.openFromFile("music.ogg")) {
        std::cout << "Error loading music!" << std::endl;
        return -1;
    }
    music.play();

    // Keep the program running to let the sound/music play
    sf::sleep(sf::seconds(5));
    return 0;
}

// -------------------------------------------------------------

// #include <SFML/Audio.hpp>
// #include <iostream>
// #include <cstdlib> // For system()

// using namespace std;

// bool checkAndConvertToOgg(const string& filePath, const string& outputFile) {
//     // Use ffmpeg to check and convert the file
//     string command = "ffmpeg -y -i " + filePath + " -c:a libvorbis " + outputFile + " 2>/dev/null";
//     int result = system(command.c_str());
//     return result == 0; // Return true if the command succeeds
// }

// int main() {
//     const string originalFile = "music.ogg";
//     const string fixedFile = "fixed_music.ogg";

//     // Check and convert the file
//     if (!checkAndConvertToOgg(originalFile, fixedFile)) {
//         cout << "Error converting file to OGG format!" << endl;
//         return -1;
//     }

//     // Load and play the converted file
//     sf::Music music;
//     if (!music.openFromFile(fixedFile)) {
//         cout << "Error loading music!" << endl;
//         return -1;
//     }
//     music.play();

//     cout << "Playing music..." << endl;
//     sf::sleep(sf::seconds(10));
//     return 0;
// }

// --------------------------------------------------------------------




