#include <SFML/Graphics.hpp>
#include <World/World.h>
using namespace yuh;

#define WIDTH 1000
#define HEIGHT 600

class ContactListener : public physical::ContactListener {
public:
	void BeginContact(physical::Contact* ct) {
		std::cout << ct << " Begin\n";
	}

	void EndContact(physical::Contact* ct) {
		std::cout << ct << " End\n";
	}
};


class DrawDebug : public physical::IDrawDebug {
public:
	DrawDebug(sf::RenderTarget* render) {
		m_render = render;
		m_circle.setRadius(1);
		m_circle.setOutlineColor(sf::Color::White);
		m_circle.setOutlineThickness(1);
		m_circle.setFillColor(sf::Color::Transparent);
	}

	void DrawCircle(Vec2f position, float radius) {
		m_circle.setRadius(radius);
		m_circle.setPosition({position.x, HEIGHT - position.y}); 
		m_circle.setOrigin(radius, radius);
		m_render->draw(m_circle);
	}

	void DrawAABB(Vec2f min, Vec2f max) {

	}


private:
	sf::CircleShape		m_circle;
	sf::RectangleShape	m_aabb;
	sf::RenderTarget*   m_render;
};


int main()
{
	sf::ContextSettings setting;
	setting.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Testbed Physical!", 7U, setting);
	window.setFramerateLimit(60);

	const float positionIterations = 6;
	const float velocityIterations = 12;

	physical::World world(Vec2f(0, -9.8f));
	world.GetContactManager()->SetListener(new ContactListener());
	world.SetDrawDebug(new DrawDebug(&window));
	
	physical::AABB clipScreen;
	clipScreen.max = { WIDTH, HEIGHT };

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed) {
				sf::Vector2i pCursor = sf::Mouse::getPosition(window);

				auto bd = world.CreateBody();
				bd->SetPosition({ (float)pCursor.x, (float)HEIGHT - pCursor.y });

				auto cl = (physical::CircleShape*)bd->CreateCollider(physical::ICollider::c_Circle);
				cl->m_restitution = 0.1;
				cl->m_mass = 10;
				cl->m_inv_mass = 0.1;
				cl->SetRadius(rand() % 20 + 20);
				cl->SetLocalPosition({ 0, 0 });
			}
		}

		world.Step(16, velocityIterations, positionIterations);


		window.clear();
		world.DrawDebug(clipScreen);
		window.display();
	}

	return 0;
}