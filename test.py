def print_vector(vec):
	print(vec)
	print('number of elements: %d' % len(vec))

def simple_return():
	return 101

def complex_return():
	return 'hello', 101, (3, 2, 1)

def simple_function():
	print('Hello from python')

def call_integer_func(n):
	print('arg:%d' % n)

def multiple_argument_call(a, b, c, d, e, f, g, h):
	print('multiple_argument_call(a, b, c, d, ...)')
	#print(a, ':', type(a))
	#print(b, ':', type(b))
	#print(c, ':', type(c))
	#print(d, ':', type(d))
	print(a)
	print(b)
	print(c)
	print(d)
	if e != None:
		print(e)
	if f != None:
		print(f)
	if g != None:
		print(g)
	if h != None:
		print(h)

