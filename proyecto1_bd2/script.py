import time
import discogs_client
import sys

print("Preparing data extraction...You may stop running now without loss of data")
d = discogs_client.Client('ExampleApplication/0.1', user_token="JtaWScAASrqjHjaPHRsekLqcyrcJnLIFdRLRbOcu")

flag = 0
f = open("flag.txt","w")
f.write(str(flag))
f.close()

f = open("output.txt", "w")

i = int(sys.argv[1])
print("Extracting data of release wiht id",i)


release = d.release(i)

artists = release.artists

artist = artists[0]
r_title = str(release.title)
r_year = release.year
r_country = str(release.country) 
a_id = artist.id
a_name = artist.name
f.write(str(i))
f.write('\n')
f.write(r_title)
f.write('\n')
f.write(str(r_year))
f.write('\n')
f.write(r_country)
f.write('\n')
f.write(str(a_id))
f.write('\n')
f.write(a_name)
f.write('\n')

f.close()

flag = "1"

"""print("Waiting...You may stop running now without loss of data(time left: 4seg)")
time.sleep(1)
print("Waiting...You may stop running now without loss of data(time left: 3seg)")
time.sleep(1)"""
print("Waiting...You may stop running now without loss of data(time left: 2seg)")
time.sleep(1)
print("Waiting...You may stop running now without loss of data(time left: 1seg)")
time.sleep(1)
print("WARNING:You may no longer stop the program ")

f = open("flag.txt","w")
f.write(flag)
f.close()