#!/home/corey/codingThings/lua-5.3.5/src/lua

local matrix = require "lua_matrix";

local function test(func, args, res, str)
	results = {pcall(func, table.unpack(args))}
	ok, err = results[1], results[2]
	print(
		(
			"[%s%s%s] %s %s"
		):format(
			string.char(27).."["..(ok==res and "32" or "31")..";1m",
			ok==res and "Passed" or "Failed",
			string.char(27).."[0m",
			str,
			(not (ok==res) or not ok) 
			and "\n         [" .. string.char(27) .. "[31m" .. "Error Message" .. string.char(27) .. "[0m] " .. err 
			or ""
		)
	)
	if ok then
		table.remove(results, 1)
		return table.unpack(results)
	end
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

function printMatrix(m, name)
	local rows, columns = m:size()
	for i = 1, rows do
		for j = 1, columns do
			print(("%s[%d][%d] = %.1f"):format(name, i, j, m:get(i,j)))
		end
	end
	print()
end

math.randomseed(os.time())





local rows = math.random(2, 10)
local columns = math.random(2, 10)

printTitle("Constructors")
local a = test(
	matrix.new, {rows, columns}, true,
	("Should make a zero matrix of variable size (%d x %d)"):format(
		rows, columns
	)
)

local b = test(
	matrix.new, { {1,2,3,4,5,6,7,8,9}, 3}, true,
	"Should make a matrix initialized by a table and an int"
)

test(
	matrix.new, { {1,2,3,4,5,6,7}, 2}, false,
	"Should error when a table cant be divided into a certain amount of rows"
)

local c = test(
	matrix.new, {{ {1,2,3}, {1,2,3}, {1,2,3} }}, true,
	"Should make a matrix initialized by a table of tables"
)

test(
	matrix.new, {{ {1,2,3}, {1,2,3,4}, {1,2} }}, false,
	"Should error when a table of tables has inconsistent length"
)

local id = test(
	matrix.identity, {rows}, true,
	"Identity matrix constructor should not error"
)

test(
	function()
		local r, c = id:size()
		for i = 1, r do
			for j = 1, c do
				if i == j and id:get(i, j) ~= 1 then
					return false
				elseif id:get(i, j) ~= 0 then
					return false
				end
			end
		end
	end, {}, true,
	"Identity matrix should have 1s along diagonal and 0 otherwise"
)

test(
	function()
		local r, c = a:size()
		return r == rows and c == columns
	end, {}, true,
	"Should correctly get the size of a given matrix"
)

test(
	function()
		local r, c = a:size()
		for i = 1, r do
			for j = 1, c do
				a:set(i, j, 5)
			end
		end
	end, {}, true,
	"Should set every entry in a given matrix to 5"
)







printTitle("Metamethods")

-- unary minus
test(
	function()
		local a = -a
	end, {}, true,
	"Unary minus (__unm) should not error"
)

test(
	function()
		local b = -a
		local r, c = a:size()
		for i = 1, r do
			for j = 1, c do
				if b:get(i, j) ~= -a:get(i, j) then
					return false
				end
			end
		end
		return true
	end, {}, true,
	"Unary minus (__unm) should negate each entry correctly"
)


--__add
test(
	function()
		local d = matrix.new(rows,columns) + matrix.new(rows,columns)
	end, {}, true,
	"Addition (__add) of two matrices of the same size should not fail"
)

test(
	function()
		local d = matrix.new(rows, columns+1) + matrix.new(rows, columns)
	end, {}, false,
	"Addition (__add) of two matrices of different sizes should error"
)

--__sub
test(
	function()
		local d = matrix.new(rows,columns) + matrix.new(rows,columns)
	end, {}, true,
	"Subtraction (__sub) of two matrices of the same size should not fail"
)

test(
	function()
		local d = matrix.new(rows, columns+1) + matrix.new(rows, columns)
	end, {}, false,
	"Subtraction (__sub) of two matrices of different sizes should error"
)

--__mul

test(
	function()
		local a = 5*a
	end, {}, true,
	"Number Matrix multiplication (__mul) should not error"
)

test(
	function()
		local a = a*5
	end, {}, true,
	"Matrix Number multiplication (__mul) should not error"
)

test(
	function()
		local x, y, z = math.random(1,10), math.random(1, 10), math.random(1, 10)
		local a = matrix.new(x, y) * matrix.new(y, z)
	end, {}, true,
	"Matrix Matrix multiplication (__mul) should not error with matrices of the correct size"
)


test(
	function()
		local x, y, z = math.random(1,10), math.random(1, 10), math.random(1, 10)
		local a = matrix.new(x, y) * matrix.new(y+1, z)
	end, {}, false,
	"Matrix Matrix multiplication (__mul) should error when two matrices of incorrect sizes are multiplied"
)

--__div

test(
	function()
		local a = a/5
	end, {}, true,
	"Matrix Number division (__div) should not error"
)

--__idiv

test(
	function()
		local a = a//5
	end, {}, true,
	"Matrix Number integer division (__idiv) should not error"
)


--__mod

test(
	function()
		local a = a%3
	end, {}, true,
	"Matrix Number modulus (__mod) should not error"
)

