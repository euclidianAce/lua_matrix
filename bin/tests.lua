#!/bin/env lua
local matrix = require "lua_matrix"

local function batch(func, argBatch, res, str)
	io.write(
		("[%s] %s     "):format(
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
		collectgarbage()
		collectgarbage()
	end
	
	if #errors > 0 then -- test failed
		io.write(
			("\r[%s] %s \n  (%d/%d) %s%s\n"):format(
				string.char(27).."[31;1mFailed"..string.char(27).."[0m",
				str,
				#errors, #argBatch, " tests failed\n    ", table.concat(errors, "\n    ")
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


