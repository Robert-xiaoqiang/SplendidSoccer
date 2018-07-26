#include <Defencer_test.h>
#include <Soccer.h>

extern double eye[3];
extern double at[3];
extern Soccer s;
GLuint defencerList;
const double PI = 3.142;

Defencer::Defencer() :
	worldPos{ (float)eye[0], (float)eye[1], (float)eye[2] },
	defencerBody()
{

}
void Defencer::init()
{
	defencerBody.openFile("datas/newlego/lego.obj", 3, 2);
}

void Defencer::render()
{
	vec3 viewVector = { (float)(at[0] - eye[0]), (float)(at[1] - eye[1]), (float)(at[2] - eye[2]) };
	normalize(viewVector);
	//std::cout << "player" << worldPos.x << ' ' << worldPos.y << ' ' << worldPos.z << std::endl;
	worldPos = {
		(float)eye[0] + viewVector.x * 7.0f,
		0.0f,
		(float)eye[2] + viewVector.z * 7.0f
	};

	glPushMatrix();
	//glTranslated(worldPos.x, worldPos.y, worldPos.z);
	glTranslatef(0.0f, -2.0f, -4.0f);
	double theta = atan2(viewVector.z, abs(viewVector.x));
	glRotated(theta * 180.0 / PI, 0.0, 1.0, 0.0);
	//here player -> view -x
	glRotated(-90.0, 0.0, 1.0, 0.0);
	//glRotated(-90.0, 1.0, 0.0, 0.0);
	glScaled(0.2, 0.2, 0.2);
	glCallList(defencerList);
	glPopMatrix();
}