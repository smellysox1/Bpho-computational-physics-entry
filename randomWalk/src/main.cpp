#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

//#include <math>

#include "util/logger.hpp"
#include "util/random.hpp"



struct pathVisualiser : sf::Drawable, sf::Transformable {
	//pathvisualiser
	
	pathVisualiser() {
		m_vertices.setPrimitiveType(sf::LinesStrip);
	}
	void add_vertex(sf::Vector2f pos,sf::Color colour) {
		
		m_vertices.append({ {pos.x,pos.y},colour });
	}
	void test_n() {
		WC2_LOG(m_vertices.getVertexCount());
	}
	sf::Vector2f lastVertex() {
		return m_vertices[m_vertices.getVertexCount() - 1].position;
	}
private:
	sf::VertexArray m_vertices;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(m_vertices, states);
	}
};

//do we need more than 1 file for this one??
int main() {
	
	//declare SFML required declarations
	sf::RenderWindow window{ sf::VideoMode::getDesktopMode(), "line game.", sf::Style::Fullscreen };
	int SCREEN_WIDTH = window.getSize().x;
	int SCREEN_HEIGHT = window.getSize().y;
	window.setTitle("random wlak");
	pathVisualiser path;
	int n=1000;
	int step=5;
	path.add_vertex({ float(SCREEN_WIDTH/2),float(SCREEN_HEIGHT/2) }, sf::Color::White);
	for (int i = 0; i < n; i++) {
		sf::Color colour(255 * random::randomFloat(), 255 * random::randomFloat(), 255 * random::randomFloat());
		float angle = random::randomFloat() * 2 * 3.141592;
		//placeholder for pi will fix later
		float x = path.lastVertex().x+ step* sin(angle);
		float y = path.lastVertex().y + step*cos(angle);
		WC2_LOG(x," ", y);
		path.add_vertex({x,y}, colour);
	}
	path.test_n();
	//the drawing only needs to happen once.
	window.draw(path);
	window.display();
	bool running = 1;
	while (running) {
		
		//not doing anything, just holding  the tab
	}
	window.close();
}