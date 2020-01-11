#!/bin/env lua

local matrix = require "lua_matrix"

function printMatrix(m)
	for r, row in m:rows() do
		print(table.unpack(row))
	end
	print("")
end

printMatrix( matrix.mainRotation(4, 1, 3, math.pi/3) )

