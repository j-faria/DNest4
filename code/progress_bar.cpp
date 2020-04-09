#include "progress_bar.hpp"

ProgressBar::ProgressBar(){
    description = "";
    out = &std::cerr;
    persist = false;
    its = false;
	
	unit_bar = "=";
	unit_space = " ";
	desc_width = description.size();	// character width of description field
    start = std::chrono::steady_clock::now();
}

ProgressBar::ProgressBar(unsigned long n_, std::string description_, 
                         std::ostream& out_, bool persist_, bool its_){
    
    n = n_;
    frequency_update = n_;
    description = description_;
    out = &out_;
    persist = persist_;
    its = its_;
	
	unit_bar = "=";
	unit_space = " ";
	desc_width = description.size();	// character width of description field
    start = std::chrono::steady_clock::now();

}

void ProgressBar::SetFrequencyUpdate(unsigned long frequency_update_){
	
	if(frequency_update_ > n){
		frequency_update = n;	 // prevents crash if freq_updates_ > n_
	}
	else{
		frequency_update = frequency_update_;
	}
}

void ProgressBar::SetStyle(const char* unit_bar_, const char* unit_space_){
	
	unit_bar = unit_bar_;
	unit_space = unit_space_;
}

void ProgressBar::SetDescription(std::string description_){
	
    description = description_;
}

void ProgressBar::SetTotal(unsigned long n_){

    n = n_;
    frequency_update = n_;
}

int ProgressBar::GetConsoleWidth(){

    int width;

	#ifdef _WINDOWS
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		width = csbi.srWindow.Right - csbi.srWindow.Left;
	#else
		struct winsize win;
		ioctl(0, TIOCGWINSZ, &win);
        width = win.ws_col;
	#endif

    return width;
}

int ProgressBar::GetBarLength(){

	// get console width and according adjust the length of the progress bar

    int bar_length = static_cast<int>((GetConsoleWidth() - desc_width - CHARACTER_WIDTH_PERCENTAGE) / 2.);

	return bar_length;
}

void ProgressBar::ClearBarField(){

    for(int i=0;i<GetConsoleWidth();++i){
        *out << " ";
    }
    *out << "\r" << std::flush;
    // std::cout << "called clear" << std::endl;
}

void ProgressBar::Progressed(unsigned long idx_, unsigned long realnum_)
{
    try{
        if(idx_ > n) throw idx_;

        // determines whether to update the progress bar from frequency_update
	    if ((idx_ != n) && (idx_ % (n/frequency_update) != 0)) return;


        // calculate the size of the progress bar
	    int bar_size = GetBarLength();
    
        // calculate percentage of progress
        double progress_percent = idx_* TOTAL_PERCENTAGE/n;

        // calculate the percentage value of a unit bar 
        double percent_per_unit_bar = TOTAL_PERCENTAGE/bar_size;

        // ClearBarField();

        // display progress bar
	    *out << description;

        // count iterations per second?
        if (its){
            end = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            int N;
            if (realnum_==0) N = idx_;
            else N = realnum_;
            *out << std::setw(4) << std::setprecision(0) << std::fixed << static_cast<int>(1e6*N/duration) << "it/s";
        }

        *out << " [";

        for(int bar_length=0;bar_length<=bar_size-1;++bar_length){
            if(bar_length*percent_per_unit_bar<progress_percent){
                *out << unit_bar;
            }
            else{
                *out << unit_space;
            }
        }

        *out << "]" << std::setw(CHARACTER_WIDTH_PERCENTAGE + 1) << std::setprecision(0) << std::fixed << progress_percent << "%\r" << std::flush;
        if(idx_ == n && persist) *out << std::endl;
    }
    catch(unsigned long e){
        ClearBarField();
        std::cerr << "PROGRESS_BAR_EXCEPTION: _idx (" << e << ") went out of bounds, greater than n (" << n << ")." << std::endl << std::flush;
    }

}
