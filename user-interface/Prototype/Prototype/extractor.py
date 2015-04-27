import csv

x = 0
y = 0
z = 0

with open('impact_ashley_driving.txt', 'rb') as csvfile:
	reader = csv.reader(csvfile, delimiter=',')
	for row in reader:
		if(len(row) == 24):
			if(abs(float(row[0]) > x )):
				x = abs(float(row[0]))
			if(abs(float(row[1]) > y )):
				y = abs(float(row[1]))
			if(abs(float(row[2]) > z )):
				z = abs(float(row[2]))

	print ("X:{0} Y:{1} Z:{2}".format( x, y, z))
