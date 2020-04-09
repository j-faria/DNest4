#ifndef _PROGRESS_BAR_
#define _PROGRESS_BAR_

#ifdef _WINDOWS
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>

#define TOTAL_PERCENTAGE 100.0
#define CHARACTER_WIDTH_PERCENTAGE 3

class ProgressBar{

public: 

    ProgressBar();
    ProgressBar(unsigned long n_, std::string description_ = "", 
                std::ostream &out_ = std::cerr, bool persist_ = false, bool its_ = false);

    void SetFrequencyUpdate(unsigned long frequency_update_);
    void SetStyle(const char* unit_bar_, const char* unit_space_);
    void SetDescription(std::string description_);
    void SetTotal(unsigned long n_);

    // void Progressed(unsigned long idx_);
    void Progressed(unsigned long idx_, unsigned long realnum_=0);
    int GetConsoleWidth();
    int GetBarLength();

private:
	
    unsigned long n;
    unsigned int desc_width;
    unsigned long frequency_update;
    bool persist, its;
    std::ostream* out;
    std::chrono::steady_clock::time_point start, end;
    std::string description;
    const char *unit_bar;
    const char *unit_space;
		
    void ClearBarField();

};

#endif
