% x=0:0.35:7;
% y=2*exp(-0.5*x);
% subplot(2,2,1);bar(x,y,'g');title('bar(x,y,''g'')');axis([0, 7, 0 ,2]);
% subplot(2,2,2);fill(x,y,'r');title('fill(x,y,''r'')');axis([0, 7, 0 ,2]);
% subplot(2,2,3);stairs(x,y,'b');title('stairs(x,y,''b'')');axis([0, 7, 0 ,2]);
% subplot(2,2,4);stem(x,y,'k');title('stem(x,y,''k'')');axis([0, 7, 0 ,2]);

% %极坐标方程
% theta=0:0.01:2*pi;
% rho=sin(3*theta).*cos(5*theta);
% figure
% polar(theta,rho,'r');

%program 1
x=0:0.1:2*pi;
[x,y]=meshgrid(x);
z=sin(y).*cos(x);
subplot(2,2,1);
mesh(x,y,z);
xlabel('x-axis'),ylabel('y-axis'),zlabel('z-axis');
title('mesh'); 

%program 2
x=0:0.1:2*pi;
subplot(2,2,2);
[x,y]=meshgrid(x);
z=sin(y).*cos(x);
surf(x,y,z);
xlabel('x-axis'),ylabel('y-axis'),zlabel('z-axis');
title('surf');

%program 3
x=0:0.1:2*pi;
subplot(2,2,3);
[x,y]=meshgrid(x);
z=sin(y).*cos(x);
plot3(x,y,z);
xlabel('x-axis'),ylabel('y-axis'),zlabel('z-axis');
title('plot3-1');grid;

%三维图形
t=0:pi/50:2*pi;
x=8*cos(t);
y=4*sqrt(2)*sin(t);
z=-4*sqrt(2)*sin(t);
subplot(2,2,4);
%figure
plot3(x,y,z,'p');
title('Line in 3-D Space');
text(0,0,0,'origin');
xlabel('X');ylabel('Y');zlabel('Z');
grid;

