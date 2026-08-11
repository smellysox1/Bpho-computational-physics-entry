N = 1000;
T = 100;
m = 28.96e-3 / 6.02e23;
M = 10*m;
r = 0.16;
R = 10*r;
a = 7*R;

C = 1;

k_B = 1.38e-23;

v = sqrt(3*k_B*(T+273)/m);
V = sqrt(3*k_B*(T+273)/M);

Kn = 15;

tmax = 200;

v = v/1000; V = V/1000;

dt = 0.01*Kn*r/v;

figure('color', [1 1 1], 'renderer', 'opengl', 'name', 'brownian motion',...
       'units','normalized','position',[0.2,0.2,0.6,0.6]);
set(gca, 'nextplot','add');

tit = title('Brownian motion simulation: t = 0s','FontSize', 3);

plot([0, a, a, 0, 0],[0, 0, a, a, 0], 'k-', 'LineWidth', 3);

axis equal; axis off; axis manual;

stopstop=0;

while stopstop == 0
    X = 0.5 * a; Y = 0.5 * a;
    theta = 2 * pi * rand;
    Vx = V * cos(theta);
    Vy = V * sin(theta);

    x = zeros(1, N);
    y = zeros(1, N);

    for n=1:N
        d=0;
        while d < (r + R)
            x(n) = r + rand*(a-2*r);
            y(n) = r + rand*(a-2*r);
            [dhat, d] = ball_displacement(x(n), y(n), X, Y);
        end
    end

    for n=1:N
        theta = 2*pi*rand;
        vx(n) = v * cos(theta);
        vy(n) = v * sin(theta);
    end

    theta = linspace(0, 2*pi, 500);
    xc = R*cos(theta);
    yc = R*sin(theta);

    large = plot(X + xc, Y + yc, 'r', 'LineWidth', 2);
    small = plot(x, y, 'b*');

    XX = X; YY = Y;
    snail = plot(XX, YY, 'r-');
    snail0 = plot(X, Y, 'g*');
    snail_latest = plot(X, Y, 'r*');

    stop = 0; t=0; tt=0;
    while stop == 0
        t = t + dt;
        tt = tt + dt;

        X = X + Vx * dt;
        Y = Y + Vy * dt;
        x = x + vx * dt;
        y = y + vy * dt;

        XX = [XX, X];
        YY = [YY, Y];

        for n=1:N
            [Vx, Vy, vx(n), vy(n), X, x(n), Y, y(n)] =...
                bounce(X, Y, x(n), y(n), Vx, vx(n), Vy, vy(n), C, M, m, R, r);
        end

        if (tt > (Kn * r / v))
            tt = 0;
            for n=1:N
                theta = 2*pi*rand;
                vx(n) = v * cos(theta);
                vy(n) = v * sin(theta);
            end
        end

        set(large, 'xdata', X + xc, 'ydata', Y + yc);
        set(snail, 'xdata', XX, 'ydata', YY);
        set(snail_latest, 'xdata', X, 'ydata', Y);
        set(small, 'xdata', x, 'ydata', y);
        set(tit, 'string', ['Brownian motion simulation: t =', num2str(t, 3), 'ps']);
        drawnow;

        if (t>tmax)
            stop = 1;
            print(gcf, 'brownian motion.png', '-dpng', '-r300');
        end
    end
    stop = 0;
    delete(large);
    delete(small);
    delete(snail);
    delete(snail0);
    delete(snail_latest);
end


function [dhat, d] = ball_displacement(x1, y1, x2, y2)
d = sqrt((x2-x1)^2 + (y2-y1)^2);
dhat = [x2-x1;y2-y1]/d;

end

function [vx1, vy1, vx2, vy2, x1, x2, y1, y2] =...
    bounce(x1, y1, x2, y2, ux1, ux2, uy1, uy2, C, M1, M2, R1, R2)

u1 = [ux1;uy1];
u2 = [ux2;uy2];

vx1 = ux1;
vx2 = ux2;
vy1 = uy1;
vy2 = uy2;

[dhat, d] = ball_displacement(x1, x2, y1, y2);

if (d <= (R1 + R2))
    delta = (R1 + R2 - d) / 2;

    r1 = [x1;y1] - delta * dhat;
    x1 = r1(1);
    y1 = r1(2);

    r2 = [x2;y2] + delta * dhat;
    x2 = r2(1);
    y2 = r2(2);

    if (dot(u2 - u1, dhat)) < 0
        V = (M1 * u1 + M2 * u2) / (M1 + M2);

        v1 = V - C * (u1 - V); vx1 = v1(1); vy1 = v1(2);
        v2 = V - C * (u2 - V); vx2 = v2(1); vy2 = v2(2);
    end
end

end