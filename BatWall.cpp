/////////////////////////////////////////////////////////////////////////////
//  BatWall.cpp - Nathan Higley - 18 Feb 2020 - contact@nathanhigley.com   //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Description:                                                           //
//  This program sets different images as the desktop wallpaper based on   //
//  the current state of the battery or whether the computer is using AC   //
//  power.                                                                 //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  Prerequisites:                                                         //
//  A folder in the current working directory named "images" with the      //
//  wallpapers inside with the following scheme, "battery_<1-5>.png" and   //
//  "charge_<1-5>.png contained inside.                                    //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////


// Libraries used
#include <Windows.h>
#include <string>

// Usage directives
using std::string;

// Function prototypes
int getBatteryPercentage();
bool isOnAC();
bool setWallpaper(string);

int main()
{
    // Wallpaper name arrays
    const string batt_walls[5] = { "battery_1", "battery_2", "battery_3", "battery_4", "battery_5" };
    const string char_walls[5] = { "charge_1", "charge_2", "charge_3", "charge_4", "charge_5" };
    // Integer variables to store important data
    int current_wall{ -1 };
    int next_wall{ 0 };
    int current_battery{ 0 };
    // Control variable
    bool wall_changed{ true };

    // Enter the event loop
    while (wall_changed)
    {
        // Check if the computer is on AC power or not
        if (isOnAC())
        {
            // Check if fully charged
            if (getBatteryPercentage() == 100)
            {
                // Set the wall to the last charging one
                wall_changed = setWallpaper(char_walls[4]);
                Sleep(1000);
            }
            else
            {
                // Change the wallpaper every second to the next charging wallpaper
                for (int x{ 0 }; x < 5; x++)
                {
                    // Attempt to change the wallpaper
                    wall_changed = setWallpaper(char_walls[x]);
                    Sleep(1000);
                }
            }
            // Reset the current wall number since the wallpaper changed
            current_wall = -1;
        }
        else
        {
            // Pause for a second
            Sleep(1000);
            // Get the current battery percentage
            current_battery = getBatteryPercentage();
            // Check if the battery is fully charged
            if (current_battery == 100)
            {
                // If so set the image to the max charge image
                next_wall = 4;
            }
            else
            {
                // Otherwise the image is just the floored fifth of the charge
                next_wall = current_battery / 20;
            }
            // Make sure the wallpaper is not already set in which case we do nothing
            if (current_wall != next_wall)
            {
                // Otherwise set the current wall equal to the next wall
                current_wall = next_wall;
                // Attempt to set the wallpaper
                wall_changed = setWallpaper(batt_walls[current_wall]);
            }
        }
    }
    // Return the inverse of wall_changed since if it is 1 then the program exited normally, else it should be 0 which is an error
    return !wall_changed;
}

// Function to get the current battery percentage
int getBatteryPercentage()
{
    // Variable to store the percentage
    int percentage{ 0 }; 
    // Get the percentage
    SYSTEM_POWER_STATUS spsPwr; 
    GetSystemPowerStatus(&spsPwr);
    // Cast the percentage to an int
    percentage = static_cast<int>(spsPwr.BatteryLifePercent);
    // Return the percentage
    return percentage;
}

// Function to get whether the laptop is on AC power or not
bool isOnAC()
{
    // Get the power status
    SYSTEM_POWER_STATUS spsPwr;
    GetSystemPowerStatus(&spsPwr);
    // Get the AC state
    return static_cast<int>(spsPwr.ACLineStatus);
}

// Function to set the wallpaper to the filename.
// Requirements: images in a folder called "images" in the current working directory
// and images are png files
bool setWallpaper(string imageName)
{
    // Get the current directory and store it into a char string
    char buf[256];
    GetCurrentDirectoryA(256, buf);
    // Create the absolute path to the image
    string file = buf + string("\\images\\") + imageName + string(".png");
    // Convert the string to a wide string
    std::wstring widestr = std::wstring(file.begin(), file.end());
    // Convert the wide string to a c string
    const wchar_t* filenm = widestr.c_str();
    // Set the wallpaper to the given image
    return SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)filenm, SPIF_UPDATEINIFILE);
}