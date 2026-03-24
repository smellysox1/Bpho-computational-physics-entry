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

unsigned int M = 5, m = 5, R = 5, r = 5;
float C = 1;
unsigned int seed = std::chrono::steady_clock::now().time_since_epoch().count();

unsigned int screenWidth = 1280;
unsigned int screenHeight = 720;

bool running = false;

int main() {
	sf::RenderWindow window(sf::VideoMode({screenWidth, screenHeight}), "Brownian motion", sf::Style::Default);

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

		ImGui::SetNextWindowSize({0.0f, 0.0f});
		ImGui::Begin("Brownian Motion", nullptr, ImGuiWindowFlags_NoResize);

		ImGui::Text("Mass of large particle:");
		ImGui::SameLine();
		ImGui::SliderScalar("##M", ImGuiDataType_U32, &M, &M_MIN, &M_MAX);

		ImGui::Text("Radius of large particle:");
		ImGui::SameLine();
		ImGui::SliderScalar("##R", ImGuiDataType_U32, &R, &R_MIN, &R_MAX);

		ImGui::Text("Mass of small particles:");
		ImGui::SameLine();
		ImGui::SliderScalar("##m", ImGuiDataType_U32, &m, &m_MIN, &m_MAX);

		ImGui::Text("Radius of small particles:");
		ImGui::SameLine();
		ImGui::SliderScalar("##r", ImGuiDataType_U32, &r, &r_MIN, &r_MAX);

		ImGui::Text("Coefficient of Restitution:");
		ImGui::SameLine();
		ImGui::SliderFloat("##C", &C, C_MIN, C_MAX);

		if (running || ImGui::Button("Step")) {
			// update particles
		}

		if (ImGui::Button(running ? "Pause" : "Play")) {
			running = !running;
		}

		ImGui::End();

		window.setView(camera);

		// draw

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
}