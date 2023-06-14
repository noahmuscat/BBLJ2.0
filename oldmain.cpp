
// For printf
#include <stdio.h>

// For csv
#include <fstream>

// For the LabJackM Library
#include "LabJackM.h"

// For LabJackM helper functions, such as OpenOrDie, PrintDeviceInfoFromHandle,
// ErrorCheck, etc., such as OpenOrDie, PrintDeviceInfoFromHandle,
// ErrorCheck, etc.
#include "LJM_Utilities.h"

int main()
{
    int err;
    int handle;
    int i;
    int errorAddress = INITIAL_ERR_ADDRESS;
    int skippedIntervals;
    int deviceType;

    const int INTERVAL_HANDLE = 1;
    int it = 0;

    // interval time (ms)
    int msDelay = 100;

    enum { MAX_FRAMES = 9 };
    
    int numFrames;
    const char* aNames[MAX_FRAMES];
    double aValues[MAX_FRAMES];

    // Open first found LabJack
    err = LJM_Open(LJM_dtANY, LJM_ctANY, "LJM_idANY", &handle);
    ErrorCheck(err, "LJM_Open");

    PrintDeviceInfoFromHandle(handle);
    printf("\n");

    deviceType = GetDeviceType(handle);
    
    std::ofstream dataFile("data.csv");

    // Begin the loop
    printf("Starting loop.  Press Ctrl+c to stop.\n\n");
    // The LabJackM (LJM) library will catch the Ctrl+c signal, close
    // all open devices, then exit the program.

    err = LJM_StartInterval(INTERVAL_HANDLE, msDelay * 1000);
    ErrorCheck(err, "LJM_StartInterval");

    while (1) {
        // configures two ports
        numFrames = 9;
        aNames[0] = "AIN0";
        aNames[1] = "FIO0";
        /*
        aNames[2] = "";
        aNames[3] = "";
        aNames[4] = "";
        aNames[5] = "";
        aNames[6] = "";
        aNames[7] = "";
        aNames[8] = "";
        aNames[9] = "";
        */
        
        // reads in values
        err = LJM_eReadNames(handle, numFrames, (const char **)aNames, aValues,
            &errorAddress);
        ErrorCheckWithAddress(err, errorAddress, "LJM_eReadNames");
        // prints values
        printf("eReadNames  :");
        for (i = 0; i < numFrames; i++) {
            printf(" %s = %.4f  ", aNames[i], aValues[i]);
        }
        printf("\n");
        
        for (i = 0; i < numFrames; i++) {
            dataFile << aNames[i] << " = " << aValues[i] << "   ";
        }
        dataFile << std::endl;

        ++it;

        err = LJM_WaitForNextInterval(INTERVAL_HANDLE, &skippedIntervals);
        ErrorCheck(err, "LJM_WaitForNextInterval");
        if (skippedIntervals > 0) {
            printf("SkippedIntervals: %d\n", skippedIntervals);
        }
        printf("\n");
    }

// Close interval and device handles
    err = LJM_CleanInterval(INTERVAL_HANDLE);
    ErrorCheck(err, "LJM_CleanInterval");

    err = LJM_Close(handle);
    ErrorCheck(err, "LJM_Close");

    WaitForUserIfWindows();
    
    dataFile.close();

    return LJME_NOERROR;
}
