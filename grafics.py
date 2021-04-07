import matplotlib.pyplot as plt
import numpy as np 

x = []
yexp = []
yreal = []
errs = []
errs_r = []

with open("results.txt", "r") as o:
	lines = o.read().split("\n")[:-1]
	for line in lines:
		nums = line.split(",")
		n, exp, real, err = nums
		print(n, exp, real, err)

		x.append(int(n))
		yexp.append(float(exp))
		yreal.append(float(real))
		errs.append(float(err))
		errs_r.append(float(err)/float(real))


x = np.array(x)
yexp = np.array(yexp)
yreal = np.array(yreal)
errs = np.array(errs)
errs_r = np.array(errs_r)


plt.plot(x, yreal, label = "Experimental results")
plt.plot(x, yexp, label = "e/sqrt(pi) (2/n)^(1/4)")
plt.legend()
plt.xlabel("n (2n is the number of roommates)")
plt.ylabel("p (probability of having a solution)")
plt.show()

plt.plot(x, errs, label = "Absolute error")
plt.plot(x, errs_r, label = "Relative error")
plt.legend()
plt.yscale('log')
plt.xlabel("n (2n is the number of roommates)")
plt.ylabel("error")
plt.show()
