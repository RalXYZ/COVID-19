from math import *

# 第一份资源文件
for i in range(0, 30):
	print("3-" + str(i + 1), end = ' ')
	print(int(50 * (sin(0.1 * i) + 1)), end = ' ')
	print(int(50 * (cos(0.3 * i) + 1)), end = ' ')
	print(int(100 * log(0.1 * i + 1)), end = ' ' )
	print(int(20 * sqrt(i)), end = '\n' )

# 第二份资源文件
for i in range(0, 30):
	print("3-" + str(i + 1), end = ' ')
	print(int(50 * pow(sin(0.1 * i), 2)), end = ' ')
	print(int(50 * pow(cos(0.3 * i), 2)), end = ' ')
	print('20', end = ' ' )
	print(int(7 * sqrt(i)), end = '\n' )

