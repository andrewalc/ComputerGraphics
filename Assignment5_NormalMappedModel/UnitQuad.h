#pragma once

#include <math.h>
#include <iostream>
#include "Renderable.h"

class UnitQuad : public Renderable
{
protected:
	QVector3D lightPos_;
	float sign_;
public:
	UnitQuad();
	virtual ~UnitQuad();

	// Our init method is much easier now.  We only need a texture!
	virtual void update(const qint64 msSinceLastFrame) override;

private:

};