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
		m_circle_radius.setSize({ 1, 1 });
		m_circle_radius.setFillColor(sf::Color::White);

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

	void DrawCircle(const Vec2f& position, float radius, float orient) {
		m_circle.setRadius(radius);
		m_circle.setPosition({position.x, position.y}); 
		m_circle.setOrigin(radius, radius);
		m_render->draw(m_circle, m_coord);

		m_circle_radius.setScale({ radius, 1 });
		m_circle_radius.setRotation(orient*180.0f/3.14f);
		m_circle_radius.setPosition({ position.x, position.y });
		m_render->draw(m_circle_radius, m_coord);
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

		case 5:
			color = sf::Color::Magenta;
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
	sf::RectangleShape  m_circle_radius;
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

class DrawDebugDeltail : public sf::Drawable, public sf::Transformable {
public:

	const int NUM_BODIES = 0;
	const int NUM_BROADPHASE_NODE = 1;
	const int HEIGHT_BROADPHASE = 2;
	const int NUM_CONTACT = 3;

	struct LineString {
		sf::Text title;
		sf::Text value;
	};

	DrawDebugDeltail(sf::Font* font, physical::World* world) {
		m_font = font;
		m_world = world;
		m_size = 0;

		AddTitle(NUM_BODIES, "Num Bodies");
		AddTitle(NUM_BROADPHASE_NODE, "BP. Num NODE");
		AddTitle(HEIGHT_BROADPHASE, "BP. Height");
		AddTitle(NUM_CONTACT, "Num Contact");
	}


	void AddTitle(int key, std::string title) {
		m_list.insert(std::make_pair(key, LineString()));
		m_list[key].title.setFont(*m_font);
		m_list[key].title.setString(title + ":");
		m_list[key].title.setCharacterSize(12);
		m_list[key].title.setPosition({0.0f, m_size*10.0f});

		m_list[key].value.setFont(*m_font);
		m_list[key].value.setCharacterSize(12);
		m_list[key].value.setPosition({ 50.0f , m_size * 10.0f });

		m_size++;
	}

private:
	/// sf::Drawable::draw override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto p : m_list) {

			if (p.first == NUM_BODIES) {
				p.second.value.setString(std::to_string(m_world->GetNumBodies()));
			}
			else if (p.first == NUM_BROADPHASE_NODE) {
				p.second.value.setString(std::to_string(m_world->GetContactManager()->GetBroadphase()->GetNumNode()));
			}
			else if (p.first == HEIGHT_BROADPHASE) {
				p.second.value.setString(std::to_string(m_world->GetContactManager()->GetBroadphase()->GetHeight()));
			}
			else if (p.first == NUM_CONTACT) {
				p.second.value.setString(std::to_string(m_world->GetContactManager()->GetNumContact()));
			}

			target.draw(p.second.title, p.second.title.getTransform()* getTransform());
			target.draw(p.second.value, p.second.value.getTransform()* getTransform());
		}
	}

	size_t m_size;
	sf::Font* m_font;
	physical::World* m_world;
	std::map<int, LineString> m_list;
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

void CreateBody(physical::World& world, const Vec2f& pos) {
	auto bd = world.CreateBody();
	bd->SetPosition(pos);

	float rl = rand() % 10 + 10;
	auto cl = (physical::CircleShape*)bd->CreateCollider(physical::ICollider::c_Circle);
	cl->SetDensity(0.01f);
	cl->SetRestitution(0.1f);
	cl->SetFriction(0.1f);
	cl->SetRadius(rl);
	cl->SetLocalPosition({ -rl, 0 });

	float rm = rand() % 10 + 10;
	auto cm = (physical::CircleShape*)bd->CreateCollider(physical::ICollider::c_Circle);
	cm->SetDensity(0.01f);
	cm->SetRestitution(0.1f);
	cm->SetFriction(0.1f);
	cm->SetRadius(rm);
	cm->SetLocalPosition({ rm, 0 });

	float rv = rand() % 10 + 10;
	auto cv = (physical::CircleShape*)bd->CreateCollider(physical::ICollider::c_Circle);
	cv->SetDensity(0.01f);
	cv->SetRestitution(0.1f);
	cv->SetFriction(0.1f);
	cv->SetRadius(rv);
	cv->SetLocalPosition({ 0, -rv });

	float rb = rand() % 10 + 10;
	auto cb = (physical::CircleShape*)bd->CreateCollider(physical::ICollider::c_Circle);
	cb->SetDensity(0.01f);
	cb->SetRestitution(0.1f);
	cb->SetFriction(0.1f);
	cb->SetRadius(rv);
	cb->SetLocalPosition({ 0, rb });

	bd->ResetDataMass();
}

int main()
{
	/// Windows
	sf::ContextSettings setting;
	setting.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Testbed Physical", 7U, setting);
	window.setFramerateLimit(60);
	font.loadFromFile("UTM Dax.ttf");

	/// Physical
	const float positionIterations = 6;
	const float velocityIterations = 12;
	physical::World world(Vec2f(0, -9.8f));
	world.GetContactManager()->SetListener(new ContactListener());

	/// Draw Debug
	bool debugAdd = false;
	bool debugDraw = true;

	DrawDebugDeltail debugDeltail(&font, &world);
	debugDeltail.setPosition(800, 20);

	DrawDebug debug(&window, &font);
	world.SetDrawDebug(&debug);

	FPSCounter fps(&font);
	fps.setPosition(10, 10);

	/// Clip screen
	physical::AABB clipScreen;
	clipScreen.max = { WIDTH, HEIGHT };

	for (int i = 0; i < 10; i++) {
		auto bd = world.CreateBody();
		bd->SetPosition({ 30.0f + i*60.0f, 100.0f });
		bd->SetType(physical::Body::b_Static);

		auto cl = (physical::CircleShape*)bd->CreateCollider(physical::ICollider::c_Circle);
		cl->SetDensity(1.0f);
		cl->SetRestitution(0.1f);
		cl->SetFriction(0.1f);
		cl->SetRadius(30.0f);
		cl->SetLocalPosition({ 0, 0 });

		bd->ResetDataMass();
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
				CreateBody(world, { (float)pCursor.x, (float)HEIGHT - pCursor.y });
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

				case sf::Keyboard::Q:
					debugAdd = !debugAdd;
					break;

				case sf::Keyboard::W:
					debugDraw = !debugDraw;
					break;

				}
			}
		} 

		if (debugAdd) {
			CreateBody(world, {(float)(rand()%WIDTH), (float)(rand()%200 + HEIGHT)});
		}

		fps.Update();
		world.Step(16, velocityIterations, positionIterations);


		window.clear();
		
		if(debugDraw)
			world.DrawDebug(clipScreen);

		window.draw(debugDeltail);
		window.draw(fps);
		window.display();
	}

	return 0;
}