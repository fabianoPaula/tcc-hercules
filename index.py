import numpy as np
import scipy.stats as sp

import matplotlib.pyplot as plt

data = [41.91,41.91,43.16,45.81,46.56,46.56,46.56,46.56,47.06,48.02,48.02,48.08,
48.08,50.00,50.00,50.58,50.58,50.95,51.20,51.93,52.22,52.22,52.41,52.41,
52.59,
52.65,
52.65,
52.65,
52.95,
54.13,
54.13,
55.61,
55.61,
55.61,
55.66,
55.91,
55.91,
55.91,
56.49,
57.59,
58.61,
58.61,
61.29,
61.29,
71.12,
71.58,
71.58,
73.15,
73.15,
76.59,
76.59,
78.80,
78.80,
83.09,
83.09,
83.09,
85.22,
92.00,
94.63,
94.63,
96.27,
99.01,
99.01,
100.72,
100.72,
100.72,
102.05,
102.05,
103.30,
103.30,
103.30,
103.30,
103.72,
103.72,
103.72,
103.72,
103.98,
104.26,
104.26,
105.31,
106.52,
110.62,
110.62,
117.33,
118.68,
118.68,
125.52,
125.52,
126.91,
130.77] 

data.sort()

if len(data) % 1 == 1:
	p50 = data[len(data)/2]
else:
	p50 = (data[len(data)/2] + data[len(data)/2- 1])/2

mean_data = np.mean(data)
	
sd = 0 
sd2 = 0
for elem in data:
	sd = sd + (elem - mean_data)*(elem - mean_data)
	sd2 = sd2 + (elem - mean_data)*(elem - mean_data)/len(data)

def mysqrt(number):
	after = 0
	s = number/2

	while( np.abs(s - after) > 0.001):
		print("Sqrt - 1: %f " % s)
		after = s
		s = (s + number/s)*0.5
		print("Sqrt - 2: %f " % after)
		print("Sqrt - 3: %f " % s)
		print("Sqrt - 4: %f " % (s - after))
	
	print("Sqrt: %f" % s)
	return s


print("Mean: %f" % (np.mean(data)))
print("Test: %f" % sd2)
print("Test: %f" % mysqrt(sd2))
print("Numpy: %f" % np.sqrt(sd2))
print("StandardDesviation: %f" % (np.std(data)))
print("p50: %f" % (p50))
print("Skew: %f" % (sp.skew(data)))

print("Test: %f" % mysqrt(2))
# print("Test: %f" % np.sqrt(2))


# plt.hist(data, bins=30)
# plt.show()