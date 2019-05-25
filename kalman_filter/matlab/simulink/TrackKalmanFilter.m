%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [sys,x0,str,ts]=TrackKalmanFilter(t,x,u,flag)
global Xkf;
global Z;
switch flag
    case 0  
        [sys,x0,str,ts]=mdlInitializeSizes;
    case 2  
        sys=mdlUpdate(t,x,u);
    case 3  
        sys=mdlOutputs(t,x,u);
    case {1,4}
        sys=[];
    case 9
        save('Zobserv','Z');
        save('Xkalman','Xkf');
    otherwise   
        error(['Unhandled flag = ',num2str(flag)]);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [sys,x0,str,ts]=mdlInitializeSizes
global P;
global Xkf;
global Z;   % 
P=0.1*eye(4);
sizes = simsizes;
sizes.NumContStates  = 0;  
sizes.NumDiscStates  = 4;  
sizes.NumOutputs     = 4;  
sizes.NumInputs      = 2; 
sizes.DirFeedthrough = 0;
sizes.NumSampleTimes = 1; 
sys = simsizes(sizes);
x0  = [10,5,12,5]';  
str = [];          
ts  = [-1 0]; 
Xkf=[];
Z=[];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function sys=mdlUpdate(t,x,u)
global P;
global Xkf;
global Z; 
F=[1,1,0,0;0,1,0,0;0,0,1,1;0,0,0,1]; 
G=[0.5,0;1,0;0,0.5;0,1];            
H=[1,0,0,0;0,0,1,0];             
Q=diag([0.0001,0.0009]);      
R=diag([0.04,0.04]);  

Xpre=F*x; 
Ppre=F*P*F'+G*Q*G';
K=Ppre*H'*inv(H*Ppre*H'+R); 
e=u-H*Xpre; 
Xnew=Xpre+K*e;
P=(eye(4)-K*H)*Ppre;
sys=Xnew; 
Z=[Z,u];
Xkf=[Xkf,Xnew];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function sys=mdlOutputs(t,x,u)
sys = x; 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
