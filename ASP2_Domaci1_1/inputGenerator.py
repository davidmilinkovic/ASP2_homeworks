f = open("Joe Biden.txt", encoding='utf8')
txt = f.read()
lines = txt.split('\n')
print(len(lines), " lines")
words = set()
for line in lines:
	words = words.union(line.split())
print(len(words), " words")
f.close()
'''
f= open("inBig.txt", "w", encoding='utf8')
f.write(str(len(words)))
f.write('\n')
for word in words:
	f.write(word + " " + word)
	f.write("\n")
f.close()'''
while True:
	pref = input()
	cnt = 0
	for word in words:
		if word.startswith(pref):
			cnt += 1
	print(cnt)
