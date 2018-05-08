#include "WaitingroomLayer.h"



WaitingroomLayer::WaitingroomLayer()
: Layer2D(maths::mat4::Orthographic(0.0f, Application::GetApplication().GetWindowWidth(), 0.0f, Application::GetApplication().GetWindowHeight(), -1.0f, 1.0f))
{
}


WaitingroomLayer::~WaitingroomLayer()
{
}

void WaitingroomLayer::OnInit(Renderer2D & renderer)
{
	int windowWidth = Application::GetApplication().GetWindowWidth();
	int windowHeight = Application::GetApplication().GetWindowHeight();



	m_panel = new Panel();

	Button* startButton = new Button("        START", maths::Rectangle(950, 70, 60, 50));
	Button* charator1 = new Button("        Attaker", maths::Rectangle(950, 600, 60, 50));
	Button* charator2 = new Button("        Defencer", maths::Rectangle(1150, 600, 60, 50));
	Button* charator3 = new Button("        Engineer", maths::Rectangle(750, 600, 60, 50));
	Button* map = new Button("        SkeletonVally", maths::Rectangle(950, 360, 200, 120));

	Button* player1 = new Button("		  YANG", maths::Rectangle(150, 600, 60, 100));
	Button* player2 = new Button("        KIM", maths::Rectangle(350, 600, 60, 100));
	Button* player3 = new Button("        JOE", maths::Rectangle(550, 600, 60, 100));

	Button* chatingBox = new Button("        CHATTTING", maths::Rectangle(350, 230, 250, 200));

	m_slider1 = new Slider(maths::Rectangle(20, 200, 10, 100), true);
	m_slider2 = new Slider(maths::Rectangle(640, 25, 300, 20), false);
	m_progressbar = new Progressbar(maths::Rectangle(640, 75, 300, 20));
	

	m_progressbar->SetCallback([](float) {
		//std::cout << "Complete" << std::endl;
	});

	Dialog* dialog = new Dialog(maths::Rectangle(1150, 500, 100, 100), "   Basic Dialog", "Test");

	dialog->SetCloseAction([&, dialog]() {
		m_panel->Remove(dialog);
	});

	m_panel->Add(startButton);
	m_panel->Add(charator1);
	m_panel->Add(charator2);
	m_panel->Add(charator3);
	m_panel->Add(player1);
	m_panel->Add(player2);
	m_panel->Add(player3);
	m_panel->Add(map);
	m_panel->Add(chatingBox);
	//m_panel->Add(m_slider1);
	//m_panel->Add(m_slider2);
	//m_panel->Add(dialog);
	//m_panel->Add(m_progressbar);
}

void WaitingroomLayer::OnTick()
{
}

void WaitingroomLayer::OnUpdate(const utils::Timestep & ts)
{
}

void WaitingroomLayer::OnRender(Renderer2D & renderer)
{
}

void WaitingroomLayer::OnEvent(Event & event)
{
	Layer2D::OnEvent(event);
	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<KeyPressedEvent>(METHOD(&WaitingroomLayer::OnKeyPressedEvent));
	//dispatcher.Dispatch<KeyReleasedEvent>(METHOD(&TestLayer3D::OnKeyReleasedEvent));
	dispatcher.Dispatch<MousePressedEvent>(METHOD(&WaitingroomLayer::OnMousePressedEvent));
	//dispatcher.Dispatch<MouseReleasedEvent>(METHOD(&TestLayer3D::OnMouseReleasedEvent));
}

bool WaitingroomLayer::OnKeyPressedEvent(KeyPressedEvent & event)
{


	return false;
}

bool WaitingroomLayer::OnMousePressedEvent(MousePressedEvent & event)
{
	std::cout << "pressed (" << event.GetX() << ", " << event.GetY() << ")" << std::endl;
	return false;
}

