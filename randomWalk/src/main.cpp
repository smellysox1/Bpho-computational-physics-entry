#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <chrono>
#include <random>
#include <iostream>
#include <vector>
#include <filesystem>

// PARAMETERS

const int n = 11;
const int step = 20;

std::mt19937 rng{ static_cast<std::mt19937::result_type>(30449350) };

class pathVisualiser : public sf::Drawable, public sf::Transformable {
public:
	pathVisualiser() {
		m_vertices.setPrimitiveType(sf::LinesStrip);
		//m_vertices.resize(n);
	}

	void add_vertex(int i, sf::Vector2f pos,sf::Color colour) {
		m_vertices.append({ pos, colour });
	}

	sf::Vector2f lastVertex() {
		return m_vertices[m_vertices.getVertexCount() - 1].position;
	}

	inline const sf::VertexArray& getVertices() {
		return m_vertices;
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(m_vertices, states);
	}

private:
	sf::VertexArray m_vertices;
};

float randomFloat() {
	static constexpr const float X = rng.max();
	return rng() / X;
}

int main() {
	sf::RenderWindow window{ { 1280, 720 }, "omg that's so rnadmo wlka", sf::Style::Default };

	const int SCREEN_WIDTH = window.getSize().x;
	const int SCREEN_HEIGHT = window.getSize().y;

	pathVisualiser path;

	path.add_vertex(0, { float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2) }, sf::Color::White);

	for (int i = 1; i < n; i++) {
		sf::Color colour(255 * randomFloat(), 255 * randomFloat(), 255 * randomFloat());

		float angle = randomFloat() * 2 * 3.141592;

		float x = path.lastVertex().x + step * sin(angle);
		float y = path.lastVertex().y + step * cos(angle);

		path.add_vertex(i, {x,y}, colour);
	}

	sf::View camera({0,0,float(SCREEN_WIDTH),float(SCREEN_HEIGHT)});

	

	float zoom = 1.0f;
	sf::Vector2i previousMousePos;
	bool dragging = false;

	while (window.isOpen()) {
		sf::Vector2i mousepos = sf::Mouse::getPosition();

		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					std::cout << "Window closed" << "\n";
					window.close();
					break;
				case sf::Event::MouseButtonPressed:
					previousMousePos = mousepos;
					dragging = true;
					break;
				case sf::Event::MouseMoved:
					if (dragging) {
						camera.move(sf::Vector2f(previousMousePos - mousepos) * zoom);
						previousMousePos = mousepos;
					}
					break;
				case sf::Event::MouseButtonReleased:
					dragging = false;
					break;
				case sf::Event::MouseWheelScrolled:
					zoom *= 1.0f + event.mouseWheelScroll.delta;
					camera.zoom(1.0f + event.mouseWheelScroll.delta);
					break;
			}
		}

		window.clear();

		window.setView(camera);

		window.draw(path);

		window.display();
	}
}