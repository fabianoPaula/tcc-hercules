# squares = [i*i for i in range(25)]

# def sqrt(number):
# 	sqrt = 0

# 	for i in range(24):
# 		if( i*i <= number and number < (i+1)*(i+1)):
# 			sqrt = i
# 			break

# 	while ( sqrt*sqrt < number):
# 		sqrt = sqrt + .01
# 	return sqrt - 0.01

# # print(1,sqrt(1), sqrt(1)*sqrt(1))
# print(2,sqrt(2), sqrt(2)*sqrt(2))
# 	# print(3,sqrt(3), sqrt(3)*sqrt(3))
# 	# print(4,sqrt(4), sqrt(4)*sqrt(4))
# 	# print(5,sqrt(5), sqrt(5)*sqrt(5))
# 	# print(6,sqrt(6), sqrt(6)*sqrt(6))
# 	# print(7,sqrt(7), sqrt(7)*sqrt(7))
# 	# print(8,sqrt(8), sqrt(8)*sqrt(8))
# 	# print(9,sqrt(9), sqrt(9)*sqrt(9))

import numpy as np

from scipy.stats import skew 
from scipy.stats import kurtosis


data = [1.0,1.0,1.0,1.0,2.0,2.0,2.0,2.0]

print("Mean: ", np.mean(data))
print("Std:  ", np.std(data))
print("Kurtosis: ", kurtosis(data))
print("Skew: ", skew(data))

print("")
print("")

print("Percentil 10", np.percentile(data, 10))
print("Quartile  25", np.percentile(data, 25))
print("Quartile  75", np.percentile(data, 75))
print("Percentil 90", np.percentile(data, 90))

# for i in range(1000):
# 	if( i*i < 2**16-1):
# 		print(i)
