#pragma once

class DrawImpl
{
public:
	DrawImpl();
	virtual ~DrawImpl();
	
	virtual void draw() = 0;
};

class ElementDraw : public DrawImpl
{
	GLsizei count;
	
public:
	ElementDraw(GLsizei count);
	~ElementDraw();
	
	void draw() override;
};