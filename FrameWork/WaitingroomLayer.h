#pragma once

#include <iostream>
#include <SUNNY.h>

#include "../05_GAME/graphics/Model3D.h"
#include "../05_GAME/MousePicker.h"


using namespace std;
using namespace sunny;
using namespace graphics;
using namespace events;
using namespace ui;


using namespace directx;
using namespace game;
using namespace maths;


class WaitingroomLayer:public Layer2D
{
public:

	Sprite* m_logo;

	Label*  m_fps;

	Panel* m_model_panel;
	Label* model_position;
	Label* model_rotation;
	Label* model_scale;


	Panel* m_panel;

	std::vector<Layer*> m_layers;

	int m_layerIndex;

	Slider* m_slider1;
	Slider* m_slider2;

	Progressbar* m_progressbar;

public:
	WaitingroomLayer();
	~WaitingroomLayer();

	void OnInit(Renderer2D& renderer);
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnRender(Renderer2D& renderer);
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnKeyReleasedEvent(KeyReleasedEvent& event);
	bool OnMousePressedEvent(MousePressedEvent& event);
	bool OnMouseReleasedEvent(MouseReleasedEvent& event);
};

