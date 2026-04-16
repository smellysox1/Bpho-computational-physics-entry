#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

#include <cmath>
#include <chrono>
#include <random>
#include <iostream>
#include <vector>
#include <filesystem>

#define TAU 6.283185307179586

// PARAMETERS

float M = 50.0f, m = 5.0f, R = 50.0f, r = 5.0f;
float C = 1.0f;
int n = 500;
unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count();

unsigned int screenWidth = 1280;
unsigned int screenHeight = 720;

bool running = false;

std::mt19937 rng {seed};

float randomFloat() {
	static constexpr const float X = rng.max();
	return rng() / X;
}

class Particle;

//todo : 
//Make UI window less dsigusting
//Have changing settings reset the simulation (to make N changeable.)

std::vector<Particle> collidingObjects;

class particlePath : public sf::Drawable, public sf::Transformable {
public:
	void newPath() {
		m_vertices.clear();

		m_vertices.resize(n);
		m_vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);
	}

	void addPoint(sf::Vector2f v) {
		m_vertices.append({ v, sf::Color::Red });
	}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(m_vertices, states);
	}

private:
	sf::VertexArray m_vertices;
};

particlePath path;

class Particle : public sf::CircleShape {
public:
	sf::Vector2f velocity;
	sf::Vector2f staleVel;

	float speed;

	unsigned int uid = std::chrono::steady_clock::now().time_since_epoch().count() - seed;//the uid makes each particle unique.

	Particle() : sf::CircleShape(r, 30){
		float distance = randomFloat() * 1000.0f + R + 10.0f;
		float angle = randomFloat() * TAU;
		speed = 1.0f;
		velocity = { speed * std::sinf(TAU * randomFloat()), speed * std::cosf(TAU * randomFloat()) };
		setPosition({distance * std::sinf(angle), distance * std::cosf(angle)});

		setPointCount(30);
		updateRadius();

		setFillColor(sf::Color::Green);
	}

	void updateRadius() {
		setRadius(r);
		setOrigin({r, r});
	}

	bool insideParticle(sf::Vector2f point){//chekcs for if the pos is within the circle
		sf::Vector2f dist_from_rad = point - getPosition();
		return dist_from_rad.length() <= r;
	}

	bool isCollision(const Particle& other) {//chekcs for if the pos is within the circle
		sf::Vector2f dist_from_rad = other.getPosition() - getPosition();
		return dist_from_rad.length() <= R + r;
	}

	virtual void update() {
		move(velocity);
		staleVel = velocity;
		bool velDone = 0;
		velocity = {speed * std::sinf(TAU * randomFloat()), speed * std::cosf(TAU * randomFloat())};
	}
};

class bigParticle : public Particle {
public:
	bigParticle() {
		setPosition({0, 0});
		updateRadius();
		setFillColor(sf::Color::Red);
		setPointCount(30);
		speed = 0.0f;
		velocity = {0.0f, 0.0f};//set initial velocity
		path.addPoint({ 0,0 });
	}

	void updateRadius() {
		setOrigin({R, R});
		setRadius(R);
	}

	virtual void update() {
		move(velocity);
		staleVel = velocity;

		for (Particle& other : collidingObjects) {
			if (isCollision(other)) {
				sf::Vector2f momentum = staleVel * M;
				sf::Vector2f other_momentum = other.staleVel * m;
				//define V for the ZMF
				sf::Vector2f V = (momentum + other_momentum) / (M + m);
				this->velocity = V * (1.0f + C) - C * this->staleVel;
				//the small particls having their thing actually calculated leads to weird sticking, but you can credit it to the model.
				other.velocity = V * (1.0f + C) - C * other.staleVel;//speed is not a factor in the big one's movement,
				//other.speed=other.velocity.x/std::sinf(other.velocity.angle().asRadians());
			}
		}
		path.addPoint(getPosition());
	}
};

int main() {
	sf::RenderWindow window(sf::VideoMode({screenWidth, screenHeight}), "Brownian motion", sf::Style::Default);

	if (!ImGui::SFML::Init(window)) {
		std::cerr << "Could not initialise ImGui for SFML" << "\n";
		return 1;
	}
	path.newPath();
	bigParticle big;

	collidingObjects.resize(n);

	sf::View camera(sf::FloatRect({0, 0}, {(float)screenWidth, (float)screenHeight}));

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

		ImGui::SetNextWindowSize({0.0f, 0.0f});
		ImGui::Begin("Brownian Motion", nullptr, ImGuiWindowFlags_NoResize);

		ImGui::Text("N:");
		ImGui::SameLine();
		if (ImGui::SliderInt("##N", &n, 1, 200)) {
			collidingObjects.resize(n);
		}

		ImGui::Text("Mass of large particle:");
		ImGui::SameLine();
		ImGui::SliderFloat("##M", &M, 1.0f, 100.0f);

		ImGui::Text("Radius of large particle:");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##R", &R, 1.0f, 100.0f)) {
			big.updateRadius();
		}

		ImGui::Text("Mass of small particles:");
		ImGui::SameLine();
		ImGui::SliderFloat("##m", &m, 1.0f, 100.0f);

		ImGui::Text("Radius of small particles:");
		ImGui::SameLine();
		if (ImGui::SliderFloat("##r", &r, 1.0f, 100.0f)) {
			for (Particle& particle : collidingObjects) {
				particle.updateRadius();
			}
		}

		ImGui::Text("Coefficient of Restitution:");
		ImGui::SameLine();
		ImGui::SliderFloat("##C", &C, 0.0f, 1.0f);

		if (running || ImGui::Button("Step")) {
			for (Particle& p : collidingObjects) {
				p.update();
			}

			big.update();
		}

		if (ImGui::Button(running ? "Pause" : "Play")) {
			running = !running;
		}

		ImGui::End();

		window.setView(camera);
		window.draw(path);
		window.draw(big);

		for (const Particle& particle : collidingObjects) {
			window.draw(particle);
		}

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
}