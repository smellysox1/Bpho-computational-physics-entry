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
std::vector<particle> collidingObjects;
unsigned int n = 11;
unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count();

unsigned int screenWidth = 1280;
unsigned int screenHeight = 720;

std::mt19937 rng{ static_cast<std::mt19937::result_type>(seed) };//just a random seed.


float randomFloat(std::mt19937 rng) {
	static constexpr const float X = rng.max();
	return rng() / X;
}

class particle : sf::CircleShape {
public:
	float mass;
	sf::Vector2f staleVel;
	float speed;
	sf::Vector2f velocity;
	unsigned int uid = std::chrono::steady_clock::now().time_since_epoch().count()-seed;//the uid makes each particle unique.
	virtual bool is_big() {
		return false;
	}
	particle(float radius, float mass) {
		setRadius(radius);
		setFillColor(sf::Color::Green);
		setPointCount(30);
		this->mass = mass;
		this->speed = 1;
		velocity = { speed * std::sin(TAU * randomFloat(rng)),speed * std::cos(TAU * randomFloat(rng)) };//set initial velocity
	}
	bool insideParticle(sf::Vector2f point){//chekcs for if the pos is within the circle
		sf::Vector2f dist_from_rad = point - getPosition();
		if (dist_from_rad.length() <= getRadius()) {
			return true;
		}
		return false;
	}
	bool isCollision(particle other) {//chekcs for if the pos is within the circle
		sf::Vector2f dist_from_rad = other.getPosition() - this->getPosition();
		if (dist_from_rad.length() <= (this->getRadius()+other.getRadius())) {
			return true;
		}
		return false;
	}
	void update_pos() {//here because of the stupid access of sfml
		setPosition(getPosition() + velocity);
	}
	virtual void update() {
		update_pos();
		staleVel = velocity;
		bool velDone = 0;
		velocity = { speed * std::sin(TAU * randomFloat(rng)),speed * std::cos(TAU * randomFloat(rng)) };
	}
};

class bigParticle:particle {
	virtual void update() {
		update_pos();
		staleVel = velocity;
		bool velDone = 0;
		for (particle other : collidingObjects) {
			if (isCollision(other)) {
				sf::Vector2f momentum = staleVel * mass;
				sf::Vector2f other_momentum = other.staleVel * other.mass;
				//define V for the ZMF
				sf::Vector2f V = (momentum + other_momentum) / (mass + other.mass);
				this->velocity = V * float(1.0 + coefficient_of_restitution) - coefficient_of_restitution * this->staleVel;
				other.velocity = V * float(1.0 + coefficient_of_restitution) - coefficient_of_restitution * other.staleVel;
				velDone = 1;
				this->speed = this->velocity.x / std::sin(this->velocity.angle().asRadians());
				other.speed=other.velocity.x/std::sin(other.velocity.angle().asRadians());
			}
		}//if no collision, just random walk again.
		if (!velDone) {
			velocity = { speed * std::sin(TAU * randomFloat(rng)),speed * std::cos(TAU * randomFloat(rng)) };
			//if velocity goes unupdated, the 
		}

	}
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

		ImGui::SetNextWindowSize({300.0f, 200.0f});
		ImGui::Begin("Radnom walk", nullptr, ImGuiWindowFlags_NoResize);

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