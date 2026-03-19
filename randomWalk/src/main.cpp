#include <SFML/Graphics.hpp>

struct pathVisualiser :sf::Drawable, sf::Transformable {
	//pathvisualiser
};

//do we need more than 1 file for this one??
int main() {
	//declare SFML required declarations
	sf::RenderWindow window{ sf::VideoMode::getDesktopMode(), "Cheese Game", sf::Style::Fullscreen };
	pathVisualiser path();
	int n;


	//the drawing only needs to happen once.
	//window.draw(path);
	window.display();
}