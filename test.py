def dx(x):
    return -x*x

x = float(input('x0: '))

# dx/dt=-x^2; x^-2 dx = -dx; x^-3 = 3x+C; x = 1 / cubt(3t+C); x*t = t / cubt(3t+C)

dt = 0.01

for i in range(10000):
    x += dx(x) * dt
    print(i * dt * x)
