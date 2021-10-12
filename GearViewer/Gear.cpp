#include <sstream>
#include <fstream>
#include <math.h>
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
    glBegin(GL_LINES);
    glVertex2f(0,0);
    glVertex2f(r2,0);
    glEnd();
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

void Gear::rotate(float rotAngle, Gear *requestingGear) { 
    angle += rotAngle;
    angle = fmod(angle,360.);
        for (Gear* currGear: attachedGears)
        {
            if (currGear != requestingGear)
            {
                currGear->rotate(rotAngle,this); //This is a self-referential pointer.
                // This is a recursion/propgation
            }
        }
        for (Gear *currGear: meshedGears)
        {
            if (currGear != requestingGear)
            {
                float otherAngle = -rotAngle * (numbTeeth/currGear -> getNumbTeeth());
                currGear->rotate(otherAngle,this); //This is a self-referential pointer.
                
            }
        }

}

bool Gear::attachToGear(Gear *otherGear, bool biDirectional) { 
    attachedGears.push_back(otherGear);
    
    // move loc.x and loc.y to match other gear.
    loc = otherGear->getLoc();
    if(biDirectional)
    {
        otherGear->attachToGear(this,false);
    }
    return true;
}

bool Gear::meshInto(Gear *otherGear, bool biDirectional) { 
    if (pitch == otherGear->pitch)
    {
        meshedGears.push_back(otherGear);
        
        if(biDirectional)
        {
            // move loc.x and loc.y to match other gear.
            float before_after_ratio = sqrt(pow(otherGear->loc.x - loc.x,2)+pow(otherGear->loc.y - loc.y,2))/((numbTeeth/pitch)/2+otherGear->getRadius()); // calculate the ratio between two distances
            
            float minX = (otherGear->loc.x - loc.x)/before_after_ratio; //minX is the x distance of two meshed gears
            float moveX = (otherGear->loc.x - loc.x) - minX;
            
            float minY = (otherGear->loc.y - loc.y)/before_after_ratio;
            float moveY = (otherGear->loc.y - loc.y) - minY;
            
            float angleLine = atan2(moveY,moveX) * 180/3.1415926;
            float otherGearAngle = otherGear->getAngle();
            angle;
            float moveAngle = 180-angle - (otherGearAngle-angleLine)/(pitchDiam()/otherGear->pitchDiam()) + angleLine + (360/numbTeeth)/2;
        
            angle += moveAngle;
            loc.x = loc.x + moveX;
            loc.y = loc.y + moveY;
            otherGear->meshInto(this,false);
        }
        return true;
    }else
        return false;
    
}

void Gear::edit()
{
 string tempString;

 cout << "\nFor each of the following, enter \".\" to keep original value:" << endl;

 cout << "\tpartID (" << partID << ") new value >> ";
 cin >> tempString;
 StringPlus::trim(tempString);
 if (tempString != ".")
  partID = tempString;

 cout << "\tpitch (" << pitch << ") new value >> ";
 cin >> tempString;
 StringPlus::trim(tempString);
 if (tempString != ".")
  pitch = stof(tempString);

 cout << "\tnumbTeeth (" << numbTeeth << ") new value >> ";
 cin >> tempString;
 StringPlus::trim(tempString);
 if (tempString != ".")
  numbTeeth = stoi(tempString);

 cout << "\tstartX (" << loc.x << ") new value >> ";
 cin >> tempString;
 StringPlus::trim(tempString);
 if (tempString != ".")
  loc.x = stof(tempString);

 cout << "\tstartY (" << loc.y << ") new value >> ";
 cin >> tempString;
 if (tempString != ".")
  loc.y = stof(tempString);

 cout << "\tangle (" << angle << ") new value >> ";
 cin >> tempString;
 if (tempString != ".")
  angle = stof(tempString);
}

