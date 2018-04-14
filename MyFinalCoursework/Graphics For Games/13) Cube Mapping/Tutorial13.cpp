#pragma comment(lib, "nclgl.lib")

#include "../../NCLGL/window.h"
#include "Renderer.h"
#include "ShadowRenderer.h"

float fps;

int scene;

int main() {
	scene = 1;
	Window w("Coursework Scene1", 1920, 1080, true);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);//This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
						 //ShadowRenderer shadowrenderer(w);
	//Renderer renderer1(w);

	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	float refreshtime = 0;

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		float updatevalue = w.GetTimer()->GetTimedMS();
		if (scene == 1) {
			float passedTime = w.GetTimer()->GetMS();
			refreshtime++;
			fps = passedTime / refreshtime;
			renderer.UpdateScene1(updatevalue);
			//renderer.UpdateScene2(updatevalue);
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_0)) {
				renderer.postEffect = 0;
			}

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
				renderer.postEffect = 1;
			}

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_2)) {
				renderer.postEffect = 2;
			}

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_3)) {
				renderer.postEffect = 3;
			}

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_4)) {
				renderer.postEffect = 4;
			}

			if (Window::GetKeyboard()->KeyDown(KEYBOARD_5)) {
				renderer.postEffect = 5;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD4)) {
				renderer.MoveLight = 1;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD6)) {
				renderer.MoveLight = 2;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD8)) {
				renderer.MoveLight = 3;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD2)) {
				renderer.MoveLight = 4;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) {
				scene = 2;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) {
				renderer.showScene3 = 0;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
				renderer.showScene3 = 2;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_PLUS)) {
				renderer.showScene3 = 0;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_MINUS)) {
				renderer.showScene3 = 1;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_F1)) {
				renderer.lightNum = 1;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_F2)) {
				renderer.lightNum = 2;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_F3)) {
				renderer.lightNum = 3;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_F4)) {
				renderer.lightNum = 4;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
				renderer.rotateCamera = !renderer.rotateCamera;
			}
			renderer.RenderScene1(fps);
		}
		else if (scene == 2) {
			renderer.UpdateScene2(updatevalue);
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_C)) {
				renderer.changeAnim = true;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD4)) {
				renderer.MoveLight2 = 1;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_NUMPAD6)) {
				renderer.MoveLight2 = 2;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) {
				scene = 1;
				renderer.showScene3 = 0;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
				scene = 1;
				renderer.showScene3 = 2;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
				renderer.rotateCamera = !renderer.rotateCamera;
			}
			if (Window::GetKeyboard()->KeyDown(KEYBOARD_L)) {
				renderer.drawLaser = !renderer.drawLaser;
			}
			renderer.RenderScene2();
		}
	}
	return 0;
}