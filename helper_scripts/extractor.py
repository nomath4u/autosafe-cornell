import csv

x = 0
y = 0
z = 0

with open('roll.txt', 'rb') as csvfile:
	reader = csv.reader(csvfile, delimiter=',')
	for row in reader:
		if(len(row) == 19):
			if(abs(float(row[3]) > x )):
				x = abs(float(row[3]))
			if(abs(float(row[4]) > y )):
				y = abs(float(row[4]))
			if(abs(float(row[5]) > z )):
				z = abs(float(row[5]))

	print ("X:{0} Y:{1} Z:{2}".format( x, y, z))
