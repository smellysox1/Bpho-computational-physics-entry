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

#include "constants.hpp"

// PARAMETERS

float R = 1.0f, r = 0.5f;
float m = 9.1*std::pow(10,-25);//electron mass.
//r=photon R=electromn rad
float W = 4.7f;
float current = 0.0f;
float wavelength = 200.0f;//this is in nanometers
float Energy = 1.6f;
float time_scale_factor = 1;
float intensity = 100;//in percent

unsigned int screenWidth = 1280;
unsigned int screenHeight = 720;
unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count();

bool running = false;
int i = 0;
float emitter_slower_factor=1.0f;//emitting 1 every 1ns

std::mt19937 rng{ seed };

float randomFloat() {
	static constexpr const float X = rng.max();
	return rng() / X;
}

class Particle;
class Photon;

std::vector<Particle> electrons;
std::vector<Photon> photons;

class Detector :public sf::RectangleShape {
public:
	Detector() : RectangleShape({ 5,100 }) {
		setFillColor(sf::Color::White);
		setPosition({ 25,20 });
		setRotation(sf::radians(0));
	}
};
class Emitter :public Detector {
public:
	Emitter() :Detector() {
		setPosition({ 500,500 });
		setRotation(sf::radians(TAU/8));
	}
	
	void update() {
		if ((i*time_scale_factor) > emitter_slower_factor ) {
			i = 0;
			photons.emplace_back();
		}
		i++;
	}
};



Detector detector;
Emitter emitter;
Detector plate2;


class Particle : public sf::CircleShape {
public:
	sf::Vector2f velocity;

	sf::Vector2f staleVel;
	bool deletionScheduled = 0;
	float speed;

	Particle() {
		float angle = TAU / 4;
		speed = 1.0f;
		velocity = { speed * std::sinf(angle), speed * std::cosf(angle) };
		//setPosition({20,30});

		setPointCount(30);
		updateRadius();

		setFillColor(sf::Color::Blue);
		
		
	}

	Particle(sf::Vector2f pos) : sf::CircleShape(R, 30) {
		float angle = TAU/4;
		speed = 1.0f;
		velocity = { speed * std::sinf(angle), speed * std::cosf(angle) };
		setPosition(pos);

		setPointCount(30);
		updateRadius();

		setFillColor(sf::Color::Blue);
	}

	void updateRadius() {
		setRadius(R);
		setOrigin({R, R});
	}

	bool insideParticle(sf::Vector2f point){//chekcs for if the pos is within the circle
		sf::Vector2f dist_from_rad = point - getPosition();
		return dist_from_rad.length() <= r;
	}

	
	

	virtual void update() {
		move(velocity*time_scale_factor);
		if (plate2.getGlobalBounds().contains(getPosition())) {
			deletionScheduled = 1;
			
			

		}
	}
};

class Photon : public Particle {
public:
	sf::Vector2f velocity;

	sf::Vector2f staleVel;

	float speed;//should always be faster than electron
	bool deletionScheduled = 0;


	Photon(sf::Vector2f pos) : Particle(pos) {
		float angle = TAU / 8;//update angle such that it is perp to emitter.
		speed = 3.0f;
		velocity = { speed * std::sinf(angle), speed * std::cosf(angle) };
		setPosition(pos);

		setPointCount(30);
		updateRadius();

		setFillColor(sf::Color::Magenta);
	}
	Photon() {
		float angle = 5*TAU / 8;//update angle such that it is perp to emitter.
		speed = 0.3f;//0.3m in 1ns
		velocity = { speed * std::sinf(angle), speed * std::cosf(angle) };
		float pos_on_line = randomFloat() * 100;
		setPosition({ 500 - pos_on_line * cosf(TAU / 8),500 + pos_on_line * sinf(TAU / 8) });//preferably somewhere random on the emitter -- 

		setPointCount(30);
		updateRadius();

		setFillColor(sf::Color::Magenta);
	}

	void updateRadius() {
		setRadius(r);
		setOrigin({ r, r });
	}

