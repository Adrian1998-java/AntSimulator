#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "colony.hpp"
#include "config.hpp"
#include "display_manager.hpp"


int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(Conf<>::WIN_WIDTH, Conf<>::WIN_HEIGHT), "AntSim", sf::Style::Default, settings);
	window.setFramerateLimit(60);

	Conf<>::loadTextures();

	World world(Conf<>::WIN_WIDTH, Conf<>::WIN_HEIGHT);
	Colony colony(800, 450, 512);
	world.addMarker(Marker(colony.position, Marker::ToHome, 10.0f, true));
	
	DisplayManager display_manager(window, window, world, colony);

	sf::Vector2f last_clic;

	while (window.isOpen())
	{
		display_manager.processEvents();

		// Add food on clic
		if (display_manager.clic) {
			const sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
			const sf::Vector2f world_position = display_manager.displayCoordToWorldCoord(sf::Vector2f(to<float>(mouse_position.x), to<float>(mouse_position.y)));
			const float clic_min_dist = 2.0f;
			if (getLength(world_position - last_clic) > clic_min_dist) {
				world.addFoodAt(world_position.x, world_position.y, 5.0f);
				last_clic = world_position;
			}
		}

		const float dt = 0.016f;

		if (!display_manager.pause) {
			colony.update(dt, world);
			world.update(dt);
		}

		window.clear(sf::Color(94, 87, 87));
		
		display_manager.draw();

		window.display();
	}

	// Free textures
	Conf<>::freeTextures();

	return 0;
}