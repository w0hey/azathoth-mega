// header for HMC6352 digital compass interface
#ifndef _COMPASS_H_
#define _COMPASS_H_

// default address
#define COMPASS_ADDRESS 0x42

class Compass {
  public:
    Compass();
    float getHeading();
    void wake();
    void sleep();
    void enterCalibration();
    void exitCalibration();
  private:
    int compassAddr;
};

#endif
