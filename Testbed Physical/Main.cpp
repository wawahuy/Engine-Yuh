#include <SFML/Graphics.hpp>
#include <World/World.h>
using namespace yuh;

int main()
{
	const float positionIterations = 6;
	const float velocityIterations = 12;

	physical::World world(Vec2f(0, -9.8f));
	physical::Body* bs = world.CreateBody();
	physical::Body* be = world.CreateBody();

	physical::ICollider* ca = bs->CreateCollider(physical::ICollider::c_Circle);
	physical::ICollider* cb = bs->CreateCollider(physical::ICollider::c_Circle);



	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		world.Step(16, velocityIterations, positionIterations);



		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}