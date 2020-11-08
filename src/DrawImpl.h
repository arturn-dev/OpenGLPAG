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
	const void* indices;
	
public:
	ElementDraw(GLsizei count, const void* indices);
	~ElementDraw();
	
	void draw() override;
};