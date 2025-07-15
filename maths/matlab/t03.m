x=linspace(0,2*pi,64);
y=round(sin(x)*128)+128;
[reshape(y,16,4)]'
plot(x,y);