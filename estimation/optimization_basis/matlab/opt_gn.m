%%  Gauss-Newton method
clc;
clear;
format long; 
syms x1 x2 x3 x4;
error=1e-5;   %停机门限
f = [x1+10*x2; 5^(1/2)*(x3-x4); (x2-2*x3)^2; 10^(1/2)*(x1-x4)^2];
v=[x1 x2 x3 x4];
j=jacobian(f,v);   %求jacobian行列式
x=[3;-1;0;1];      %初始迭代点
J=subs(j,v,x');     %初点数值带入表达式
F=subs(f,v,x');
k=0;               %迭代次数
tic
while (sum((J'*F).^2))^(1/2)>error  %判断停机与否, 这里相当于求梯度的模
	    d=-inv(J'*J)*J'*F;          %搜索方向
        x=x+d;                      %新的迭代点
	    J=subs(j,v,x');              %新的迭代点数值带入表达式           
        F=subs(f,v,x');
	    k=k+1;                      %迭代次数加1
end
disp('Gauss-Newton algorithm');
toc
k
x
sigma=(sum((J'*F).^2))^(1/2)        
F=(sum(F.^2))^(1/2)                 %显示迭代次数，变量取值，停机表达式值，目标函数值
%% 