function [sys,x0,str,ts] = SimuKalmanFilter(t,x,u,flag)
switch flag
    case 0  
        [sys,x0,str,ts]=mdlInitializeSizes;
    case 1 
        sys=mdlDerivatives(t,x,u);
    case 2  
        sys=mdlUpdate(t,x,u);
    case 3 
        sys=mdlOutputs(t,x,u);
    case 4 
        sys=mdlGetTimeOfNextVarHit(t,x,u);
    case 9 
        sys=mdlTerminate(t,x,u);
    otherwise 
        error(['Unhandled flag = ',num2str(flag)]);
end
function [sys,x0,str,ts]=mdlInitializeSizes
sizes = simsizes;
sizes.NumContStates  = 0;
sizes.NumDiscStates  = 1;
sizes.NumOutputs     = 1;
sizes.NumInputs      = 1;
sizes.DirFeedthrough = 1;
sizes.NumSampleTimes = 1;
sys = simsizes(sizes);
x0  = 5000+sqrt(5)*randn;     
str = [];             
ts  = [-1 0]; 
global P;   
P=5;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function sys=mdlDerivatives(t,x,u)
sys = [];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function sys=mdlUpdate(t,x,u)
global P;
F=1;B=0;H=1;  
Q=0;R=5;  
xpre=F*x+B*u; 
Ppre=F*P*F'+Q;
K=Ppre*H'*inv(H*Ppre*H'+R);
e=u-H*xpre;  
xnew=xpre+K*e; 
P=(eye(1)-K*H)*Ppre; 
sys=xnew;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function sys=mdlOutputs(t,x,u)
sys = x; 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function sys=mdlGetTimeOfNextVarHit(t,x,u)
sampleTime = 1; 
sys = t + sampleTime;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function sys=mdlTerminate(t,x,u)
sys = [];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%