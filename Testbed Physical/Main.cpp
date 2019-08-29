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

	void setColor(const sf::Color& color) {
		int s = m_vao.getVertexCount();
		for (int i = 0; i < s; i++) {
			m_vao[i].color = color;
		}
	}

	void Set(const sf::Vector2f& p, const sf::Vector2f& normal, float length) {
		Vec2f axisX = Vec2f( normal.x, normal.y );
		Vec2f axisY = { -axisX.y,  axisX.x};

		axisX *= length;
		axisY *= length < 6 ? 2 : length / 3;

		/// Ma trận hàng
		float* matrix = (float*)this->getTransform().getMatrix();
		
		/// i = [ 0(x),  1(y), 2.., 3..]
		/// j = [ 4(x),  5(y), 6.., 7..]
		/// w = [12(x), 13(y), ........]
		matrix[0] =  axisX.x;
		matrix[1] =  axisX.y;
		matrix[4] =  axisY.x;
		matrix[5] =  axisY.y;
		matrix[12] = p.x;
		matrix[13] = p.y;
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

		m_point.setFillColor(sf::Color::Blue);
		m_point.setSize({4, 4});
		m_point.setOrigin({ 2,2 });

		float* matrix = (float*)m_coord.getMatrix();
		matrix[5] = -1;
		matrix[13] = HEIGHT;
	}

	void DrawCircle(const Vec2f& position, float radius) {
		m_circle.setRadius(radius);
		m_circle.setPosition({position.x, position.y}); 
		m_circle.setOrigin(radius, radius);
		m_render->draw(m_circle, m_coord);
	}

	void DrawAABB(const Vec2f& min, const Vec2f& max) {
		Vec2f size = max - min;
		m_aabb.setSize({ size.x, size.y });
		m_aabb.setPosition(min.x, min.y);
		m_render->draw(m_aabb, m_coord);
	}

	void DrawLine(const Vec2f& pA, const Vec2f& pB) {
		sf::VertexArray line;
		line.setPrimitiveType(sf::Lines);
		line.append(sf::Vertex({ pA.x, pA.y }, sf::Color::Cyan));
		line.append(sf::Vertex({ pB.x, pB.y }, sf::Color::Cyan));
		m_render->draw(line, m_coord);
	}

	void DrawArrow(const Vec2f& p, const Vec2f& n, float length) {
		m_arrow.Set({ p.x, p.y }, { n.x, n.y }, length);
		m_render->draw(m_arrow, m_coord);
	}

	void DrawPoint(const Vec2f& p) {
		m_point.setPosition({ p.x, p.y });
		m_render->draw(m_point, m_coord);
	}

	void SetColor(int colorI) {
		sf::Color color;

		switch (colorI)
		{
		case 1:
			color = sf::Color::Yellow;
			m_arrow.setColor(color);
			break;

		case 2:
			color = sf::Color(0, 40, 0);
			m_aabb.setOutlineColor(color);
			break;

		case 3:
			color = sf::Color::Red;
			m_arrow.setColor(color);
			break;

		case 4:
			color = sf::Color::Blue;
			m_point.setFillColor(color);
			break;

		default:
			color = sf::Color::White;
			m_circle.setOutlineColor(color);
			break;
		}

	}

private:
	sf::CircleShape		m_circle;
	sf::RectangleShape	m_aabb;
	sf::RectangleShape	m_point;
	ArrowLine			m_arrow;
	sf::RenderTarget*   m_render;
	sf::Font*			m_font;
	sf::Transform		m_coord;
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
		/// std::cout << ct << " Begin\n";
	}

	void EndContact(physical::Contact* ct) {
		/// std::cout << ct << " End\n";
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

	DrawDebug debug(&window, &font);

	physical::World world(Vec2f(0, -9.8f));
	world.GetContactManager()->SetListener(new ContactListener());
	world.SetDrawDebug(&debug);
	
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

			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code)
				{
				case sf::Keyboard::P:
					debug.active_AABBDynamic = !debug.active_AABBDynamic;
					break;

				case sf::Keyboard::O:
					debug.active_NormalContact = !debug.active_NormalContact;
					break;

				case sf::Keyboard::I:
					debug.active_PointContact = !debug.active_PointContact;
					break;

				case sf::Keyboard::U:
					debug.active_Velocity = !debug.active_Velocity;
					break;
				}
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