	virtual void update() {
		move(velocity * time_scale_factor);
		
		if ((detector.getGlobalBounds().contains(getPosition()) && getPosition().y < 115) ){// to filter out things that hit the bottom edge.
			deletionScheduled = 1;
			float electronSpeed = sqrt(2 * ((Energy - W) * 1.602 * pow(10, -13)) / m) * pow(10, -9);//sqrt(2*(Energy-W)/m) is in ms^-1 with kg we need in m/ns
			if (electronSpeed > 0) {
				electrons.emplace_back();
				electrons[electrons.size() - 1].speed = electronSpeed;//energy=energy-w
				electrons[electrons.size() - 1].velocity = { electronSpeed,0 };
				electrons[electrons.size() - 1].setPosition({ 30,getPosition().y - float(getPosition().x - 30) / tanf(5 * TAU / 8) });
			}
		}
		else if (((getPosition() - detector.getPosition()).angleTo({ velocity }) > sf::radians(3 * TAU / 8)) && getPosition().x < 20 && ((getPosition() - detector.getPosition()).angleTo({ velocity })) < sf::degrees(10)) {
			deletionScheduled = 1;
			float electronSpeed = sqrt(2 * ((Energy - W) * 1.602 * pow(10, -13)) / m) * pow(10, -9);//sqrt(2*(Energy-W)/m) is in ms^-1 with kg we need in m/ns
			if (electronSpeed > 0) {
				electrons.emplace_back();
				electrons[electrons.size() - 1].speed = electronSpeed;//energy=energy-w
				electrons[electrons.size() - 1].velocity = { electronSpeed,0 };

				electrons[electrons.size() - 1].setPosition({ 30,getPosition().y - float(getPosition().x - 30) / tanf(5 * TAU / 8) });//electron speed but subtract the amount increasedin themeantime
			}
		}

	}
};


int main() {
	current = 0.0f;
	plate2.setPosition({200,20});
	sf::RenderWindow window(sf::VideoMode({screenWidth, screenHeight}), "Photoelectric Simulation.", sf::Style::Default);

	if (!ImGui::SFML::Init(window)) {
		std::cerr << "Could not initialise ImGui for SFML" << "\n";
		return 1;
	}
	

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


		ImGui::SetNextWindowSize({ 0.0f, 0.0f });
		ImGui::Begin("PhotoElectric Effect", nullptr, ImGuiWindowFlags_NoResize);

		ImGui::Text("Unit of time is nanoseconds, unit of distance is meters, unit of mass is milligrams  (size of particles inaccurate):");

		ImGui::Text("Cohesion of the electrons to the metal plate (W) (eV):");
		ImGui::SameLine();
		ImGui::SliderFloat("##W", &W, 0.0f, 5.0f);

		ImGui::Text("wavelength of light (nm):");
		ImGui::SameLine();
		ImGui::SliderFloat("##lambda", &wavelength, 1.0f, 1000.0f);

		ImGui::Text("intensity of light (arbitrary units):");
		ImGui::SameLine();
		ImGui::SliderFloat("##intensity", &intensity, 0.0f, 100.0f);

		
		ImGui::Text("current (predicted):");
		ImGui::SameLine();
		ImGui::Text(std::to_string(current).c_str());

		ImGui::Text("Energy of each particle:");
		ImGui::SameLine();
		ImGui::Text(std::to_string(Energy).c_str());

		ImGui::Text("flow of time per frame (ns):");
		ImGui::SameLine();
		ImGui::SliderFloat("##timeflow", &time_scale_factor, 1.0f, 25.0f);
		
		if (running || ImGui::Button("Step")) {
			Energy = 413.6f * 3.0f / (wavelength);
			emitter_slower_factor = (100/intensity);
			float sum_electrons = 0.0f;
			for (Particle& p : electrons) {
				// current = the number of electrons times their speed times the electron charge over their distance
				p.update();
				sum_electrons += p.speed;
			}
			current = sum_electrons / 180 *1.6 / pow(10, 10); //1.6 to -19 to convert electron to coulomb, 
			for (Photon& ph : photons) {
				ph.update();
			}
			emitter.update();
			
		}

		if (ImGui::Button(running ? "Pause" : "Play")) {
			running = !running;
		}

		ImGui::End();

		window.setView(camera);
		
		window.draw(detector);
		window.draw(emitter);
		window.draw(plate2);
		for (auto iterator = electrons.begin(); iterator != electrons.end();) {
			Particle particle = *iterator;
			window.draw(particle);
			if (particle.deletionScheduled) {
				iterator = electrons.erase(iterator);
			}
			iterator++;
		}
		for (auto iterator = photons.begin(); iterator != photons.end();) {
			Photon pharticle = *iterator;
			window.draw(pharticle);
			if (iterator->deletionScheduled) {
				
				iterator=photons.erase(iterator);
			}
			if (iterator != photons.end()) {
				iterator++;
			}
		}

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
}