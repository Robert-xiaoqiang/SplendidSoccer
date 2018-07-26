
#include "Defencer.h"
#include <math.h>

int isShoot = 0; // 0/1 Press J
int shootDiredtion;
int isGoal;
double shootTime = 0;
double waitTime = 0;

extern GamingState _gamingCurrent;
extern Direction _direction;
extern Height _height;


const double PI = 3.142;

Defencer::Defencer() :
	defencerBody()
{

}
void Defencer::init()
{
	defencerBody.openFile("datas/cola/cola.obj", 3, 2);
	defencerList = glGenLists(1);
	glNewList(defencerList, GL_COMPILE);
	defencerBody.draw();
	glEndList();
}

void Defencer::render()
{
	double mood;		//乘数，用于改变跳跃和转动的方向
	int type = 0;		//-1/0/1 左中右
	int high = 0;		//0/1 高球还是低球
	int defenceOK = 1;
	double defencerAcceration;
	double yposition =17;
	double scaleda = 0.7, scaleb = 0.55, scalec = 0.45;
	double defencerSpeedY;
	double defencerTime;
	double t1 = 30, t2 = 60, t3 = 70, t4 = 120, t0 = 20;
	double g = 0;
	double height = 7;
	double g1 = 2 * height / t1 / t1;
	double height2 = 6.5;
	double g2 = 2 * height2 / (t2-t1) / (t2-t1);
	double x1 = 13.5;
	double vx = x1/t2;

	//Data for high ball
	double high_t1 = 60;
	double high_height = 16;
	double high_g = 2 * high_height / high_t1 / high_t1;
	double high_x1 = 17;

	//Data for middle ball
	double mid_t1 = 50, mid_t2 = 100, mid_t3 = 130, mid_t4 = 160;
	double mid_height = 17, mid_height1 = 10;
	
	double mid_g1 = 2 * mid_height1 / (mid_t3-mid_t2) / (mid_t3 - mid_t2);

	if (_direction == RIGHT)
	{
		mood = 1;
	}
	else if (_direction == LEFT)
	{
		if(_height == LOW)
			mood = 1;
		else if(_height == HIGH)
			mood = -1;
	}
	if (_gamingCurrent == AFTERKICK)
	{
		if (waitTime < t0)
		{
			waitTime += 0.5;	
			glPushMatrix();
			glTranslatef(yposition, - 62.5, 0);
			glScaled(scaleda, scaleb, scalec);
			glCallList(defencerList);
			std::cout << "watiTime = " << waitTime << std::endl;
			glPopMatrix();
		}
		if (waitTime > t0)
		{
			shootTime = 0;
		}
	}
	if (_gamingCurrent == BEFOREKICK)
	{
		waitTime = 0;
	}
	
	//往左侧或右侧的低球
	if (_height == LOW && _direction != MIDDLE && _gamingCurrent == AFTERKICK && waitTime >= t0)
	{
		if (waitTime >= t0) {
			if (shootTime <= t1)
			{
				glPushMatrix();

				//glTranslatef(60.0f, -35.0f, -28.0f);
				glTranslatef(yposition, sqrt(2 * g1 * height)*shootTime - 0.5 * g1 * shootTime*shootTime - 62.5, mood *(-x1 / t2 * shootTime));
				glRotated(mood*(-60 / t1 * shootTime - 20), 1, 0, 0);
				glScaled(scaleda, scaleb, scalec);
				glCallList(defencerList);
				std::cout << shootTime << std::endl;
				glPopMatrix();

				shootTime += 0.5;
			}
			else if (shootTime >= t1 && shootTime < t2)
			{
				double delta = shootTime - t1;
				glPushMatrix();

				//glTranslatef(60.0f, -35.0f, -28.0f);

				glTranslatef(yposition, (height - 0.5*g2*delta*delta) - 62.5, mood*(-x1 / t2 * shootTime));
				glRotated(mood*(-20 / (t2 - t1) * (shootTime - t1) - 80), 1, 0, 0);
				glScaled(scaleda, scaleb, scalec);
				glCallList(defencerList);
				std::cout << shootTime << std::endl;
				glPopMatrix();

				shootTime += 0.5;
			}
			else if (shootTime >= t2 && shootTime < t3)
			{
				glPushMatrix();
				double delta = shootTime - t2;
				glTranslatef(yposition, 0 - 63, mood*(-x1 - (vx*delta - 0.5*vx / (t4 - t2)*delta*delta)));
				glRotated(mood*(-100 + 10 / (t3 - t2) * delta), 1, 0, 0);
				glScaled(scaleda, scaleb, scalec);
				glCallList(defencerList);
				std::cout << shootTime << std::endl;
				glPopMatrix();

				shootTime += 0.5;
			}
			else if (shootTime >= t3 && shootTime < t4)
			{
				glPushMatrix();
				double delta = shootTime - t2;
				glTranslatef(yposition, 0 - 63, mood*(-x1 - (vx*delta - 0.5*vx / (t4 - t2)*delta*delta)));
				glRotated(mood*(-90), 1, 0, 0);
				glScaled(scaleda, scaleb, scalec);
				glCallList(defencerList);
				std::cout << shootTime << std::endl;
				glPopMatrix();

				shootTime += 0.5;
			}
			else if (shootTime == t4)
			{
				shootTime = 0;
				//waitTime = 0;
			}
		}
	}
	
	if (_height == HIGH && _direction != MIDDLE && _gamingCurrent == AFTERKICK && waitTime >= t0)
	{
		
		if (waitTime >= t0) {
			if (shootTime < high_t1)
			{
				glPushMatrix();
				double delta = shootTime - 0;
				glTranslatef(yposition, -63 + (high_t1*high_g)*delta - 0.5*high_g*delta*delta, mood*(-high_x1 / high_t1 * delta));
				glRotated(mood*(-30 - 35 / high_t1 * delta), 1, 0, 0);
				glScaled(scaleda, scaleb, scalec);
				glCallList(defencerList);
				std::cout << shootTime << std::endl;
				glPopMatrix();
				shootTime += 0.5;
			}

			else if (shootTime >= high_t1)
			{
				shootTime = 0.0;
				//waitTime = 0;
			}
		}
	}
	if (_direction == MIDDLE || _gamingCurrent == BEFOREKICK)
	{
		
		if (waitTime < t0 && _gamingCurrent == AFTERKICK)
		{
			waitTime += 0.5;
			glPushMatrix();
			glScaled(scaleda, scaleb, scalec);
			glCallList(defencerList);
			glPopMatrix();
		}
		if (waitTime >= t0 || _gamingCurrent == BEFOREKICK) {
			
			double mid_g = 2 * mid_height / mid_t1 / mid_t1;
			if (shootTime < mid_t1)
			{
				glPushMatrix();
				double delta = shootTime - 0;
				glTranslatef(yposition, -63 + (mid_t1*mid_g)*delta - 0.5*mid_g*delta*delta, 0);
				glScaled(scaleda, scaleb, scalec);
				glCallList(defencerList);
				std::cout << shootTime << std::endl;
				glPopMatrix();
				shootTime += 0.5;
			}
			else if (shootTime >= mid_t1 && shootTime < mid_t2)
			{
				glPushMatrix();
				double delta = shootTime - mid_t1;
				glTranslatef(yposition, -63 + mid_height - 0.5*mid_g*delta*delta, 0);
				glScaled(scaleda, scaleb, scalec);
				glCallList(defencerList);
				std::cout << shootTime << std::endl;
				glPopMatrix();
				shootTime += 0.5;
			}
			else if (shootTime >= mid_t1)
			{
				shootTime = 0;
				//waitTime = 0;
				glPushMatrix();
				glScaled(scaleda, scaleb, scalec);
				glCallList(defencerList);
				glPopMatrix();
			}
		}
	}
	//glCallList(defencerList);
	
}

