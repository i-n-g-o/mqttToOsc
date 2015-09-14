#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
	
	ofGLFWWindowSettings settings;
	
	settings.width = 250;
	settings.height = 80;
	settings.setPosition(ofVec2f(20, 20));
	settings.resizable = false;
	shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
	
	shared_ptr<ofApp> mainApp(new ofApp);
	
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
}
