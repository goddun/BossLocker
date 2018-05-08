#pragma once
#include "../05_GAME/shoot/BulletSystem.h"


class ShootingLayer:public Layer3D
{
public:
	ShootingLayer();
	~ShootingLayer();

	void OnInit(Renderer3D& renderer) override;
	void OnTick() override;
	void OnUpdate(const utils::Timestep& ts) override;
	void OnEvent(Event& event) override;

	bool OnKeyPressedEvent(KeyPressedEvent& event);
	bool OnKeyReleasedEvent(KeyReleasedEvent& event);
	bool OnMousePressedEvent(MousePressedEvent& event);
	bool OnMouseReleasedEvent(MouseReleasedEvent& event);

private:
	Model* m_sphere = nullptr;
	vector<BulletSystem*> m_bulletSystemList;
};

