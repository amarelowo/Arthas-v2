#include <Arduino.h>

class PID{
    public:
        double error,  
               lastError = 0,
               kP, 
               kI, 
               kD, 
               P, 
               I = 0, 
               D, 
               pid, 
               setPoint;
        
        long lastProcess;

        int period;

    PID(double _kP, double _kI, double _kD, double _setPoint, int _period){
        kP = _kP;
        kI = _kI;
        kD = _kD;
        setPoint = _setPoint;
        period = _period;
    }

    void atualizarPID(double _novoKp, double _novoKi, double _novoKd){
        kP = _novoKp;
        kI = _novoKi;
        kD = _novoKd;
    }

    double process(double sample){

        error = setPoint - sample;

        P = kP * error;

        D = kD * ((error - lastError)/period);

        I = I + (kI * error);

        pid = P + I + D;

        lastError = error;

        return pid;
    }

    String pidString(){
        String sPID = "P: " + (String)P + " | I: " + (String)I + " | D: " + (String)D + " | pid: " + (String)pid;
        return sPID;
    }

};