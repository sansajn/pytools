import math

def print_vector(vec):
	print(vec)
	print('number of elements: %d' % len(vec))

def simple_return():
	return 101

def complex_return():
	return 'hello', 101, (3, 2, 1)

def return_list():
	return [102, 'hello', (1, 2, 1)]

def return_dict():
	return {'one':1, 'two':2, 'three':3}

def simple_function():
	print('Hello from python')

def call_integer_func(n):
	print('arg:%d' % n)

def multiple_argument_call(a, b, c, d, e, f):
	print('multiple_argument_call(a, b, c, d, ...)')
	print('  ', a, ':', type(a))
	print('  ', b, ':', type(b))
	print('  ', c, ':', type(c))
	print('  ', d, ':', type(d))
	print('  ', e, ':', type(e))
	print('  ', f, ':', type(f))

def list_return():
	return [101, 101.101]

def tuple_return():
	return 102, 102.102

def integer_return():
	return 202

def list_return_complex():
	return [101, 101.101, (1, 0, 1)]

def sin(x):
	return math.sin(math.degrees(x))

def print_coordinates(coord):
	print('lat:%g, lon:%g' % coord)

