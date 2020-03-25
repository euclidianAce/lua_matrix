#!/usr/bin/env lua

--local matrix = require "lua_matrix"

for n = 3, 5 do
	local simplex = matrix.random(n, n+1)
	local ok, rot = pcall(matrix.rotation, simplex, math.pi/3)
	if not ok then
		print(rot)
		break
	end
	if n < 10 then
		for r, row in rot:rows() do
			io.write(("%12.1f\t"):rep(#row):format(table.unpack(row)) .. "\n")
		end
	end
	print(n)
	collectgarbage()
end