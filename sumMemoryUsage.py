f = open("./ramprof.txt","r")

sumRamBytes = 0
for line in f.readlines():
	aux = int(line.split(" ")[0])
	sumRamBytes += aux

f = open("./flashprof.txt","r")

sumFlashBytes = 0
for line in f.readlines():
	aux = int(line.split(" ")[0])
	sumFlashBytes += aux

print "Total of ram memory bytes  : ", sumRamBytes," bytes"
print "Total of flash memory bytes:", sumFlashBytes," bytes"
