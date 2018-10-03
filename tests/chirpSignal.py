from math import pi, cos
import matplotlib.pyplot as plt

def digitalChirp(f_start, f_end, interval, n_steps):
	y = []
	t_arr = []
	for i in range(n_steps):
		delta = i / float(n_steps)
		t = interval * delta
		t_arr.append(t)
		phase = 2 * pi * t * (f_start + (f_end - f_start) * delta / 2)
		y.append(cos(phase))

	return t_arr, y

def digitalFMPeeble(f_start, f_end, T):
	y = []
	t = []
	delta_f = f_end - f_start
	N = (int)(delta_f*T) * 20

	T_1 = T/((float)(N))
	for n in range(1, N+1):
		t_n = (n - ((N+1)/2.0))*T_1
		t.append(t_n)

		phase = 2*pi*t_n*(0*delta_f + delta_f*(t_n)/(2*T))
		y.append(cos(phase))

	return t, y

def getNames():
	name = "Lebohang"
	surname = "Mbele"

	return name, surname

def main():
	t1, chirpSignal = digitalChirp(1, 10, 5, 1000)
	t2, FMPeeble = digitalFMPeeble(1, 10, 5)

	plt.subplot(211)
	plt.plot(t1, chirpSignal)

	plt.subplot(212)
	plt.plot(t2, FMPeeble)
	plt.show()

main()