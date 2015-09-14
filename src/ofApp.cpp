#include "ofApp.h"

#define SETTINGS_FILE "settings.xml"

#define MQTT_DEFAULT_ADR "192.168.2.4"
#define MQTT_DEFAULT_PORT 1883
#define OSC_DEFAULT_ADR "localhost"
#define OSC_DEFAULT_PORT 12345

//--------------------------------------------------------------
void ofApp::setup(){

	ofLogNotice() << "MQTT -> OSC starting up...";
	
	ofSetFrameRate(10);
	
	
	//------------------------
	// read settings
	
	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( XML.loadFile(SETTINGS_FILE) ){
		ofLogNotice() << SETTINGS_FILE << " loaded";
	} else {
		ofLogNotice() << "creating settings file with default values " << SETTINGS_FILE;
		
		//------------------------
		// create settings
		mqttAdr = XML.setValue("MQTTAddress", MQTT_DEFAULT_ADR);
		mqttPort = XML.setValue("MQTTPort", MQTT_DEFAULT_PORT);
		oscAdr = XML.setValue("OSCAddress", OSC_DEFAULT_ADR);
		oscPort = XML.setValue("OSCPort", OSC_DEFAULT_PORT);
		
		XML.saveFile(SETTINGS_FILE);
	}
	
	
	mqttAdr = XML.getValue("MQTTAddress", MQTT_DEFAULT_ADR);
	mqttPort = XML.getValue("MQTTPort", MQTT_DEFAULT_PORT);
	oscAdr = XML.getValue("OSCAddress", OSC_DEFAULT_ADR);
	oscPort = XML.getValue("OSCPort", OSC_DEFAULT_PORT);
	
	
	//------------------------
	// setup mosquitto
	ofLogNotice() << "connecting to mosquitto @ " << mqttAdr << ":" << mqttPort;
	mosquitto.setup(mqttAdr, mqttPort);
	ofAddListener(mosquitto.onConnect, this, &ofApp::onMosquittoConnect);
	
	mosquitto.connect();
	
	
	//------------------------
	// setup osc
	// open an outgoing connection to HOST:PORT
	ofLogNotice() << "setting up OSC @ " << oscAdr << ":" << oscPort;
	oscSender.setup(oscAdr, oscPort);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofBackground(127);
	ofSetColor(255);
	ofDrawBitmapString("MQTT -> OSC", 10, 20);
	
	string mqttStr = "MQTT: ";
	mqttStr += mqttAdr;
	mqttStr +=  ":";
	mqttStr += ofToString(mqttPort);
	
	string OSCStr = "OSC: ";
	OSCStr += oscAdr;
	OSCStr +=  ":";
	OSCStr += ofToString(oscPort);
	
	ofDrawBitmapString(mqttStr, 10, 40);
	ofDrawBitmapString(OSCStr, 10, 60);
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit()
{
	//------------------------
	// disconnect
	mosquitto.disconnect();
}


//--------------------------------------------------------------
void ofApp::onMosquittoConnect(int &rc)
{
	if (MOSQ_ERR_SUCCESS == rc)
	{
		mosquitto.subscribe(NULL, "/#");
		ofAddListener(mosquitto.onMessage, this, &ofApp::onMosquittoMessageReceived);
	} else {
		ofLogError() << "error connecting: " << rc;
	}
}

//--------------------------------------------------------------
void ofApp::onMosquittoMessageReceived(ofxMosquittoMessage &msg)
{
	ofxOscMessage m;
	m.setAddress(msg.topic);
	
	if (msg.payloadlen)
	{
		string msgStr = msg.payloadAsString();
		ofLogNotice() << "to OSC: " << msg.topic << " " << msgStr;
		
		m.addStringArg(msgStr);
	}
	
	oscSender.sendMessage(m, false);
}