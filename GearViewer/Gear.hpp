#pragma once
#include <string>
#include <iostream>


struct Point2D {
    float x, y;
};
class Gear{
private:
    std::string partID; // used for identification
    float pitch;        // given in teeth per inch
    int numbTeeth;
    Point2D loc;        // current location of gear {x, y}
    float angle;        // current rotation of gear in degrees
    
    
    std::vector<Gear *> attachedGears;
    std::vector<Gear *> meshedGears;
    
public:
    // reads data from a file over-riding any existing data. We’ll work on
    // this function in class together.
    void load(std::ifstream &inFile);
    // outputs all the data for a gear in format matching attached example
    void print(std::ostream &output = std::cout);
    // calculates and return the pitch diameter
    float pitchDiam() { return numbTeeth / pitch; };
    // draws the gear on graphics window using simplified geometry as attached
    void draw();
    // rotate the gear by given amount
    // and cause the rotation of connected gears (attached and meshed)
    // except do not rotate the gear that is requesting the rotation
    void rotate(float rotAngle, Gear *requestingGear = nullptr);
    
    float getNumbTeeth() {return numbTeeth;};
    
    Point2D getLoc() {return loc;};
    
    float getRadius() {return (numbTeeth/pitch)/2;};
    float getAngle() {return angle;};
    std::string getID() { return partID; }
    void setID(const std::string &newID) { partID = newID; }
    
    Point2D minBound() {
      return { loc.x - pitchDiam() / 1.7f, loc.y - pitchDiam() / 1.7f };
     }
    
    Point2D maxBound() {
      return { loc.x + pitchDiam() / 1.7f, loc.y + pitchDiam() / 1.7f };
     }
    
    // Extra: Translation
//    void moveup(float distance) { loc.y += distance; };
//    void movedown(float distance) { loc.y -= distance; };
//    void moveleft(float distance) { loc.x -= distance; };
//    void moveright(float distance) { loc.x += distance; };
    void spin(float speed){
            angle+=speed;
    };
    // create a bi-directional attach relationship between this gear and
    // otherGear, unless bi-directional is not appropriate
    // may require this gear’s location to change to match otherGear’s
    // returns false if connection cannot be done
    bool attachToGear(Gear *otherGear, bool biDirectional = true);
    
    // create a bi-directional meshed relationship between this gear and
    // otherGear, unless bi-directional is not appropriate
    // may require this gear to translate (along line connecting centers)
    // and rotate to allow meshing, but do not move or rotate otherGear
    // returns false if connection cannot be done (unmatched pitches)
    bool meshInto(Gear *otherGear, bool biDirectional = true);
    
    void edit();
    
};
