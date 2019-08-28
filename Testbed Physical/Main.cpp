#include <SFML/Graphics.hpp>
#include <World/World.h>
using namespace yuh;

#define WIDTH 1000
#define HEIGHT 600
sf::Font font;


class ArrowLine : public sf::Drawable, public sf::Transformable {
public:
	ArrowLine() {
		const sf::Color color = sf::Color::Yellow;
		m_vao.setPrimitiveType(sf::Lines);
		m_vao.append(sf::Vertex({ 0.0f, 0.0f }, color));
		m_vao.append(sf::Vertex({ 1.0f, 0.0f }, color));
		m_vao.append(sf::Vertex({ 0.7f, 0.2f }, color));
		m_vao.append(sf::Vertex({ 1.0f, 0.0f }, color));
		m_vao.append(sf::Vertex({ 0.7f,-0.2f }, color));
		m_vao.append(sf::Vertex({ 1.0f, 0.0f }, color));
	}

	void Set(const sf::Vector2f& p, const sf::Vector2f& normal, float length) {
		const Vec2f xAxis(1, 0);
		float angle = VectorCorner({ normal.x, normal.y }, xAxis) * 180 / 3.14;
		setPosition(p);
		setScale(length, length > 10 ? 10 : length);
		setRotation(angle);
	}
	
private:
	/// sf::Drawable::draw override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= getTransform();
		target.draw(m_vao, states);
	}

	sf::VertexArray m_vao;
};


class DrawDebug : public physical::IDrawDebug {
public:
	DrawDebug(sf::RenderTarget* render, sf::Font* font) {
		m_render = render;
		m_font = font;

		m_circle.setRadius(1);
		m_circle.setOutlineColor(sf::Color::White);
		m_circle.setOutlineThickness(1);
		m_circle.setFillColor(sf::Color::Transparent);

		m_aabb.setFillColor(sf::Color::Transparent);
		m_aabb.setOutlineColor(sf::Color(0, 40, 0));
		m_aabb.setOutlineThickness(1);
	}

	void DrawCircle(const Vec2f& position, float radius) {
		m_circle.setRadius(radius);
		m_circle.setPosition({position.x, HEIGHT - position.y}); 
		m_circle.setOrigin(radius, radius);
		m_render->draw(m_circle);
	}

	void DrawAABB(const Vec2f& min, const Vec2f& max) {
		Vec2f size = max - min;
		m_aabb.setSize({ size.x, size.y });
		m_aabb.setPosition(min.x, HEIGHT - max.y);
		m_render->draw(m_aabb);
	}

	void DrawLine(const Vec2f& pA, const Vec2f& pB) {
		sf::VertexArray line;
		line.setPrimitiveType(sf::Lines);
		line.append(sf::Vertex({ pA.x, HEIGHT - pA.y }, sf::Color::Yellow));
		line.append(sf::Vertex({ pB.x, HEIGHT - pB.y }, sf::Color::Yellow));
		m_render->draw(line);
	}

	void DrawArrow(const Vec2f& p, const Vec2f& n, float length) {
		m_arrow.Set({ p.x, HEIGHT - p.y }, { n.x, n.y }, length);
		m_render->draw(m_arrow);
	}



private:
	sf::CircleShape		m_circle;
	sf::RectangleShape	m_aabb;
	ArrowLine			m_arrow;
	sf::RenderTarget*   m_render;
	sf::Font*			m_font;
};


class FPSCounter : public sf::Drawable, public sf::Transformable  {
public:
	FPSCounter(sf::Font *font) {
		m_counter = 0;
		m_draw.setFont(*font);
		m_draw.setCharacterSize(18);
		m_draw.setColor(sf::Color::Green);
	}

	void Update() {
		if (m_timer.getElapsedTime().asMilliseconds() > 999) {
			m_draw.setString("FPS: " + std::to_string(m_counter));
			m_counter = 0;
			m_timer.restart();
		}
		++m_counter;
	}

private:
	/// sf::Drawable::draw override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.transform *= this->getTransform();
		target.draw(m_draw, states);
	}

	unsigned int m_counter;
	sf::Text	 m_draw;
	sf::Clock	 m_timer;
};


class ContactListener : public physical::ContactListener {
public:
	void BeginContact(physical::Contact* ct) {
		std::cout << ct << " Begin\n";
	}

	void EndContact(physical::Contact* ct) {
		std::cout << ct << " End\n";
	}
};


int main()
{
	sf::ContextSettings setting;
	setting.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Testbed Physical", 7U, setting);
	window.setFramerateLimit(60);

	font.loadFromFile("UTM Dax.ttf");
	FPSCounter fps(&font);
	fps.setPosition(10, 10);

	const float positionIterations = 6;
	const float velocityIterations = 12;

	physical::World world(Vec2f(0, -9.8f));
	world.GetContactManager()->SetListener(new ContactListener());
	world.SetDrawDebug(new DrawDebug(&window, &font));
	
	physical::AABB clipScreen;
	clipScreen.max = { WIDTH, HEIGHT };

	for (int i = 0; i < 10; i++) {
		auto bd = world.CreateBody();
		bd->SetPosition({ 30.0f + i*60.0f, 100.0f });
		bd->SetType(physical::Body::b_Static);
		bd->m_mass = 0.0f;
		bd->m_invMass = 0.0f;

		auto cl = (physical::CircleShape*)bd->CreateCollider(physical::ICollider::c_Circle);
		cl->m_restitution = 0.1;
		cl->m_friction = 0.1;
		cl->m_density = 28;
		cl->SetRadius(30.0f);
		cl->SetLocalPosition({ 0, 0 });
	}

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
				bd->m_mass = 10.0f;
				bd->m_invMass = 0.1f;

				float rl = rand() % 20 + 20;
				auto cl = (physical::CircleShape*)bd->CreateCollider(physical::ICollider::c_Circle);
				cl->m_restitution = 0.1;
				cl->m_friction = 0.1;
				cl->m_density = 28;	/// 2d => kg/m^2 = m/S
				cl->SetRadius(rl);
				cl->SetLocalPosition({ 0, 0 });

				//float rm = rand() % 20 + 20;
				//auto cm = (physical::CircleShape*)bd->CreateCollider(physical::ICollider::c_Circle);
				//cm->m_restitution = 0.1;
				//cm->m_friction = 0.1;
				//cm->m_density = 28;	/// 2d => kg/m^2 = m/S
				//cm->SetRadius(rm);
				//cm->SetLocalPosition({ rm, 0 });
			}
		}

		fps.Update();
		world.Step(16, velocityIterations, positionIterations);


		window.clear();
		world.DrawDebug(clipScreen);
		window.draw(fps);
		window.display();
	}

	return 0;
}