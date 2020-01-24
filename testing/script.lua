print(_VERSION)
local a = matrix.random(4,5)

for r, row in a:rows() do
	print(("%10.4f "):rep(#row):format(table.unpack(row)))
end