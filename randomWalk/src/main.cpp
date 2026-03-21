#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <chrono>
#include <random>
#include <iostream>
#include <vector>
#include <filesystem>

#define TAU 6.283185307179586

// PARAMETERS

unsigned int n = 10;
unsigned int step = 20;
unsigned int seed = 0;

unsigned int screenWidth = 1280;
unsigned int screenHeight = 720;

class PathVisualiser : public sf::Drawable, public sf::Transformable {
public:
	void newPath() {
		m_vertices.clear();

		m_vertices.resize(n);
		m_vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);

		m_vertices[0] = sf::Vertex({(float)(screenWidth / 2), (float)(screenHeight / 2)}, sf::Color::White);

		for (int i = 1; i < n; i++) {
			sf::Color colour(255 * randomFloat(colourRng), 255 * randomFloat(colourRng), 255 * randomFloat(colourRng));

			float angle = randomFloat(angleRng) * TAU;

			const sf::Vertex& previous = m_vertices[i - 1];

			float x = previous.position.x + step * std::sinf(angle);
			float y = previous.position.y + step * std::cosf(angle);

			m_vertices[i] = sf::Vertex({x, y}, colour);
		}
	}

	void resetRng(unsigned int s) {
		angleRng = std::mt19937(s);
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(m_vertices, states);
	}

	static float randomFloat(std::mt19937& rng) {
		static constexpr const float max = rng.max();
		return rng() / max;
	}

private:
	sf::VertexArray m_vertices;

	std::mt19937 angleRng {seed};
	std::mt19937 colourRng {seed};
};

int main() {
	sf::RenderWindow window(sf::VideoMode({screenWidth, screenHeight}), "omg that's so rnadmo wlka", sf::Style::Default);

	if (!ImGui::SFML::Init(window)) {
		std::cerr << "Could not initialise ImGui for SFML" << "\n";
		return 1;
	}

	PathVisualiser path;
	path.newPath();

	sf::View camera(sf::FloatRect({0, 0}, {(float)screenWidth, (float)screenHeight}));

	sf::Font font;
	if (!font.openFromFile("arial.ttf")) {
		std::cerr << "Could not load font." << "\n";
	}

	sf::Clock clock;

	float zoom = 1.0f;
	sf::Vector2i previousMousePos;
	bool dragging = false;

	while (window.isOpen()) {
		ImGui::SFML::Update(window, clock.restart());

		sf::Vector2i mousepos = sf::Mouse::getPosition();
		screenWidth = window.getSize().x;
		screenHeight = window.getSize().y;

		while (const auto event = window.pollEvent()) {
			ImGui::SFML::ProcessEvent(window, *event);

			if (event->is<sf::Event::Closed>()) {
				std::cout << "Window closed" << "\n";
				window.close();
			} else if (const auto btn = event->getIf<sf::Event::MouseButtonPressed>()) {
				if (mousepos.x <= 500)
					continue;

				if (btn->button == sf::Mouse::Button::Left) {
					previousMousePos = mousepos;
					dragging = true;
				}
			} else if (event->is<sf::Event::MouseMoved>()) {
				if (mousepos.x <= 500) {
					dragging = false;
					continue;
				}

				if (dragging) {
					camera.move(sf::Vector2f(previousMousePos - mousepos) * zoom);
					previousMousePos = mousepos;
				}
			} else if (const auto btn = event->getIf<sf::Event::MouseButtonReleased>()) {
				if (mousepos.x <= 500)
					continue;

				if (btn->button == sf::Mouse::Button::Left) {
					dragging = false;
				}
			} else if (const auto scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
				if (mousepos.x <= 500)
					continue;

				float z = 1.0f - scroll->delta;
				zoom *= z;
				camera.zoom(z);
			}
		}

		window.clear();

		ImGui::SetNextWindowPos({0, 0});
		ImGui::SetNextWindowSize({300.0f, (float)screenHeight});
		ImGui::Begin("Radnom walk", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

		ImGui::Text("N:");
		ImGui::SameLine();
		ImGui::InputScalar("##n", ImGuiDataType_U32, &n);

		ImGui::Text("Step:");
		ImGui::SameLine();
		ImGui::InputScalar("##step", ImGuiDataType_U32, &step);

		ImGui::Text("Seed:");
		ImGui::SameLine();
		ImGui::InputScalar("##seed", ImGuiDataType_U32, &seed);

		if (ImGui::Button("Redraw")) {
			path.resetRng(seed);
			path.newPath();
		}

		ImGui::End();

		window.setView(camera);

		window.draw(path);

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
}