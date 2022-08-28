clear all;
close all;
clc;

a=1;b=2;c=1;              %待求解的系数

x=(0:0.01:1)';
w=rand(length(x),1)*2-1;   %生成噪声
y=exp(a*x.^2+b*x+c)+w;     %带噪声的模型 
plot(x,y,'.')

pre=rand(3,1);      %步骤1
for i=1:1000
    
    f = exp(pre(1)*x.^2+pre(2)*x+pre(3));
    g = y-f;                    %步骤2中的误差 
    
    p1 = exp(pre(1)*x.^2+pre(2)*x+pre(3)).*x.^2;    %对a求偏导
    p2 = exp(pre(1)*x.^2+pre(2)*x+pre(3)).*x;       %对b求偏导
    p3 = exp(pre(1)*x.^2+pre(2)*x+pre(3));          %对c求偏导
    J = [p1 p2 p3];             %步骤2中的雅克比矩阵
    
    delta = inv(J'*J)*J'* g;    %步骤3，inv(J'*J)*J'为H的逆
    
    pcur = pre+delta;           %步骤4
    if norm(delta) <1e-16
        break;
    end
    pre = pcur;
end

hold on;
plot(x,exp(a*x.^2+b*x+c),'r');
plot(x,exp(pre(1)*x.^2+pre(2)*x+pre(3)),'g');

%比较一下
[a b c]
pre'
