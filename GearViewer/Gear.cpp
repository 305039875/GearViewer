#include <sstream>
#include <fstream>
#include <math.h> //sdfsdfasd
#include "StringPlus.h"
#include "Gear.hpp"
#include "fssimplewindow.h"
#include "ysglfontdata.h"

using namespace std;
void Gear::load(std::ifstream& inFile)
{
 string wholeLineString;
 stringstream wholeLineStream;
 bool continueReading = true;
 int colonLocation;

 // go through file
 while (!inFile.eof() && continueReading) {
  // read the whole line
  getline(inFile, wholeLineString);

  // find the colon and prepare to read from stringstream after the colon
  if ((colonLocation = wholeLineString.find(":")) != string::npos)
   wholeLineStream.str(wholeLineString.substr(colonLocation + 1));

  // when the find() function doesn't find it, string::npos is returned
  if (wholeLineString.find("partID") != string::npos) {
   partID = StringPlus::trim(
    wholeLineString.substr(colonLocation + 1));
  }
  else if (wholeLineString.find("pitch") != string::npos) {
   wholeLineStream >> pitch;
  }
  else if (wholeLineString.find("numbTeeth") != string::npos) {
   wholeLineStream >> numbTeeth;
  }
  else if (wholeLineString.find("startX") != string::npos) {
   wholeLineStream >> loc.x;
  }
  else if (wholeLineString.find("startY") != string::npos) {
   wholeLineStream >> loc.y;
  }
  else if (wholeLineString.find("startAngle") != string::npos) {
   wholeLineStream >> angle;
  }

  else if (wholeLineString.find("Gear End") != string::npos) {
   continueReading = false;
  }

  wholeLineStream.clear(); // get ready for next line

 }
}

void Gear::print(std::ostream &output) {
    

        output << "partID: " << partID << endl;
        output << "pitch: " << pitch<< endl;
        output << "numbTeeth: " << numbTeeth<< endl;
        output << "startX" << loc.x<< endl;
        output << "startY" << loc.y<< endl;
        output << "startAngle" << angle<< endl;

      
}

void Gear::draw() {
    float radius = (numbTeeth/pitch)/2;
    float M = 1/pitch;
    float D = 1.25*M;
    float A = 1.0*M;
    float WB = 1.8*M;
    float WT = 1.0*M;
    float r2 = sqrt(((WB/2)*(WB/2))+((radius-D)*(radius-D)));
    float theta = 360./numbTeeth*3.1415926/180. - atan((WB/2)/(radius-D)); //atan returns radians
    glTranslated(loc.x, loc.y, 0);// translate the gear to the start position. Lecture 8 23:00
    glRotated(angle, 0, 0, 1.); //rotate transform to start angle of the gear
    //Set a memory point
    for (int N = 0; N < numbTeeth; N++)
    {
        glBegin(GL_LINE_STRIP);
        glVertex2f(radius-D, -WB/2);
        glVertex2f(radius+A, -WT/2);
        glVertex2f(radius+A, WT/2);
        glVertex2f(radius-D, WB/2);
        glVertex2f(r2*cos(theta),r2*sin(theta));
        glEnd();
        glRotated(360./numbTeeth, 0, 0, 1);
        
    }
    glRotated(-angle, 0, 0, 1.);//Rest Transformation
    glTranslated(-loc.x, -loc.y, 0);
}


