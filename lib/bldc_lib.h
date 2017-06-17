extern unsigned long TIME;
extern unsigned int REV;
extern unsigned long RPM;
extern unsigned int delay_ms_count;
extern unsigned int hall_sensor_count;
extern char avgIndex;
extern unsigned int Array[4];
extern char gainP;
extern char gainI;



unsigned int scaler(char comm, char data); // 4 LSB of comm is determening scaler of data

void delay_ms_init();        // Uses TMR4 to cause interrupt every 1 ms
void delay_ms_start();
void speed_timer_init();

void drive_time(unsigned long tTIME, unsigned long tRPM, unsigned int tPOWER, char modeselect);
void drive_rev(unsigned int rREV, unsigned long rRPM, unsigned int rPOWER, char rMODE);

void PID(int *PIDintegral);
void avgPush(unsigned int avgArray[],unsigned long int value);
