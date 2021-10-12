#include <string.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "Gear.hpp"
#include "GearSystem.hpp"
using namespace std;


void GearSystem::draw(bool highlightOther) { 
    // draw the gears
    glColor3ub(0, 0, 0);
    glLineWidth(1);
    
    for (Gear* currGear : allGears){ //what is the difference between use & and *
        currGear->draw();
    }
    
    //highlight current Gear
    glColor3ub(255, 0, 0);
    glLineWidth(3);
    if (currGearIndex >= 0 && currGearIndex < allGears.size()) // extra protection
        allGears.at(currGearIndex)->draw(); 
    
    
    if (highlightOther)
    {
        glColor3ub(0, 255, 0);
        glLineWidth(3);
        if (otherGearIndex >= 0 && otherGearIndex < allGears.size())
            allGears.at(otherGearIndex)->draw();
        
    }
}

void GearSystem::inputRequiredMessage() { 
    // display message on graphics window directing user to check console
    
    string message = "Input required on console . . .";
    // black color
    glColor3f(0, 0, 0);
    
    //center horizontally and vertically for 20-pixel-wide by 28-pixel-height letters
    int wid, hei;
    FsGetWindowSize(wid, hei);
    glRasterPos2d(wid / 2 - message.length() * 20 / 2, hei / 2 + 28 / 2);
    
    YsGlDrawFontBitmap20x28(message.c_str()); // function requires a C-language string
    FsSwapBuffers();
}

void GearSystem::rotateCurrent(float deg) { 
    allGears.at(currGearIndex)->rotate(deg);
}

void GearSystem::deleteCurrent() {
    
    if (currGearIndex >= 0) {
        // redirect to console
        inputRequiredMessage();
        
        // ask for verification
        cout << "\n\nAre you sure you want to delete this gear ("
        << allGears.at(currGearIndex)->getID() << ")? (Y/N) >> ";
        cin >> yesNo;
        
        // delete/erase the current curve and make the previous curve current
        if (yesNo == 'Y' || yesNo == 'y') {
            delete allGears[currGearIndex];
            allGears.erase(allGears.begin() + currGearIndex);
            currGearIndex--;
            if (currGearIndex < 0)
                currGearIndex = allGears.size() - 1;
        }
        
        while (getchar() != '\n'); // this will clear the newline associated with last input
      
    }
}

void GearSystem::editCurrent() { 
    allGears.at(currGearIndex)->edit();
}

void GearSystem::printCurrent() { 
    cout << "\n\n";
    if (currGearIndex >= 0)
        allGears.at(currGearIndex)->print();
}

void GearSystem::changeCurrent(int change) {
    currGearIndex += change;
    currGearIndex = (allGears.size()+currGearIndex)%allGears.size();
    
}

void GearSystem::changeOther(int change) {
    otherGearIndex += change;
    otherGearIndex = (allGears.size()+otherGearIndex)%allGears.size();
    if (otherGearIndex == currGearIndex)
    {
        otherGearIndex += change;
        otherGearIndex = (otherGearIndex)%allGears.size();
    }
    
}

void GearSystem::addGear(const std::string &filename) {
    // redirect to console
    Gear *newGear;
    inputRequiredMessage();
    if (filename == "")
    {
        cout << "\n\nPlease enter the name of the file to read > ";
        cin >> datafilename;
    }
    else{
        datafilename = filename;
    }
    //ask for filename
    
    if (datafilename.find(".gear") == string::npos)
        datafilename += ".gear";     // allows user to omit extension
    inFile.open(datafilename);
    
    if (inFile.is_open()) { // may not have found file
        //loop
        newGear = new Gear;
        newGear->load(inFile);
        //printf("added one");
        inFile.close();
        allGears.push_back(newGear); 
        currGearIndex = allGears.size() - 1;
    }
    else
        cout << "\nError reading file. Please check data and try again." << endl;
    
}

void GearSystem::getOriginAndScale(float &originX, float &originY, float &scale)
{
    if (allGears.size() > 0) {
        Point2D minBnd, maxBnd, currMin, currMax;

        minBnd = allGears[0]->minBound();
        maxBnd = allGears[0]->maxBound();
        for (Gear* currGear : allGears) {
            currMin = currGear->minBound();
            currMax = currGear->maxBound();
            if (minBnd.x > currMin.x)
                minBnd.x = currMin.x;
            if (minBnd.y > currMin.y)
                minBnd.y = currMin.y;
            if (maxBnd.x < currMax.x)
                maxBnd.x = currMax.x;
            if (maxBnd.y < currMax.y)
                maxBnd.y = currMax.y;
        }
        originX = minBnd.x; originY = minBnd.y;

        int hei, wid;
        FsGetWindowSize(wid, hei);

        float scaleX, scaleY;
        int extraBorder = 10; // allows for some space around all the gears
        scaleX = (wid - extraBorder) / (maxBnd.x - minBnd.x);
        scaleY = (hei - extraBorder) / (maxBnd.y - minBnd.y);
        scale = min(scaleX, scaleY);
        if (scale == scaleX) // adjust originY so as to center vertically
            originY -= 0.5 * (hei / scale - (maxBnd.y - minBnd.y));
        else // adjust originX so as to center horizontally
            originX -= 0.5 * (wid / scale - (maxBnd.x - minBnd.x));

        // adjust for half extraborder
        originX -= extraBorder / 2. / scale;
        originY -= extraBorder / 2. / scale;
    }
    else {
        originX = originY = 0.;
        scale = 1.0;
    }

}

bool GearSystem::attachGears()
{
    //for (Gear* attached: attachGears())
    //{
    return allGears.at(currGearIndex)->attachToGear(allGears.at(otherGearIndex));
    //}
}


bool GearSystem::meshGears()
{
    return allGears.at(currGearIndex)->meshInto(allGears.at(otherGearIndex));
}

void GearSystem::saveAll() { 
    ofstream outFile;
    string datafilename;
    
    //ask for filename
    cout << "\n\nPlease enter the name of the file";
    cin >> datafilename;
    
    if (datafilename.find(".gear") == string::npos)
        datafilename += ".gear";
    outFile.open(datafilename);
    
    if (outFile.is_open())
    {
        for (Gear* currGear : allGears)
        {
            currGear->print(outFile);
            //outFile << *currGear;
        }
        outFile.close();
    }
    else
        cout << "\n Error writing file";
}
















