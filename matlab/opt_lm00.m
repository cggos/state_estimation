clear all;
close all;
clc;
warning off all;

a=7;b=7;c=7;              %待求解的系数

x=(0:0.01:1)';
w=rand(length(x),1)*2-1;   %生成噪声
y=exp(a*x.^2+b*x+c)+w;     %带噪声的模型 
plot(x,y,'.')

pre=rand(3,1);             
update=1;
u=0.1;
for i=1:100    
    if update==1
        f = exp(pre(1)*x.^2+pre(2)*x+pre(3));
        g = y-f;                                        %计算误差 

        p1 = exp(pre(1)*x.^2+pre(2)*x+pre(3)).*x.^2;    %对a求偏导
        p2 = exp(pre(1)*x.^2+pre(2)*x+pre(3)).*x;       %对b求偏导
        p3 = exp(pre(1)*x.^2+pre(2)*x+pre(3));          %对c求偏导
        J = [p1 p2 p3];                                 %计算雅克比矩阵
        H=J'*J;
        if i==1
            e=dot(g,g);
        end          
    end
    
    delta = inv(H+u*eye(length(H)))*J'* g;      
    pcur = pre+delta;                           %迭代
    fcur = exp(pcur(1)*x.^2+pcur(2)*x+pcur(3)); 
    ecur = dot(y-fcur,y-fcur);
    
    if ecur<e                                   %比较两次差值，新模型好则使用
        if norm(pre-pcur)<1e-10
           break; 
        end
        u=u/2;  
        pre=pcur;
        e=ecur;
        update=1;    
    else
        u=u*2;        
        update=0;
    end 
end

hold on;
plot(x,exp(a*x.^2+b*x+c),'r');
plot(x,exp(pre(1)*x.^2+pre(2)*x+pre(3)),'g');

%比较一下
[a b c]
pre'
