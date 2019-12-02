#!/home/corey/codingThings/lua_matrix/src/lua-5.3.5/src/lua

local matrix = require("lua_matrix")
math.randomseed(os.time())

local function benchmark(func, args, str)
	local clock = os.clock
	local startTime = clock()
	ok, err = pcall(func, table.unpack(args))
	local endTime = clock()
	print(("[%s%s%s] <%.3e s> %s"):format(
		string.char(27).."[1m",
		"Bench",
		string.char(27).."[0m",
		endTime-startTime, 
		str
	))
end
local mtest, mout, t

for n = 1, 7 do
	mtest = matrix.new({math.random(), math.random(), math.random(), math.random()}, 2)
	mout = matrix.identity(2)
	t = 10^n
	benchmark(
		function()
			for i = 1, t do
				mout = mout*mtest
			end
		end, {},
		("Multiplying a random 2x2 matrix by itself 10^%d times"):format(n)
	)

	mtest = matrix.new({math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),}, 3)
	mout = matrix.identity(3)
	benchmark(
		function()
			for i = 1, t do
				mout = mout*mtest
			end
		end, {},
		("Multiplying a random 3x3 matrix by itself 10^%d times"):format(n)
	)

	mtest = matrix.new({math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),math.random(),}, 4)
	mout = matrix.identity(4)
	benchmark(
		function()
			for i = 1, t do
				mout = mout*mtest
			end
		end, {},
		("Multiplying a random 4x4 matrix by itself 10^%d times"):format(n)
	)

	mtest = matrix.new(
		{math.random(),math.random(),math.random(),math.random(),math.random(),
		 math.random(),math.random(),math.random(),math.random(),math.random(),
		 math.random(),math.random(),math.random(),math.random(),math.random(),
		 math.random(),math.random(),math.random(),math.random(),math.random(),
	 	 math.random(),math.random(),math.random(),math.random(),math.random(),}, 5)
	mout = matrix.identity(5)
	benchmark(
		function()
			for i = 1, t do
				mout = mout*mtest
			end
		end, {},
		("Multiplying a random 5x5 matrix by itself 10^%d times"):format(n)
	)
end

