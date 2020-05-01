#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "ant.hpp"
#include "utils.hpp"
#include "world.hpp"


struct Colony
{
	Colony(float x, float y, uint32_t n)
		: position(x, y)
		, last_direction_update(0.0f)
		, va(sf::Triangles, 3 * n)
	{
		for (uint32_t i(n); i--;) {
			ants.emplace_back(x, y, getRandRange(2.0f * PI), n - i - 1);
		}
	}

	void update(const float dt, World& world)
	{
		/*const std::size_t ants_count = ants.size();
		
		last_direction_update += dt;
		if (last_direction_update > direction_update_period) {
			last_direction_update = 0.0f;
			const uint32_t ants_per_thread = ants_count / Conf<>::THREAD_COUNT;
			auto group = swarm.execute([&](uint32_t worker_id, uint32_t thread_count) {
				for (uint32_t i(worker_id * ants_per_thread); i < ((worker_id + 1) * ants_per_thread); ++i) {
					ants[i].findMarker(world);
				}
			});
			group.waitExecutionDone();
		}
		*/
		for (Ant& ant : ants) {
			ant.update(dt, world);
		}
	}

	void render(sf::RenderTarget& target, const sf::RenderStates& states) const
	{
		//for (const Ant& a : ants) {
		//	a.render(target, states);
		//}

		uint64_t index = 0;
		for (const Ant& a : ants) {
			a.render_in(va, 3 * index);
			++index;
		}

		const float size = Conf<>::COLONY_SIZE;
		sf::CircleShape circle(size);
		circle.setOrigin(size, size);
		circle.setPosition(position);
		circle.setFillColor(Conf<>::COLONY_COLOR);
		target.draw(circle, states);

		target.draw(va, states);
	}

	const sf::Vector2f position;
	std::vector<Ant> ants;

	float last_direction_update;
	const float direction_update_period = 0.25f;
	mutable sf::VertexArray va;

};
