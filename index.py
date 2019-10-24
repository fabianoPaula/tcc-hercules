import numpy as np
import scipy.stats as sp

import matplotlib.pyplot as plt

data = [
44.65,
44.77,
44.77,
45.09,
46.18,
46.20,
46.20,
46.20,
46.20,
46.61,
46.61,
47.40,
47.40,
47.40,
47.40,
47.59,
47.63,
47.65,
47.72,
47.72,
48.65,
48.65,
48.65,
48.65,
48.70,
48.70,
48.79,
48.79,
48.79,
48.79,
48.79,
48.79,
48.79,
48.86,
48.86,
49.09,
49.09,
49.09,
49.15,
49.54,
49.54,
49.54,
49.93,
49.93,
49.93,
50.11,
51.95,
51.95,
51.95,
52.09,
52.45,
52.45,
52.66,
52.66,
54.43,
54.43,
54.43,
56.91,
58.13,
64.86,
66.38,
67.93,
67.93,
68.54,
69.94,
73.16,
74.95,
74.95,
75.80,
76.77,
76.77,
78.72,
80.33,
80.33,
80.33,
81.19,
81.34,
81.55,
81.75,
83.29,
83.29,
85.94,
85.94,
85.94,
85.94,
86.16,
86.55,
86.55,
86.55,
86.55,
86.55,
86.91,
86.91,
87.98,
88.22,
91.73,
91.73,
91.73,
92.69,
95.68,
100.84,
100.84,
101.47,
101.47,
101.47,
101.47,
104.02,
104.02,
123.66,
123.66,
124.75,
124.75,
128.64,
128.64,
128.64,
128.64,
132.57,
132.57, ] 

data.sort()



# def myskew(data):


def mysqrt(number):
	after = 0
	s = number/2

	while( np.abs(s - after) > 0.001):
		# print("Sqrt - 1: %f " % s)
		after = s
		s = (s + number/s)*0.5
		# print("Sqrt - 2: %f " % after)
		# print("Sqrt - 3: %f " % s)
		# print("Sqrt - 4: %f " % (s - after))
	
	# print("Sqrt: %f" % s)
	return s

def print_test(data):
	print("Data Size: %d" % (len(data)))

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

	print("Mean: %f" % (np.mean(data)))
	# print("Sd2: %f" % sd2)
	# print("Sd2^1/2: %f" % mysqrt(sd2))
	# print("Numpy Sd2^1/2: %f" % np.sqrt(sd2))
	# print("p50: %f" % p50)
	print("StandardDesviation: %f" % (np.std(data)))
	print("Skew: %f" % (3*(np.mean(data) - p50)/mysqrt(sd2)))
	print("Skew: %f" % (sp.skew(data)))

	plt.hist(data)
	plt.show()


print_test(data)
print_test(data[0:65])
print_test(data[0:46])
print_test(data[54:57])


# plt.hist(data, bins=30)
# plt.show()