--__pow

test(
	function()
		local a = a^3
	end, {}, true,
	"Matrix Integer power (__pow) should not error"
)

printTitle("Indexing")
--__index

test(
	function()
		local x = a[1][1]
	end, {}, true,
	"Matrix indexing should not error"
)

test(
	function()
		local r, c = a:size()
		for i = 1, r do
			for j = 1, c do
				if a[i][j] ~= a:get(i, j) then
					return false
				end
			end
		end
		return true
	end, {}, true,
	"Matrix indexing should return the same value as the get() function"
)

--__newindex

test(
	function()
		local x = math.random()
		a[1][1] = x
		return a:get(1,1) == x
	end, {}, true,
	"Should be able to set matrix entries using table syntax (a[1][1] = 3)"
)


printTitle("Iterators")

-- Rows
test(
	function()
		local b = a:rows()
	end, {}, true,
	"Rows iterator should not error when called"
)

test(
	function()
		return type(a:rows()) == "function" or error(type(a:rows()))
	end, {}, true,
	"Rows iterator should generate a function"
)

test(
	function()
		local rowCount = 0
		for k, v in a:rows() do
			rowCount = rowCount+1
		end
		local r = a:size()
		return rowCount == r
	end, {}, true,
	"Rows should iterate through the correct amount of rows"
)

test(
	function()
		local _, c = a:size()
		for k, v in a:rows() do
			if type(v) ~= "table" then
				error("Didn't return table")
			elseif #v ~= c then
				error("Incorrect length")
			end
		end
	end, {}, true,
	"Rows iterator should return tables of the correct length"
)

test(
	function()
		for k, v in a:rows() do
			for i = 1, #v do
				if v[i] ~= a:get(k, i) then
					error(("Entries don't match a:get(%d, %d) = %f, iterator value = %f"):format(r, c, a:get(r, c), v[i]))
				end
			end
		end
	end, {}, true,
	"Rows iterator's table values should match values from __index:get()"
)

-- Columns
test(
	function()
		local b = a:cols()
	end, {}, true,
	"Columns iterator should not error when called"
)

test(
	function()
		return type(a:cols()) == "function" or error(type(a:cols()))
	end, {}, true,
	"Columns iterator should generate a function"
)

test(
	function()
		local colCount = 0
		for k, v in a:cols() do
			colCount = colCount+1
		end
		local _, c = a:size()
		return colCount == c
	end, {}, true,
	"Columns should iterate through the correct amount of Columns"
)

test(
	function()
		local r = a:size()
		for k, v in a:cols() do
			if type(v) ~= "table" then
				error("Didn't return table")
			elseif #v ~= r then
				error("Incorrect length")
			end
		end
	end, {}, true,
	"Columns iterator should return tables of the correct length"
)

test(
	function()
		for c, t in a:cols() do
			for r = 1, #t do
				if t[r] ~= a:get(r, c) then
					error(("Values don't match a:get(%d, %d) = %f, iterator value = %f"):format(r, c, a:get(r, c), t[r]))
				end
			end
		end
	end, {}, true,
	"Columns iterator's table values should match values from __index:get()"
)

-- Entries
test(
	function()
		local b = a:entries()
	end, {}, true,
	"Entries iterator should not error when called"
)

test(
	function()
		return type(a:entries()) == "function" or error(type(a:entries()))
	end, {}, true,
	"Entries iterator should generate a function"
)

test(
	function()
		local eCount = 0
		for k, v in a:entries() do
			eCount = eCount+1
		end
		local r, c = a:size()
		return eCount == r*c
	end, {}, true,
	"Entries should iterate through the correct amount of Entries"
)

test(
	function()
		for r, c, e in a:entries() do
			if a:get(r, c) ~= e then
				error(("Entries don't match a:get(%d, %d) = %f, entries = %f"):format(r, c, a:get(r, c), e))
			end
		end
	end, {}, true,
	"Entries iterator values should match values from __index:get()"
)

test(
	function()
		local f = a.map
		return f ~= nil
	end, {}, true,
	"Map should exist"
)

test(
	function()
		local a = a:map(function() return 4 end)
	end, {}, true,
	"Map should not error when function doesnt need to take arguments"
)

test(
	function()
		local a = a:map(function(x) return x end)
	end, {}, true,
	"Map should not error when given the identity function"
)

test(
	function()
		return a:map(function(x) return {x} end)
	end, {}, false,
	"Map should error when the function passed doesn't return a number"
)

test(
	function()
		local b = a:map(function(x)
			return x+1
		end)
		for r, c, e in a:entries() do
			if e+1 ~= b:get(r,c) then
				error("entries dont match")
			end
		end
	end, {}, true,
	"Map should return a matrix with correct values"
)

test(
	function()
		local r, c = 1, 1
		local rows, columns = a:size()
		local b = a:map(function(entry, row, column)
			if r ~= row or r < 1 then
				error("Wrong row value passed in")
			end
			if c ~= column or c < 1 then
				error("Wrong column value passed in")
			end
			c = c+1
			if c > columns then
				c = 1
				r = r+1
			end
			return entry
		end)
	end, {}, true,
	"Map should pass the correct arguments into the function"
)




