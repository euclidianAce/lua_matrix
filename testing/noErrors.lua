#!/bin/env lua
--local matrix = require "lua_matrix"

local function batch(func, argBatch, res, str)
	io.write(
		("[%s] %s"):format(
			string.char(27).."[1m......"..string.char(27).."[0m",
			str
		)
	)
	io.flush()
	local errors = {}
	for i, v in ipairs(argBatch) do
		local ok, err = pcall(func, table.unpack(v))
		if ok ~= res then
			table.insert(errors, "Error: "..(err or "No error message"))
		end
	end

	if #errors > 0 then -- test failed
		io.write(
			("\r[%s] %s \n  (%d/%d) %s%s\n"):format(
				string.char(27).."[31;1mFailed"..string.char(27).."[0m",
				str,
				#errors, #argBatch, " Tests failed with the following errors\n    ", table.concat(errors, "\n    ")
			)
		)
	else
		io.write(
			("\r[%s] %s\n"):format(
				string.char(27).."[32;1mPassed"..string.char(27).."[0m",
				str
			)
		)
	end
	errors = nil
end

local titlebarLen = 20
local function printTitle(str)
	print(table.concat{
		string.rep("-", titlebarLen),
		string.rep(" ", (2*titlebarLen-#str)//2),
		str,
		string.rep(" ", (2*titlebarLen-#str)//2),
		string.rep("-", titlebarLen)
	})
end

math.randomseed(os.time())



local ints = {}
for i = 1, 20 do
	table.insert(ints, {i})
end
local intPairs = {}
for i = 1, 20 do
	for j = 1, 20 do
		table.insert(intPairs, {i, j})
	end
end
local intTriplets = {}
for i = 1, 20 do
	for j = 1, 20 do
		for k = 1, 20 do
			table.insert(intTriplets, {i, j, k})
		end
	end
end

local intTablePairs = {}
local tableIntPairs = {}
local function range(n)
	local t = {}
	for i = 1, n do
		table.insert(t, i)
	end
	return t
end
for i = 1, 20 do
	local t = range(i*20)
	table.insert(intTablePairs, {i, t})
	table.insert(tableIntPairs, {t, i})
end

local tablesOfTables = {}
for i = 1, 20 do
	local t = {}
	for j = 1, i do
		table.insert(t, range(i))
	end
	table.insert(tablesOfTables, {t})
end

printTitle("CONSTRUCTORS")

batch(matrix.new, intPairs, true, "matrix.new should not error when given two integers")
batch(matrix.random, intPairs, true, "matrix.random should not error when given two integers")
batch(matrix.identity, ints, true, "matrix.identity should not error when given an integer")
batch(matrix.new, intTablePairs, true, "matrix.new should not error given (int, table)")
batch(matrix.new, tableIntPairs, true, "matrix.new should not error given (table, int)")
batch(matrix.new, tablesOfTables, true, "matrix.new should not error given a table of tables")
batch(function(size)
	local m = matrix.identity(size)
	for i = 1, size do
		for j = 1, size do
			local val = m:get(i,j)
			if i == j then
				if val ~= 1 then
					error(("Entry %d, %d ~= 1, instead = %f"):format(i, j, val))
				end
			else
				if val ~= 0 then
					error(("Entry %d, %d ~= 0, instead = %f"):format(i, j, val))
				end
			end
		end
	end
end, ints, true, "matrix.identity should produce a matrix with 1s along the diagonal and 0s elsewhere")


printTitle("ARITHMETIC")

batch(function(i, j)
	local a = matrix.random(i, j) + matrix.random(i, j)
end, intPairs, true, "Addition should not error given correct size matrices")

batch(function(i, j)
	local a = matrix.random(i, j) - matrix.random(i, j)
end, intPairs, true, "Subtraction should not error given correct size matrices")

batch(function(i, j)
	local a = i * matrix.random(i, j)
end, intPairs, true, "Number Matrix multiplication should not error")
batch(function(i, j)
	local a = matrix.random(i, j) * i
end, intPairs, true, "Matrix Number multiplication should not error")
batch(function(i, j, k)
	local a = matrix.random(i, j) * matrix.random(j, k)
end, intTriplets, true, "Matrix Matrix multiplication should not error")
batch(function(i, j, k)
	local a = matrix.random(i, j) * matrix.random(j+1, k)
end, intTriplets, false, "Matrix Matrix multiplication should error given the wrong size of matrices")
batch(function(i, j)
	local a = matrix.random(i, j) / i
end, intPairs, true, "Matrix number division should not error")

batch(function(i)
	local a = matrix.random(i, i) ^ i
end, ints, true, "Square Matrix int exponents should not error")

batch(function(i)
	local a = matrix.random(i, i+1) ^ i
end, ints, false, "Non-square matrix int exponent should error")

printTitle("METAMETHODS")

batch(function(i, j)
	local a = matrix.random(i,j)
	for i = 1, i do
		for j = 1, j do
			a:set(i,j, 1)
		end
	end
end, intPairs, true, "Matrix:set should not error when within bounds")

batch(function(i, j)
	local a = matrix.random(i,j)
	for i = 1, i do
		for j = 1, j do
			local b = a:get(i,j)
		end
	end
end, intPairs, true, "Matrix:get should not error when within bounds")

batch(function(i, j)
	local a = matrix.random(i, j)
	a:set(i+1, j+1, 1)
end, intPairs, false, "Matrix:set should error when out of bounds")

batch(function(i, j)
	local a = matrix.random(i, j)
	local b = a:get(i+1, j+1)
end, intPairs, false, "Matrix:get should error when out of bounds")


batch(function(i, j)
	local a = matrix.random(i, j):schur( matrix.random(i, j) )
end, intPairs, true, "Matrix schur (hadamard) product shoud not error given correct size matrices")
printTitle("ROTATION")

-- create a batch of vectors to use

local function range(n)
	local t = {}
	for i = 1, n do
		table.insert(t, i)
	end
	return t
end

local rowVecs = {}
for i = 2, 20 do
	table.insert(rowVecs, { matrix.new(range(i), 1) } )
end

local colVecs = {}
for i = 2, 20 do
	table.insert(colVecs, { matrix.new(1, range(i)) } )
end
-- vector
batch(matrix.translation, rowVecs, true, "matrix.translation should not error given row vectors")
batch(matrix.translation, colVecs, true, "matrix.translation should not error given column vectors")
-- size, axis1, axis2
local sizeAxesAngle = {}
for i = 2, 10 do -- size
	for j = 1, i do -- axis 1
		for k = 1, i do -- axis 2
			for l = 1, 10 do
				table.insert(sizeAxesAngle, {i, j, k, math.random()})
			end
		end
	end
end
batch(matrix.mainRotation, sizeAxesAngle, true, "matrix.mainRotation should not error given a size and axes within that size")
--]]
local simplexesAngle = {}

for i = 1, 20 do
	table.insert(simplexesAngle, {
		matrix.random(i,i+1),
		math.random() * math.pi * 2
	})
end

batch(matrix.rotation, simplexesAngle, true, "matrix.rotation should not error given correctly sized simplexes and an angle ")
