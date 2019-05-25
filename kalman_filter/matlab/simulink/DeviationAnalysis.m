%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 跟踪偏差分析
%  详细原理介绍及中文注释请参考：
%  《卡尔曼滤波原理及应用-MATLAB仿真》，电子工业出版社，黄小平著。
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function DeviationAnalysis
load Xstate;
load Xkalman;
load Zobserv;
Xstate 
Xkf   
Z 
T1=length(Xstate(1,:))
T2=length(Z(1,:)) 
T=min(T1,T2)
Div_Observ_Real=zeros(1,T);
Div_Kalman_Real=zeros(1,T);
for i=2:T
    Div_Observ_Real(i)=sqrt( (Z(1,i)-Xstate(1,i))^2+(Z(2,i)-Xstate(3,i))^2 );
    Div_Kalman_Real(i)=sqrt( (Xkf(1,i)-Xstate(1,i))^2+(Xkf(3,i)-Xstate(3,i))^2 );
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
figure  
hold on;box on;
plot(Xstate(1,:),Xstate(3,:),'-r');
plot(Xkf(1,:),Xkf(3,:),'-k+');
plot(Z(1,:),Z(2,:),'-r.');
legend('true','Kalman','observation');
xlabel('X/m');ylabel('X/m');
figure
hold on;box on;
plot(Div_Observ_Real,'-ko','MarkerFace','g');
plot(Div_Kalman_Real,'-kd','MarkerFace','r');
legend('Observ','Kalman');
xlabel('tinme/s');ylabel('Value of the Deviation');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
