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
const int step = 1;


class pathVisualiser : public sf::Drawable, public sf::Transformable {
public:
	pathVisualiser() {
		m_vertices.setPrimitiveType(sf::LinesStrip);
		//m_vertices.resize(n);
	}

	void add_vertex(int i, sf::Vector2f pos,sf::Color colour) {
		m_vertices.append({ pos, colour });
	}
	std::vector<sf::Vector2f> give_verts() {
		std::vector<sf::Vector2f> results;
		for (int i = 0; i < m_vertices.getVertexCount(); i++) {
			results.push_back(m_vertices[i].position);
		}
		return results;
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

pathVisualiser path_to_hunt;

float errorFromPath(pathVisualiser p) {
	float error = 0.0;
	std::vector<sf::Vector2f> hunt_verts = path_to_hunt.give_verts();
	int i = 0;
	for (sf::Vector2f vert : p.give_verts()) {
		error += (vert.x - hunt_verts[i].x)*(vert.x - hunt_verts[i].x) + (vert.y - hunt_verts[i].y)*(vert.y - hunt_verts[i].y);
		i++;
	}
	return error;
}

float randomFloat(std::mt19937 rng) {
	static constexpr const float X = rng.max();
	return rng() / X;
}

int main() {
	

	sf::RenderWindow window{ { 1280, 720 }, "if you see anything, something is incredibly wrong.", sf::Style::Default };

	const int SCREEN_WIDTH = window.getSize().x;
	const int SCREEN_HEIGHT = window.getSize().y;

	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2) }, sf::Color::White);
	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2+1), float(SCREEN_HEIGHT / 2) }, sf::Color::White);
	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2 + 1 + sqrt(3) / 2), float(SCREEN_HEIGHT / 2 + 0.5) }, sf::Color::White);
	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2 + 1 + sqrt(3) / 2), float(SCREEN_HEIGHT / 2 - 0.5) }, sf::Color::White);
	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2 + 1 + sqrt(3) / 2), float(SCREEN_HEIGHT / 2 - 1.5) }, sf::Color::White);
	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2 + 1), float(SCREEN_HEIGHT / 2 - 1) }, sf::Color::White);
	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2) , float(SCREEN_HEIGHT / 2 - 1) }, sf::Color::White);
	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2 - sqrt(3) / 2), float(SCREEN_HEIGHT / 2 - 1.5) }, sf::Color::White);
	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2 - sqrt(3) / 2), float(SCREEN_HEIGHT / 2 - 0.5) }, sf::Color::White);
	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2 - sqrt(3) / 2), float(SCREEN_HEIGHT / 2 + 0.5) }, sf::Color::White);
	path_to_hunt.add_vertex(0, { float(SCREEN_WIDTH / 2)  , float(SCREEN_HEIGHT / 2) }, sf::Color::White);

	int j = 0;
	float error = 999999999999999;//something such that it starts off with error
	while (1) {//each loop here generates a new path with the random number.
		std::mt19937 rng{ static_cast<std::mt19937::result_type>(j) };

		pathVisualiser path;

		path.add_vertex(0, { float(SCREEN_WIDTH / 2), float(SCREEN_HEIGHT / 2) }, sf::Color::White);
		sf::Color colour(255, 255, 255);
		for (int i = 1; i < n; i++) {
			

			float angle = randomFloat(rng) * 2 * 3.141592;

			float x = path.lastVertex().x + step * sin(angle);
			float y = path.lastVertex().y + step * cos(angle);

			path.add_vertex(i, { x,y },colour);
		}
		float currentError = errorFromPath(path);
		
		if (currentError <= error) {
			error = currentError;
			std::cout << j << " has an error of: " << error << "\n\n\n\n";
		}

		j++;
		if (error == 0) break;
	}

	

}