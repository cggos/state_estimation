close all;
clc;
lamda = 0.01;
lamda
vv = 2;
updateJ=1;
errorsum1=[];
a=0.5; b=2;
x1=1:5;
x2=1:5;             %x1,x2在1-5中随机取25个点
x=[x1;x2];         % 变量矩阵
 
[X1 ,X2]=meshgrid(x1,x2);
y=1./(X1+a)+X2.^2./(b+X2.^2);
randn(0);
random=randn(size(y)).*sqrt(0.1);
Y=y+random;
S1=reshape(X1,25,1);
S2=reshape(X2,25,1);
S3=reshape(Y,25,1);
%figure(1);
%plot3(X1,X2,y);
%hold on
% figure(2);
% plot3(X1,X2,y);
%scatter3(S1,S2,S3,'r');
%hold on;
X1=X1';
X2=X2';
Y=Y';
syms a b;                  %MATLAB不支持符号变量编译为exe，所以只要用到符号变量的就只能在MATLAB运行罢了
B=[a b];                    %但是求导是只能用符号变量的，所以无法编译为独立运行的exe
B1=[20 20];              %步骤1,给ab赋予初值
f = 1./(X1+B(1))+X2.^2./(B(2)+X2.^2);
r=Y-f;
error=r'*r;
error1=subs(error,B,B1);
errorsum=sum(error1(:).^2);
errorsum1=[errorsum1 errorsum];
last_errorsum = errorsum;
 
r=reshape(r,25,1);
J=jacobian(r,B);
G=2.*(J'*r);
G1=subs(G,B,B1);
H=2.*(J'*J);
H1=subs(H,B,B1);
% 根据阻尼系数lamda混合得到对的H2矩阵
H2=H1+(lamda*eye(2,2));
 
i=1;
jj=0;
 
while((sum((G1).^2))^(1/2) >= 1e-1) %如果梯度太小，那么停止迭代
    jj=jj+1;
    jj
    delta=double(inv(H2)*G1)';
    B2=B1-delta*0.1;
    f = 1./(X1+B(1))+X2.^2./(B(2)+X2.^2);
    r=Y-f;
    error=r'*r;
    error1=subs(error,B,B2);
    errorsum=sum(error1(:).^2);
    
    
    if (last_errorsum-errorsum)>0   %step acceptable
        vv = 2;
        lamda = lamda/3;                    %减小lamda，更接近高斯牛顿法，会更快下降
        lamda
        B1=B2;
        H1=H2;
        last_errorsum = errorsum;
        errorsum1=[errorsum1 errorsum];
        i
        i=i+1;                        %真正成功迭代的次数
        updateJ=1;
    else                                %说明目标函数反而上升了，不能接受
        lamda = lamda*vv;   %赶紧增大lamda,更接近于梯度下降
        vv=vv*5;
        lamda
        if lamda>100            %说明当前已经处于了最小值的沟谷里了，没必要再优化了，因为始终走不出来
            break;                     %跳出循环
        end
        updateJ=0;
    end
    if updateJ==1
        r=reshape(r,25,1);
        J=jacobian(r,B);
        G=2.*(J'*r);
        G1=subs(G,B,B2);
        H=2.*(J'*J);
        H1=subs(H,B,B2);
    end
    % 根据阻尼系数lamda混合得到对的H1矩阵
    H2=H1+(lamda*eye(2,2));
end
Q1=1:0.1:10;
Q2=1:0.1:10;
[P1 P2]=meshgrid(Q1,Q2);
s=1./(P1+B1(1))+P2.^2./(P2.^2+B1(2));
s1=1./(P1+0.5)+P2.^2./(P2.^2+2);
figure(1);
surf(P1,P2,s);
hold on;
surf(P1,P2,s1);
hold on;
scatter3(S1,S2,S3,'r');
xlabel('x1');
ylabel('x2');
zlabel('y');
title(['LM method 原始曲面a=0.5 b=2 迭代起始点a=20 b=20 和拟合出曲面a=' num2str(B1(1,1)) 'b=' num2str(B1(1,2))],'fontsize',12,'color','r');
hold on;
figure(2);
step=1:i;
plot(step,errorsum1');
xlabel('迭代次数');
ylabel('误差值');
title('LM method error','fontsize',12,'color','r');
B1