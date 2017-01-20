
#ifndef SRC_UTILS_CONSOLE_COLORS_H
#define SRC_UTILS_CONSOLE_COLORS_H

#include <stdio.h>


#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


#define danger_text(txt) \
  printf("    " RED "%s" RESET "\n", txt); \

#define NONE    0x00
#define DBG      0x1F
#define INFO     0x0F
#define ERR      0x07
#define EMR      0x03
#define CRIT     0x01

/* current log level */
#define LOG_LEVEL INFO

#define WHERESTR "[FILE : %s, FUNC : %s, LINE : %d]: "
#define WHEREARG __FILE__,__func__,__LINE__
#define PRINT_DEBUG_STR(...)  printf(__VA_ARGS__)
#define PRINT_LOG(X, _fmt, ...) if((LOG_LEVEL & X) == X) \
                                    PRINT_DEBUG_STR(WHERESTR RED _fmt RESET, WHEREARG,__VA_ARGS__)

namespace utils {
	namespace logger {
		
		//void get_log_color(int level, char* color) {
			//if (level==ERR) {
			//}
		//}
		//void print_log(int level, _fmt, ...) {
			//if ((LOG_LEVEL & level) == level) {

			//}
		//}
	}
}
#endif
