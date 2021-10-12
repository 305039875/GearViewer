#include <stdio.h>
#include "Gear.hpp"
#include <iostream>
#include <vector>


class GearSystem{

private:
    std::vector<Gear *> allGears;
    int currGearIndex;
    int otherGearIndex;
    char yesNo;
    std::string datafilename;
    std::ifstream inFile;
    

public:
    // draws all gears, highlighting current gear and optionally
    //      highlighting      otherGear.
    void draw(bool highlightOther = true);
    // displays helpful message on graphics window
    void inputRequiredMessage();
    // rotates the current gear
    void rotateCurrent(float deg);
    // removes current gear, with confirmation
    void deleteCurrent();
    // edits current gear
    void editCurrent();
    // prints current gear to console
    void printCurrent();
    // changes the current gear, being careful to "loop around" as needed
    void changeCurrent(int change);
    // changes the other gear, being careful to "loop around" as needed
    // but skipping over current gear
    void changeOther(int change);
    // OPTIONAL:
    // changes the current gear based on given model coords
    void changeCurrent(Point2D givenCoords);
    // OPTIONAL:
    // changes the other gear based on given model coords
    // but skipping over current gear
    void changeOther(Point2D givenCoords);
    // asks for filename and loads a gear or gears,
    // adding it/them to system
    void addGear(const std::string &filename = "");
    // returns a pointer to the gear that matches the givenID,
    // returns nullptr if not found
    Gear *getGear(const std::string &gearID) ; //THis is called function overloading
    // returns a pointer to the gear at given index,
    // returns nullptr if not found
    Gear *getGear(int gearIndex){
        
        return allGears.at(gearIndex);
        
    };
    // figures out best origin and scale for display
    void getOriginAndScale(float &originX, float &originY, float &scale);
    // attaches the current gear to the other gear
    bool attachGears();
    // meshes the current gear into the other gear
    bool meshGears();
    // asks for a file name and saves all gears in a single file
    void saveAll();



};
