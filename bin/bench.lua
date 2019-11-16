local function benchmark(func, args, str)
	local startTime = os.clock()
	ok, err = pcall(func, table.unpack(args))
	local endTime = os.clock()
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
end

